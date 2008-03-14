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

class TalkDiscoHandler: public DiscoHandler {

public:
        TalkDiscoHandler() {}

        ~TalkDiscoHandler() {}

        void handleDiscoInfoResult(IQ * stanza, int context  ) ;
        void handleDiscoItemsResult(IQ * stanza  ,    int context  ) ;
        void handleDiscoError(IQ * stanza  , int context  ) ;

        void handleDiscoInfo(const JID& from, const Disco::Info& info, int context);
        void handleDiscoItems(const JID& from, const Disco::Items& items, int context);
        void handleDiscoError(const JID& from, const Error* error, int context);
        bool handleDiscoSet(IQ* iq);

};



#endif   /* ----- #ifndef TALKDISCOHANDLER_FILE_HEADER_INC  ----- */

