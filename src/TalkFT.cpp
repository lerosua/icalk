
#include <gtkmm.h>
#include <gtkmm/dialog.h>
#include "TalkFT.h"
#include "icalk.h"
#include "Bodies.h"

#define STOP_STATUS 0
#define RUN_RECV  1
#define RUN_SEND  2
#define PORT 7777
#define BLOCK_SIZE 200024

TalkFT::TalkFT():
	recvThread(this, &TalkFT::loopRecv)
	,sendThread(this, &TalkFT::loopSend)
    , R_RUNNING(STOP_STATUS)
    , S_RUNNING(STOP_STATUS)
	,m_bs_send(0)
{
}

TalkFT::~TalkFT()
{
	delete m_server;
	delete m_ft;

}

void TalkFT::initFT(Client * client_)
{
	m_ft = new SIProfileFT(client_, this);
	m_server =
	    new SOCKS5BytestreamServer(client_->logInstance(), PORT);
	ConnectionError le = ConnNoError;
	if ((le = m_server->listen()) != ConnNoError)
		PBUG("listen returned: %d\n", le);
	m_ft->registerSOCKS5BytestreamServer(m_server);
	m_ft->addStreamHost(JID("reflector.amessage.eu"),"reflector.amessage.eu",6565);
	//m_ft->addStreamHost(JID("proxy.jabber.org"), "208.245.212.98",
	//		    PORT);
	//m_ft->addStreamHost(client_->jid(),"192.168.1.103",PORT);
}


void *TalkFT::loopRecv(void *)
{
	PBUG(" TalkFT thread starting\n");
	while (R_RUNNING==RUN_RECV) {
		std::list < Bytestream * >::iterator it =
		    m_bs_list.begin();
		for (; it != m_bs_list.end(); ++it) {
			//PBUG(" TalkFT thread runing\n");
			(*it)->recv(100);
		}
	}
	return NULL;
}

void* TalkFT::loopSend(void* )
{
	ConnectionError se=ConnNoError;
	ConnectionError ce=ConnNoError;
	char input[BLOCK_SIZE];
	while(S_RUNNING==RUN_SEND)
	{
	if(m_server)
	{
		se = m_server->recv(1);
		if(se != ConnNoError)
		{
			PBUG("SOCKS5BytestreamServer returned: %d\n",se);
			//something error happen,please exit the send file
		}
	}

	if(m_bs_send && ! sendfile.eof())
	{
		if(m_bs_send->isOpen())
		{
			sendfile.read(input,BLOCK_SIZE);
			std::string content(input, sendfile.gcount());
			if(!m_bs_send->send(content))
			PBUG("file send shuld be return\n");
			;	//do something end the file send thread
		}
		m_bs_send->recv(1);
	}
	else if(m_bs_send)
	{
		m_bs_send->close();
	}
	}


	return NULL;

}

void TalkFT::handleFTSend(const JID& to, const std::string m_file)
{
	struct stat f_stat;
	if(stat(m_file.c_str(), &f_stat))
		return;
	uint32_t m_size = f_stat.st_size;
	sendfile.open(m_file.c_str(), std::ios_base::in | std::ios_base::binary);
	if(!sendfile)
		return;
	if(S_RUNNING == RUN_SEND)
		return;
	m_ft->requestFT(to, m_file, m_size);
	S_RUNNING = RUN_SEND;
	sendThread.start();
}

void TalkFT::handleFTBytestream(Bytestream * bs)
{
	PBUG("received bytestream type: %s\n",
	     bs->type() ==
	     Bytestream::S5B ? "sock5bytestream" : "ibbstream");
	// 如果发起者是本人的话，则表示这是发送的流
	if(bs->initiator().bare() == Bodies::Get_Bodies().get_jid().bare() )
		m_bs_send = bs;
	else 
		m_bs_list.push_back(bs);

	bs->registerBytestreamDataHandler(this);
	if (bs->connect()) {
		if (bs->type() == Bytestream::S5B)
			PBUG("ok! s5b connected to streamhost\n");
		else
			PBUG("ok! ibb sent request to remote entity\n");
	}
}


void TalkFT::handleFTRequest(const JID & from,
			     const std::string & sid,
			     const std::string & name, long size,
			     const std::string & hash,
			     const std::string & date,
			     const std::string & mimetype,
			     const std::string & desc, int /*stypes */ ,
			     long /*offset */ , long /*length */ )
{
	PBUG("received m_ft request from %s: %s (%ld bytes, sid : %s). hash: %s, date: %s, mime-type: %s\ndesc: %s\n", from.full().c_str(), name.c_str(), size, sid.c_str(), hash.c_str(), date.c_str(), mimetype.c_str(), desc.c_str());


	if(R_RUNNING == RUN_SEND)
		return;
	Gtk::MessageDialog askDialog("文件传输",
				     false /*use markup */ ,
				     Gtk::MESSAGE_QUESTION,
				     Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring msg_ =
	    from.full() + "想要给你发送文件" + name;
	askDialog.set_secondary_text(msg_);
	int result = askDialog.run();
	switch (result) {
	case (Gtk::RESPONSE_OK):
		{
			m_ft->acceptFT(from, sid, SIProfileFT::FTTypeS5B);
			R_RUNNING = RUN_RECV;
			recvThread.start();
			recvfile.open(name.c_str(), std::ios_base::out | std::ios_base::binary);
			break;
		}
	case (Gtk::RESPONSE_CANCEL):
		{
			m_ft->declineFT(from, sid,
					SIManager::RequestRejected,
					"just testing");
			break;
		}
	default:
		{
			m_ft->declineFT(from, sid,
					SIManager::RequestRejected,
					"just testing");
			break;
		}
	}

}

void TalkFT::handleBytestreamOpen(Bytestream * s5b)
{
	PBUG("stream opened\n");
}

void TalkFT::handleBytestreamClose(Bytestream * s5b)
{
	PBUG("stream closed\n");
	if(R_RUNNING == RUN_RECV)
	{
		R_RUNNING = STOP_STATUS;
		recvThread.join();
		recvfile.close();
	}
	if(S_RUNNING == RUN_SEND)
	{
		S_RUNNING = STOP_STATUS;
		sendThread.join();
		sendfile.close();
	}
	if(s5b)
		s5b->close();
	//s5b->removeBytestreamBytestreamDataHandler();
	//m_bs_list.remove(s5b);
	//m_ft->dispose(s5b);
}

void TalkFT::handleBytestreamError(Bytestream * s5b, const IQ & stanza)
{
	PBUG("socks5 stream error\n");
}

void TalkFT::handleBytestreamData(Bytestream * s5b,
				  const std::string & data)
{
	//PBUG("received %d bytes of data\n%s\n", data.length(),data.c_str());
	PBUG("received %d bytes of data\n\n", data.length());
	recvfile<<data;
}

void TalkFT::handleFTRequestError(const IQ & iq, const std::string & sid)
{
	PBUG("m_ft request error\n");
	if(R_RUNNING == RUN_RECV)
	{
		R_RUNNING = STOP_STATUS;
		recvThread.join();
		recvfile.close();
	}
	if(S_RUNNING == RUN_SEND)
	{
		S_RUNNING = STOP_STATUS;
		sendThread.join();
		sendfile.close();
	}
}
