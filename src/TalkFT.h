/*
 * =====================================================================================
 * 
 *       Filename:  TalkFT.h
 * 
 *    Description:  gloox的文件传输模块
 * 
 *        Version:  1.0
 *        Created:  2007年08月10日 19时01分46秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  TALKFT_FILE_HEADER_INC
#define  TALKFT_FILE_HEADER_INC

#include <gloox/disco.h>
#include <gloox/client.h>
#include <gloox/siprofileft.h>
#include <gloox/socks5bytestreamdatahandler.h>
#include <gloox/siprofilefthandler.h>
using namespace gloox;

#include <unistd.h>
#include <stdio.h>
#include <string>

#if defined( WIN32 ) || defined( _WIN32 )
# include <windows.h>
#endif

/**
 * Receives one file and displayes it. Does not save anything.
 */
class TalkFT:public SIProfileFTHandler, public SOCKS5BytestreamDataHandler {
      public:
	TalkFT();
	void set_sipFT(Client * client_) {
		ft = new SIProfileFT(client_, this);
		ft->addStreamHost( JID("proxy.jabber.org"),"208.245.212.98", 777 );
	}

	void streamLoopRecv() {
		std::list<SOCKS5Bytestream*>::iterator it = m_s5bslist.begin();
		for(; it!=m_s5bslist.end();++it)
			(*it)->recv(100);
	}

	void handleFTRequest(const JID & from, const std::string & id,
				const std::string& sid,
			       const std::string & name, long size,
			       const std::string & hash,
			       const std::string & date,
			       const std::string & mimetype,
			       const std::string & desc, int /*stypes */ ,
			       long /*offset */ , long /*length */ );
	void handleFTRequestError(Stanza * /*stanza */ ) {
		printf("ft request error\n");
	}

	void handleFTSOCKS5Bytestream(SOCKS5Bytestream * s5b) {
		printf("received socks5 bytestream\n");
		m_s5bslist.push_back(s5b);
		s5b->registerSOCKS5BytestreamDataHandler(this);
		if (s5b->connect()) {
			printf("ok! s5b connected to streamhost\n");
		}
	}

	void handleSOCKS5Data(SOCKS5Bytestream* s5b, const std::string& data);
	void handleSOCKS5Error(SOCKS5Bytestream* s5b, Stanza* stanza);
	void handleSOCKS5Open(SOCKS5Bytestream* s5b);
	void handleSOCKS5Close(SOCKS5Bytestream* s5b);

      private:
	SIProfileFT * ft;
	std::list<SOCKS5Bytestream*> m_s5bslist;
};



#endif   /* ----- #ifndef TALKFT_FILE_HEADER_INC  ----- */

