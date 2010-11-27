
#include <gtkmm.h>
#include <gtkmm/dialog.h>
#include <glib/gi18n.h>
#include <libgen.h>
#include "icalk.h"
#include "Bodies.h"
#include "FTWidget.h"
#include "TalkFT.h"
#include "FT.h"

#define STOP_STATUS 0
#define RUN_RECV  1
#define RUN_SEND  2
#define LOCALPORT 8010
#define PORT 7777
#define BLOCK_SIZE 200024

#define SEND_TYPE _("send")
#define RECV_TYPE _("receive")

TalkFT::TalkFT(Client* client_): m_client(client_)
                , recvThread(this, &TalkFT::loopRecv)
                , sendThread(this, &TalkFT::loopSend)
                , R_RUNNING(STOP_STATUS)
                , S_RUNNING(STOP_STATUS)
                , m_ft(NULL)
                , m_ftwidget(NULL)
                , m_server(NULL)
                , recvCount(0)
                , sendCount(0)
{}

TalkFT::~TalkFT()
{
		R_RUNNING = STOP_STATUS;
		S_RUNNING = STOP_STATUS;
		recvThread.join();
		sendThread.join();
        delete m_server;
        delete m_ft;
        m_client = NULL;
        m_server = NULL;
        m_ft = NULL;
        m_ftwidget = NULL;

}

void TalkFT::initFT()
{
        m_ft = new SIProfileFT(m_client, this);
        m_server =
                new SOCKS5BytestreamServer(m_client->logInstance(), LOCALPORT);
        ConnectionError le = ConnNoError;

        if ((le = m_server->listen()) != ConnNoError) {
                DLOG("listen returned: %d\n", le);
                delete m_server;
                m_server = new SOCKS5BytestreamServer(m_client->logInstance(), PORT + 1);

                if ((le = m_server->listen()) != ConnNoError)
                        DLOG("listen2 returned: %d\n", le);

        }

        m_ft->registerSOCKS5BytestreamServer(m_server);

        m_ft->addStreamHost(m_client->jid(), "localhost", 6666);
        m_ft->addStreamHost(JID("reflector.amessage.eu"), "reflector.amessage.eu", 6565);
}


void *TalkFT::loopRecv(void *)
{
        DLOG(" TalkFT thread starting\n");

        while (R_RUNNING == RUN_RECV) {
                if (recvCount == 0) {
                        bs_recvList.clear();
                        DLOG("all bytestream clear \n");
                        break;
                }

                std::list < Bytestream * >::iterator it =

                        bs_recvList.begin();

                for (; it != bs_recvList.end(); ++it) {
                        (*it)->recv(100);
                }
        }

        return NULL;
}

void* TalkFT::loopSend(void* )
{
        DLOG(" TalkFT send thread starting\n");
        ConnectionError se = ConnNoError;
        ConnectionError ce = ConnNoError;
        char input[BLOCK_SIZE];

        while (S_RUNNING == RUN_SEND) {
                if (m_server) {
                        se = m_server->recv(100);
                        if (se != ConnNoError) {
                                DLOG("SOCKS5BytestreamServer returned: %d\n", se);
                                //something error happen,please exit the send file
                        }
                }
                if ( 0 == sendCount ) {
                        bs_sendList.clear();
                        DLOG("all send bytestream clear\n");
                        break;
                }

                std::list<Bytestream*>::iterator it = bs_sendList.begin();

                for (; it != bs_sendList.end(); ++it) {

                        FILELIST::iterator iter = sfilelist.find((*it)->sid());
						if(iter == sfilelist.end())
								break;
                        XferFile* sendfile = (*iter).second;

                        if ( !sendfile->eof()) {
                                if ((*it)->isOpen()) {
                                        int per_percent = sendfile->getPercent();
                                        sendfile->read(input, BLOCK_SIZE);
                                        std::string content(input, sendfile->gcount());
                                        int new_percent = sendfile->getPercent();
										DLOG("loop send %ld\n",sendfile->gcount());

                                        if (!(*it)->send(content))
                                                DLOG("file send shuld be return\n");

                                        if (new_percent >= per_percent)
                                                m_ftwidget->updateXfer((*it)->sid(), new_percent);

                                        ; //do something end the file send thread
                                }

                                (*it)->recv(1);
                        } else if ((*it)) {
							//(*it)->recv(1);
							(*it)->close();
							DLOG(" == send end loop ==        ");
                        }
                }
				DLOG("send thread ");

        }


        DLOG(" TalkFT send thread leaving\n");
        return NULL;

}

bool TalkFT::isSend(Bytestream* bs)
{
        /** 比较流的发起人，如果是自己则返回真。*/
        return bs->initiator().bare() == Bodies::Get_Bodies().get_jid().bare();

}

