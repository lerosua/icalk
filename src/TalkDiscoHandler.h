/*
 * =====================================================================================
 * 
 *       Filename:  TalkDiscoHandler.h
 * 
 *    Description:  server discover
 * 
 *        Version:  1.0
 *        Created:  2007年09月27日 20时10分25秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  TALKDISCOHANDLER_FILE_HEADER_INC
#define  TALKDISCOHANDLER_FILE_HEADER_INC
#include <gloox/discohandler.h>
#include <gloox/gloox.h>
using namespace gloox;

/** 
 * @brief 发掘服务的处理类
 */
class TalkDiscoHandler:public DiscoHandler {
      public:
	TalkDiscoHandler(){}
	~TalkDiscoHandler(){}

	void handleDiscoInfoResult(Stanza * stanza, int context  ) ;
	 void handleDiscoItemsResult(Stanza * stanza  ,    int context  ) ;
	void handleDiscoError(Stanza * stanza  ,int context  ) ;

};



#endif   /* ----- #ifndef TALKDISCOHANDLER_FILE_HEADER_INC  ----- */

