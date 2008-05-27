/*
* =====================================================================================
*
*       Filename:  Buddy.h
*
*    Description:  好友的数据结构
*
*        Version:  1.0
*        Created:  2007年06月24日 22时12分00秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:  cyclone
*
* =====================================================================================
*/

#ifndef _BUDDY_H_
#define _BUDDY_H_

#include <string>

#include <gloox/jid.h>
#include <gloox/rosteritem.h>
#include <gloox/vcard.h>
#include <gloox/messagesession.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatefilter.h> 
//#include <gloox/inbandbytestream.h>
#include "MsgPage.h"
#include "Unit.h"

using namespace gloox;

/**
 * @brief 好友的数据结构类
 *
 */

class TalkMsg;

class Buddy
{

public:
        Buddy();
        explicit Buddy(const RosterItem&);
        virtual ~Buddy();
        typedef std::map<std::string, Resource*> ResourceMap;
	typedef std::vector<std::string> MessageList;
        void set_jid(const JID& f_jid) ;

        /**返回JID的bare（）值*/
        const std::string& get_jid() const
        {
                return jid.bare();
        }

        /** 返回JID*/
        const JID& getJID() const
        {
                return jid;
        }

        /**得到constact/roster里的用户名，此nickname与rosteritem里name对应*/
        const std::string& get_nickname() const
        {
                return nickname;
        }

        /** 设置昵称*/
        void set_nickname(const std::string& f_name);
        /** 将VCard信息填充进Buddy里*/
        void set_vcard(const VCard*);
        /** 返回Buddy里的VCard信息*/
        const VCard* get_vcard() const
        {
                return vcard;
        }

        /** 启动更新VCard流程*/
        void refreshVcard();
        /** 更新Buddy的状态，主要用于更新VCard中的图标信息*/
        void refreshinfo();

        /** 返回好友的定阅状态*/
        SubscriptionType getSubscription()
        {
                return subscription;
        }

        /**
         * @brief 得到好友状态
         */
        Presence::PresenceType get_status()const
        {
                return status;
        }

        /**
         * @brief 根据得到的好友状态更新列表中好友的状态
         * @param presence_ 状态变量
         */
        void set_status(Presence::PresenceType presence_)
        {
                status = presence_;
        }

        /**
         * @brief 获取当前的签名
         */
        const std::string& get_sign_msg()
        {
                return sign_msg;
        }

        /**
         * @brief 设置签名信息
         * @param msg_ 需要设置的签名信息
         */
        void set_sign_msg(const std::string& msg_)
        {
                sign_msg = msg_;
        }

        /**
         * @brief 获取所属的组
         */
        StringList getGroups()
        {
                return groups;
        }

        /**
         * @brief 返回当前标签页
         */
        MsgPage* get_page();
	/** 返回当前有效的page或新建一个page */
	MsgPage* new_page();

        /**
         *@brief 用于发起一个针对jid的会话
         *@param jid 会话的对象
         *@note 完成的功能：生成会话，生成标签页(调用@link set_session set_session @endlink)，发送正在打字的事件。
         */
        void new_session();
	/** 将消息存于队列中*/
	void storeMessage(const Message& f_message);

        /**
         * @brief 用于将Buddy里的session变量设置为给定的session，并注册
         * 会话处理函数和生成新的标签页。
         * @param session_ 给定的会话参数
         * @param handler  需要绑定的会话处理类
         */
        void set_session(MessageSession* session_,
                         TalkMsg* handler);
        /**
         * @brief @b close_session(): 关闭会话和把标签页指针置空。
         * @param closepage 是否关闭窗口，默认为关闭。
         * @note  这个函数应该在关闭了标签页对话框后调用。
         */
        void close_session(bool closepage = true);
        /**
         * @brief 获取当前的session
         * @return 返回此Buddy的会话
         */
        MessageSession* get_session()
        {
                return session;
        }

        /**
         * @brief 获取消息事件处理(message_event_filter)
         */
        MessageEventFilter* get_message_event_filter( ) const
        {
                return message_event_filter;
        }

        /**
         * @brief 获取用户状态处理(chat_state_filter)
         */
        ChatStateFilter* get_chat_state_filter() const
        {
                return chat_state_filter;
        }

        Glib::RefPtr<Gdk::Pixbuf> getLogo()
        {
                return logo;
        }

        void setLogo(Glib::RefPtr<Gdk::Pixbuf > f_logo)
        {
                logo = f_logo ;
        }

        /**
         * @brief 设置用户事件,如打字，停止打字
         * @param type 事件类型
         * */
        void setChatState(ChatStateType type)
        {
                if (chat_state_filter)
                        chat_state_filter->setChatState(type);
        }

        /**
         * @brief 激活某消息事件
         * @param event 事件类型
         */
        void raiseMessageEvent(MessageEventType event)
        {
                if (message_event_filter)
                        message_event_filter->raiseMessageEvent(event);
        }

        /** 返回Buddy的资源*/
        const std::string& getResource()
        {
                return m_resource;
        }

        void setResource(const std::string resource_)
        {
                m_resource = resource_ ;
                jid.setResource(m_resource);
        }

        /** 返回此用户的类型*/
        BuddyType getType()const
        {
                return type;
        }

        /** 设置此用户的类型*/
        void setType(BuddyType f_type) ;
        void setType(int f_type) ;

        /** 根据用户ID的特殊猜测用户的类型*/
        BuddyType guessType()const ;

        /** 带内数据传输地发送图片--测试*/
        void sendPicture();
        /** 带内数据传输地发送图片--filename为文件名*/
        void sendPicture(const std::string& filename);
        /** 发送文件*/
        void sendFile(const std::string& filename);
        const std::string& getCustomData()
        {
                return customSmile;
        }

        //void set_ibbstream(InBandBytestream* ibb){ m_ibb=ibb;}
        //InBandBytestream* get_ibbstream(){ return m_ibb;}
        //void ibbattachToSession(){ if(m_ibb&&session) m_ibb->attachTo(session);}
        //void cleanIBBstream();

public:

        friend class BuddyView;

private:
        Buddy(const Buddy&);
        const Buddy& operator = (const Buddy&);

protected:
        JID jid;
        std::string nickname;
        SubscriptionType subscription;
        StringList groups;
        std::string m_resource;
        VCard* vcard;
        MessageSession* session;
        MessageEventFilter* message_event_filter;
        ChatStateFilter* chat_state_filter;
        MsgPage* page;
        Presence::PresenceType status;
        std::string sign_msg;
        Glib::RefPtr<Gdk::Pixbuf> logo;
        std::string customSmile;
        BuddyType type;
        sigc::connection timer;
	MessageList m_messagelist;
};

#endif // _BUDDY_H_

