/*
 * =====================================================================================
 *
 *       Filename:  TalkMsg.h
 *
 *    Description:  TalkMsg类，管理消息处理.
 *
 *        Version:  1.0
 *        Created:  2007年05月24日 19时41分25秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#ifndef _TALK_H_
#define _TALK_H_

#include <iostream>
#include <cstdlib>
#include <gloox/client.h>
#include <gloox/messagehandler.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/chatstatehandler.h>

using namespace gloox;

class TalkRoom;
/**
 *  @brief 用来处理消息，事件的类。多继承。
 *  @author lerosua <lerosua@gmail.com> wind <xihels@gmail.com>
 */
class TalkMsg:public MessageHandler,public MessageEventHandler,public ChatStateHandler, public MessageSessionHandler 
{
	public:
		TalkMsg();
		void handleMessage(Stanza * stanza,MessageSession *session = 0) ;
		/**
		 * @brief 处理消息前的事件。
		 * @param from 发消息事件来的对方的jid
		 * @param event 消息事件的类型。
		 * @b MessageEventType :
		 * @li   MessageEventCancel    = 0 取消打字事件。
		 * @li   MessageEventOffine    = 1 消息已经被离线的接受者的服务保存。
		 * @li   MessageEventDelivered = 2 消息已经传递给接受者。
		 * @li   MessageEventDisplayed = 4 消息已经被显示。
		 * @li   MessageEventComposing = 8 用户正在打字事件。
		 */
		void handleMessageEvent( const JID& from, MessageEventType event );
		/**
		 * @brief 处理用户状态 （似乎和handleMessage相类似）
		 * @param from 发消息事件来的对方的jid
		 * @param state 用户的状态。
		 * @b ChatStateType :
		 * @li   ChatStateActive     = 1  用户积极参与对话
		 * @li   ChatStateComposing  = 2  用户正在打字
		 * @li   ChatStatePaused     = 4  用户停止了打字
		 * @li   ChatStateInactive   = 8  用户不积极参与对话
		 * @li   ChatStateGone	     = 16 用户关闭了对话
		 */
		void handleChatState( const JID& from, ChatStateType state );
		void handleMessageSession( MessageSession *session );
};
#endif
