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
#include <gloox/bytestreamdatahandler.h>
#include <gloox/siprofilefthandler.h>
using namespace gloox;

#include <unistd.h>
#include <stdio.h>
#include <string>
#include "XPThread.h"

#if defined( WIN32 ) || defined( _WIN32 )
# include <windows.h>
#endif

/**
 * 文件传输的类，负责接收与发送文件
 */
class TalkFT:public SIProfileFTHandler, public BytestreamDataHandler {
      public:
	TalkFT();
	void set_sipFT(Client * client_) {
		ft = new SIProfileFT(client_, this);
		ft->addStreamHost( JID("proxy.jabber.org"),"208.245.212.98", 7777 );
	}

	//int start_recv(){ return pthread_create(&recv_id,NULL, (void*)loopRecv,NULL); }

	void* loopRecv(void *) {
		std::list<Bytestream*>::iterator it = m_bsslist.begin();
		for(; it!=m_bsslist.end();++it)
			(*it)->recv(100);
		return NULL;
	}

	void handleFTRequest(const JID & from, 
				const std::string& sid,
			       const std::string & name, long size,
			       const std::string & hash,
			       const std::string & date,
			       const std::string & mimetype,
			       const std::string & desc, int /*stypes */ ,
			       long /*offset */ , long /*length */ );
	void handleFTRequestError(const IQ& iq , const std::string& sid) {
		printf("ft request error\n");
	}

	void handleFTBytestream(Bytestream * bs) {
		printf("received bytestream type: %s\n",bs->type()==Bytestream::S5B? "sock5bytestream" : "ibbstream");
		m_bsslist.push_back(bs);
		bs->registerBytestreamDataHandler(this);
		if (bs->connect()) {
			if( bs->type() == Bytestream::S5B )
				  printf( "ok! s5b connected to streamhost\n" );
			else
				  printf( "ok! ibb sent request to remote entity\n" );
	      }
	}

	virtual const std::string handleOOBRequestResult(const JID& from,const std::string& sid)
	{
		return std::string();
	};


	void handleBytestreamData(Bytestream* bs, const std::string& data);
	void handleBytestreamError(Bytestream* bs, const IQ& stanza);
	void handleBytestreamOpen(Bytestream* bs);
	void handleBytestreamClose(Bytestream* bs);

      private:
	SIProfileFT * ft;
	std::list<Bytestream*> m_bsslist;
	XPThread<TalkFT>	recvThread;
	//pthread_t recv_id  ;
	
      private:
	TalkFT(const TalkFT& rhs);	//不用
	TalkFT& operator = (const TalkFT& rh); // 不用
};



#endif   /* ----- #ifndef TALKFT_FILE_HEADER_INC  ----- */

