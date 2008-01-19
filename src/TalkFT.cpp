
#include <gtkmm.h>
#include <gtkmm/dialog.h>
#include "TalkFT.h"

TalkFT::TalkFT()
{
}

void TalkFT::handleFTRequest(const JID & from, const std::string & id,
			const std::string& sid,
			     const std::string & name, long size,
			     const std::string & hash,
			     const std::string & date,
			     const std::string & mimetype,
			     const std::string & desc, int /*stypes */ ,
			     long /*offset */ , long /*length */ )
{
	printf
	    ("received ft request from %s: %s (%ld bytes). hash: %s, date: %s, mime-type: %s\ndesc: %s\n",
	     from.full().c_str(), name.c_str(), size, hash.c_str(),
	     date.c_str(), mimetype.c_str(), desc.c_str());


	Gtk::MessageDialog askDialog("文件传输", false /*use markup*/,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring msg_ = from.full()+"想要给你发送文件"+name;
	askDialog.set_secondary_text(msg_);
	int result = askDialog.run();
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			ft->acceptFT( from, id, SIProfileFT::FTTypeS5B );
			//ft->declineFT(from, id, SIManager::RequestRejected,
		      //"just testing");
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			ft->declineFT(from, id, SIManager::RequestRejected,
		      "just testing");
			break;
		}
		default:
		{
			ft->declineFT(from, id, SIManager::RequestRejected,
		      "just testing");
			break;
		}
	}

	//ft->acceptFT( from, id, SIProfileFT::FTTypeS5B );
	//ft->declineFT(from, id, SIManager::RequestRejected,		      "just testing");
}

void TalkFT::handleSOCKS5Open(SOCKS5Bytestream* s5b)
{
	printf("stream opened\n");
}

void TalkFT::handleSOCKS5Close(SOCKS5Bytestream* s5b)
{
	printf("stream closed\n");
	s5b->removeSOCKS5BytestreamDataHandler();
	//m_s5bslist.remove(*s5b);
}

void TalkFT::handleSOCKS5Error(SOCKS5Bytestream* s5b, Stanza* stanza)
{
	printf("socks5 stream error\n");
}

void TalkFT::handleSOCKS5Data(SOCKS5Bytestream* s5b, const std::string& data)
{
	printf("received %d bytes of data\n%s\n", data.length(),data.c_str() );
}