void TalkFT::handleFTSend(const JID& to, const std::string& m_file, int streamtypes)
{

        XferFile* sendfile = new XferFile();
        sendfile->open(m_file.c_str(), std::ios_base::in | std::ios_base::binary);

        std::string m_filename(basename(const_cast<char *>(m_file.c_str())));
		std::string sid;
		long m_size = sendfile->getTotalsize();
		if(streamtypes){
			sid = m_ft->requestFT(to, m_filename, m_size,"","image","2010","",SIProfileFT::FTTypeIBB);
		}
		else
			sid = m_ft->requestFT(to, m_filename, m_size);

        if (sid.empty()) {
                DLOG("requestFT error\n");
                return ;
        }

        sendCount++;
        sfilelist.insert(sfilelist.end(), FILELIST::value_type(sid, sendfile));

        //建立文件传输窗口
        Bodies::Get_Bodies().get_main_window().on_fileXer_activate();

        if (m_ftwidget == NULL)
                m_ftwidget = Bodies::Get_Bodies().get_main_window().get_ftwidget();

        const Glib::ustring& f_sid = Glib::ustring(sid);

        m_ftwidget->addXfer(f_sid, m_filename, to.bare(), m_size, SEND_TYPE);

        if (S_RUNNING != RUN_SEND) {
                S_RUNNING = RUN_SEND;
                sendThread.start();
        }

}


void TalkFT::handleFTBytestream(Bytestream * bs)
{
        DLOG("received bytestream type: %s from %s \n",
             bs->type() ==
             Bytestream::S5B ? "sock5bytestream" : "ibbstream", bs->sid().c_str());
        // 如果发起者是本人的话，则表示这是发送的流

        if (isSend(bs)) {
                bs_sendList.push_back(bs);
        } else
                bs_recvList.push_back(bs);

        bs->registerBytestreamDataHandler(this);

        if (bs->connect()) {
                if (bs->type() == Bytestream::S5B)
                        PBUG("ok! s5b connected to streamhost\n");
                else
                        DLOG("ok! ibb sent request to remote entity\n");
        } else {
                DLOG("s5b bytestream connect false\n");
                //m_ft->dispose(bs);
        }
}


void TalkFT::handleFTRequest(const JID & from,
			     const JID & to,
                             const std::string & sid,
                             const std::string & name, long size,
                             const std::string & hash,
                             const std::string & date,
                             const std::string & mimetype,
                             const std::string & desc, int /*stypes */ )
{
        DLOG("received m_ft request from %s: %s (%ld bytes, sid : %s). hash: %s, date: %s, mime-type: %s\ndesc: %s\n", from.full().c_str(), name.c_str(), size, sid.c_str(), hash.c_str(), date.c_str(), mimetype.c_str(), desc.c_str());

        if (R_RUNNING == RUN_SEND)
                return ;
		if(0 == desc.compare(0,5,"image")){
				m_ft->acceptFT(from, sid, SIProfileFT::FTTypeIBB);
				recvCount++;
				if (R_RUNNING != RUN_RECV) {
						R_RUNNING = RUN_RECV;
						recvThread.start();
				}

            XferFile* recvfile = new XferFile();
            recvfile->open(name.c_str(), std::ios_base::out | std::ios_base::binary);
            recvfile->setTotalsize(size);
            rfilelist.insert(rfilelist.end(), FILELIST::value_type(sid, recvfile));
            //建立文件传输窗口
            Bodies::Get_Bodies().get_main_window().on_fileXer_activate();
            if (m_ftwidget == NULL)
                    m_ftwidget = Bodies::Get_Bodies().get_main_window().get_ftwidget();
            const Glib::ustring& f_sid = Glib::ustring(sid);
            m_ftwidget->addXfer(f_sid, name, from.bare(), size, RECV_TYPE);

			//显示到用户聊天窗口中，先显示loading占位，传输完毕后用真正图片替代
			Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(from.bare());
			assert(buddy != NULL);
			buddy->recvPicture(name);

			return ;
		}

        Gtk::MessageDialog askDialog(_("File Transport"),
                                     false /*use markup */ ,
                                     Gtk::MESSAGE_QUESTION,
                                     Gtk::BUTTONS_OK_CANCEL);
        Glib::ustring msg_ =
                from.full() + _(" want to send you a file: ") + name;

        askDialog.set_secondary_text(msg_);

        int result = askDialog.run();

        switch (result) {
        case (Gtk::RESPONSE_OK): {
                        m_ft->acceptFT(from, sid, SIProfileFT::FTTypeS5B);
                        recvCount++;

                        if (R_RUNNING != RUN_RECV) {
                                R_RUNNING = RUN_RECV;
                                recvThread.start();
                        }

                        XferFile* recvfile = new XferFile();
                        recvfile->open(name.c_str(), std::ios_base::out | std::ios_base::binary);
                        recvfile->setTotalsize(size);
                        rfilelist.insert(rfilelist.end(), FILELIST::value_type(sid, recvfile));
                        //建立文件传输窗口
                        Bodies::Get_Bodies().get_main_window().on_fileXer_activate();
                        if (m_ftwidget == NULL)
                                m_ftwidget = Bodies::Get_Bodies().get_main_window().get_ftwidget();
                        const Glib::ustring& f_sid = Glib::ustring(sid);
                        m_ftwidget->addXfer(f_sid, name, from.bare(), size, RECV_TYPE);
                        break;
                }

        case (Gtk::RESPONSE_CANCEL): {
                        m_ft->declineFT(from, sid,
                                        SIManager::RequestRejected,
                                        "just testing");
                        break;
                }

        default: {
                        m_ft->declineFT(from, sid,
                                        SIManager::RequestRejected,
                                        "just testing");
                        break;
                }
        }

}

