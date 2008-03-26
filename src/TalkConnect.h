/*
* =====================================================================================
*
*       Filename:  TalkConnect.h
*
*    Description:  连接处理类
*
*        Version:  1.0
*        Created:  2007年07月1日 16时52分02秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/
#ifndef _TALKCONNECT_H_
#define _TALKCONNECT_H_

#include <gloox/connectionlistener.h>
#include <gloox/loghandler.h>
#include <iostream>
#include "MVC.h"

using namespace gloox;

using namespace std;

class Bodies;

/**
 * @brief 监听网络连接的类
 *
 */

class TalkConnect: public ConnectionListener, public LogHandler
{

public:
        TalkConnect();
        virtual ~TalkConnect();
        void Observer(CLogin::Handler* f_handler, CLogin::Model::Func f_call);

protected:
        virtual void onConnect();
        virtual void onDisconnect(ConnectionError er);
        virtual void onResourceBindError(ResourceBindError error);
        virtual void onSessionCreateError(SessionCreateError error);
        virtual bool onTLSConnect(const CertInfo& info);
        virtual void onStreamEvent(StreamEvent event);
        virtual void handleLog(LogLevel level, LogArea area, const std::string& message);

private:
        CLogin::Handler* m_login_handler;
        CLogin::Model::Func m_login_call;
};



#endif
