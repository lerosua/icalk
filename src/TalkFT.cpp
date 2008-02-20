
#include <gtkmm.h>
#include <gtkmm/dialog.h>
#include "TalkFT.h"

TalkFT::TalkFT()
{
}

void TalkFT::handleFTRequest(const JID & from,
			const std::string& sid,
			     const std::string & name, long size,
			     const std::string & hash,
			     const std::string & date,
			     const std::string & mimetype,
			     const std::string & desc, int /*stypes */ ,
			     long /*offset */ , long /*length */ )
{
	printf
	    ("received ft request from %s: %s (%ld bytes, sid : %s). hash: %s, date: %s, mime-type: %s\ndesc: %s\n",
	     from.full().c_str(), name.c_str(), size,sid.c_str(), hash.c_str(),
	     date.c_str(), mimetype.c_str(), desc.c_str());


	Gtk::MessageDialog askDialog("文件传输", false /*use markup*/,Gtk::MESSAGE_QUESTION,Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring msg_ = from.full()+"想要给你发送文件"+name;
	askDialog.set_secondary_text(msg_);
	int result = askDialog.run();
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			ft->acceptFT( from, sid, SIProfileFT::FTTypeS5B );
			//ft->acceptFT( from, sid, SIProfileFT::FTTypeIBB );
			//ft->declineFT(from, sid, SIManager::RequestRejected,
		      //"just testing");
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			ft->declineFT(from, sid, SIManager::RequestRejected,
		      "just testing");
			break;
		}
		default:
		{
			ft->declineFT(from, sid, SIManager::RequestRejected,
		      "just testing");
			break;
		}
	}

	//ft->acceptFT( from, sid, SIProfileFT::FTTypeS5B );
	//ft->declineFT(from, sid, SIManager::RequestRejected,		      "just testing");
}

void TalkFT::handleBytestreamOpen(Bytestream* s5b)
{
	printf("stream opened\n");
}

void TalkFT::handleBytestreamClose(Bytestream* s5b)
{
	printf("stream closed\n");
	//s5b->removeBytestreamBytestreamDataHandler();
	//m_bsslist.remove(*s5b);
}

void TalkFT::handleBytestreamError(Bytestream* s5b, const IQ& stanza)
{
	printf("socks5 stream error\n");
}

void TalkFT::handleBytestreamData(Bytestream* s5b, const std::string& data)
{
	printf("received %d bytes of data\n%s\n", data.length(),data.c_str() );
}
