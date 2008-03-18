/*
* =====================================================================================
*
*       Filename:  TalkMsg.cpp
*
*    Description:  a jabber soft
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


#include "TalkMsg.h"
#include "Bodies.h"
#include <assert.h>
#include <glib/gi18n.h>
#include <gloox/xhtmlim.h>

TalkMsg::TalkMsg()
{}


void TalkMsg::handleMessage(Message * stanza, MessageSession *session)
{}

void TalkMsg::handleMessage(const Message & stanza, MessageSession *session)
{
        const JID target = session->target();
        //Buddy* buddy=Bodies::Get_Bodies().get_buddy_list().find_buddy(stanza.from().bare());
        Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(target.bare());
        assert(buddy != NULL);

        if (!buddy) {
                DLOG("buddy is NULL");
        }

        /* 发送消息已经显示的事件*/
        buddy->raiseMessageEvent(MessageEventDisplayed);

        /*取消打字事件*/
        Bodies::Get_Bodies().get_msg_window().showTypeImage(false);

        Glib::ustring sender;

        Glib::ustring msg = stanza.body();

        MsgPage* page_ = buddy->get_page();

        sender = buddy->get_nickname();

        if (sender.empty())
                sender = target.username();

        if (NULL == session) {
                DLOG("session is NULL");
                return ;
        }

        //page_->showMessage(sender,msg);
        const XHtmlIM* x = stanza.findExtension<XHtmlIM>(ExtXHtmlIM);

        if (x) {
                PBUG("the msg has use XhtmlIM\n");
        }

        const DelayedDelivery* dd = stanza.when();

        if (dd) {
                DLOG("message time is %s\n", dd->stamp().c_str());
                page_->showMessage(sender, msg, dd->stamp());
        } else
                page_->showMessage(sender, msg);

        Bodies::Get_Bodies().get_msg_window().add_page(*page_);

        Bodies::Get_Bodies().get_msg_window().show();
}

void TalkMsg::handleMessageEvent( const JID& from, MessageEventType event)
{
        PBUG( "received event: %d from: %s\n", event, from.full().c_str() );
        Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(from.bare());

        switch (event) {

        case 0:
                /*取消打字事件*/
                Bodies::Get_Bodies().get_msg_window().showTypeImage(false);
                break;

        case 1:
                /*消息已经被离线的接受者的服务保存*/
                buddy->get_page()->showSystemMsg(_("\t\tThe Message has stored by the receiver's server\n"));
                break;

        case 2:
                /*消息已经传递给接受者*/
                break;

        case 4:
                /*消息已经被显示*/
                break;

        case 8:
                /*用户正在打字事件*/
                Bodies::Get_Bodies().get_msg_window().showTypeImage(true);
                break;

        default:
                break;
        }

}

void TalkMsg::handleChatState( const JID& from, ChatStateType state )
{
        //PBUG( "received state: %d from: %s\n", state, from.full().c_str() );
        Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(from.bare());

        switch (state) {

        case 1:
                /*用户积极参与对话*/
                break;

        case 2:
                /*用户正在打字 */
                Bodies::Get_Bodies().get_msg_window().showTypeImage(true);
                break;

        case 4:
                /*用户停止了打字*/
                Bodies::Get_Bodies().get_msg_window().showTypeImage(false);
                break;

        case 8:
                /*用户不积极参与对话*/
                break;

        case 16:
                /*用户关闭了对话*/
                buddy->get_page()->showSystemMsg(_("\n\t\tUser has closed the window\n\n"));
                break;

        default:
                break;
        }

}

/**
 * 需要用JID中的bare（）得到id字符串，然后用id在BuddyList里找到
 * Buddy。然后用现在这个session填充Buddy里的session,跟着就注册
 * session里的消息处理（不然不会出消息了），和对话中的一些事件。
 * 调出对话框。大概是这个思路了
 */
void TalkMsg::handleMessageSession( MessageSession *session )
{
        //PBUG("got new session\n");
        JID receipient = session->target();
        std::string id = receipient.bare();
        std::cout << "session id is " << id << std::endl;
        Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(id);
        // assert(buddy != NULL);

        if (buddy != NULL)
                buddy->set_session(session, this);

}



