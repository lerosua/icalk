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

using namespace gloox;

class Bodies;

/**
 * @brief 监听网络连接的类
 *
 */
class TalkConnect:public ConnectionListener,public LogHandler
{
	public:
		TalkConnect();
		~TalkConnect();
		void onConnect();
		void onDisconnect(ConnectionError er);
		void onResourceBindError(ResourceBindError error);
		void onSessionCreateError(SessionCreateError error);
		bool onTLSConnect(const CertInfo& info);
		void onStreamEvent(StreamEvent event);

		void handleLog(LogLevel level,LogArea area, const std::string& message);

};



#endif