void TalkFT::handleBytestreamOpen(Bytestream * s5b)
{
        DLOG("stream opened\n");
}

void TalkFT::handleBytestreamClose(Bytestream * s5b)
{
        DLOG("stream closed\n");

        //区别对待发送流与接收流

        m_ftwidget->doneXfer(s5b->sid());

        if (isSend(s5b)) {
                sendCount = sendCount - 1;
                /** 如果发送文件数为0,则关闭发送线程 */

                if (sendCount <= 0) {
                        S_RUNNING = STOP_STATUS;
                        sendThread.join();
                }

				bs_sendList.remove(s5b);
				s5b->removeBytestreamDataHandler();
                FILELIST::iterator s_iter = sfilelist.find(s5b->sid());

                if (s_iter != sfilelist.end()) {
                        (*s_iter).second->close();
						if(s5b->type() == Bytestream::IBB ){
							Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(s5b->target().bare());
							buddy->finish_recv_pic( (*s_iter).second->getFilePath());
						}
                        delete (*s_iter).second;
                        sfilelist.erase(s_iter);
                }
			   DLOG(" close send bs sid %s\n", s5b->sid().c_str());
				//m_ft->dispose(s5b);
				//s5b->close();
        } else {
                recvCount = recvCount - 1;

                /**如果接收文件数为0，则关闭接收线程*/

                if (recvCount <= 0) {
                        R_RUNNING = STOP_STATUS;
                        recvThread.join();
                }

				bs_sendList.remove(s5b);
                s5b->removeBytestreamDataHandler();
                FILELIST::iterator iter = rfilelist.find(s5b->sid());
                if (iter != rfilelist.end()) {
                        (*iter).second->close();

						if(s5b->type() == Bytestream::IBB ){
							Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(s5b->initiator().bare());
							buddy->finish_recv_pic( (*iter).second->getFilePath());
						}
                        delete (*iter).second;
                        rfilelist.erase(iter);
                        DLOG(" close recv bs sid %s\n", s5b->sid().c_str());
                }
				//m_ft->dispose(s5b);
				//s5b->close();
        }
}

void TalkFT::handleBytestreamError(Bytestream * s5b, const IQ & stanza)
{
        DLOG("socks5 stream error\n");
        handleBytestreamClose(s5b);
        m_ftwidget->doneXfer(s5b->sid(), 1);
}

void TalkFT::handleBytestreamData(Bytestream * s5b,
                                  const std::string & data)
{
        FILELIST::iterator iter = rfilelist.find(s5b->sid());
        int per_percent = (*iter).second->getPercent();
        (*iter).second->write(data, data.length());
        int new_percent = (*iter).second->getPercent();

        if (new_percent > per_percent)
                m_ftwidget->updateXfer(s5b->sid(), new_percent);
		(*iter).second->getTotalsize();
		if ((*iter).second->end())
		{
			DLOG(" recv filer close the s5b\n");
			s5b->close();
		}
}

void TalkFT::handleFTRequestError(const IQ & iq, const std::string & sid)
{
        DLOG("m_ft request error sid =%s\n", sid.c_str());
        sendCount = sendCount - 1;
        /** 如果发送文件数为0,则关闭发送线程 */

        if (sendCount <= 0) {
                S_RUNNING = STOP_STATUS;
                sendThread.join();
        }

        FILELIST::iterator s_iter = sfilelist.find(sid);

        if (s_iter != sfilelist.end()) {
                (*s_iter).second->close();
                delete (*s_iter).second;
                sfilelist.erase(s_iter);
                DLOG("close send bs sid %s\n", sid.c_str());
        }

        Gtk::MessageDialog askDialog(_("File Transport"),
                                     false /*use markup */ ,
                                     Gtk::MESSAGE_QUESTION,
                                     Gtk::BUTTONS_OK_CANCEL);

        Glib::ustring msg_(_("File Send Error"));

        askDialog.set_secondary_text(msg_);

        int result = askDialog.run();

        switch (result) {
        case (Gtk::RESPONSE_OK):
                                case(Gtk::RESPONSE_CANCEL):
                                                break;
        }

}
