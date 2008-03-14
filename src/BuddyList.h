/*
* =====================================================================================
* 
*       Filename:  BuddyList.h
* 
*    Description:  好友列表
* 
*        Version:  1.0
*        Created:  2007年06月14日 22时42分09秒 CST
*       Revision:  none
*       Compiler:  gcc
* 
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
* 
* =====================================================================================
*/

#ifndef  BUDDYLIST_FILE_HEADER_INC
#define  BUDDYLIST_FILE_HEADER_INC

#include <iostream>
#include <gloox/rostermanager.h>
#include <map>

using namespace gloox;

class Buddy;
/**
 * @brief 用于得到好友，处理好友状态信息的类。
 *   继承自@link RosterListener @endlink 类。
 *   与@ref BuddyView 类相关，处理@link BuddyView BuddyView@endlink类内部事件
 *
 */

class BuddyList: public RosterListener
{

public:
        BuddyList()
        {}

        /** 保存好友数据的结构，以好友id为关键字*/
        typedef std::map<Glib::ustring, Buddy*> BUDDY_MAP;

        ~BuddyList()
        {}

        /** 获取好友数据，好友都保存在此MAP中*/
        const BUDDY_MAP& get_buddy_map() const
        {
                return buddy_map;
        }

        /**
         * @brief 查找好友列表中的好友.
         * @note 不要 delete 返回的指针.
         * @param id 好友的名字.
         * @return 好友的Buddy类指针.
         */
        Buddy* find_buddy(const Glib::ustring& id) const;

        /** @brief 遍历好友列表
         * @param fun 一个函数对象.实现 operator()(BUDDY_MAP::value_type).
         */

        template <class Tfun>
        void for_each(Tfun fun) const
        {
                std::for_each(buddy_map.begin(), buddy_map.end(), fun);
        }

protected:
        /**
         * @brief 服务器返回错误时调用
         * @param stanza 关于错误的Stanza
         */
        void handleRosterError(const IQ& iq);
        /** 应该是某用户同意添加好友*/
        void handleItemSubscribed(const JID& jid);
        /** 用户已添加*/
        void handleItemAdded(const JID& jid);
        /** 用户被退订*/
        void handleItemUnsubscribed(const JID& jid);
        /** 用户被删除*/
        void handleItemRemoved(const JID& jid);
        /** 用户更新*/
        void handleItemUpdated(const JID& jid);
        /** 处理花名册*/
        void handleRoster(const Roster& roster);
        /**
         * @brief 接收到在线好友的状态及签名
         * @param item 原始的好友类
         * @param resource 好友的资源
         * @param presence 好友的状态
         * @b Presence::PresenceType:
         * @li Available   =0      在线并可见
         * @li Chat =1 唠叨
         * @li Away =2 暂时离开
         * @li DND  =3 请勿打挠(dnd="Do Not Disturb")
         * @li XA  =4 远远离开
         * @li Unavailable =5 离线
         * @li Probe =6      This is a presence probe.
         * @li Error =7 This is a presence error.
         * @li Invalid =8 the stanza is invalid.
         * 
         * @param msg 签名消息
         */
        void handleRosterPresence(const RosterItem& item, const std::string& resource,
                                  Presence::PresenceType presence, const std::string& msg );
        void handleSelfPresence(const RosterItem& item, const std::string& resource,
                                Presence::PresenceType presence, const std::string& msg );
        /**
         * @brief 对方要求添加为好友的请求
         * @param jid 对方jid
         * @param msg 添加请求时一起发来的信息
         * @return 返回真则同意，返回假则忽略请求
         */
        bool handleSubscriptionRequest(const JID& jid, const std::string& msg);
        /**
         * @brief 对方删除偶
         * @param jid 对方jid
         * @param msg 发送删除通知时一起发来的信息
         * @return 返回真则删除，返回假则忽略请求
         */
        bool handleUnsubscriptionRequest(const JID& jid, const std::string& msg);
        /** 在 gloox 1.0-beta2后，这个函数被废除*/
        void handleNonrosterPresence(Presence* stanza);
        /** 当一个不在好友列表中的人发来Presence::PresenceType信息时回调,奇怪~*/
        void handleNonrosterPresence(const Presence& stanza);

private:
        BUDDY_MAP buddy_map;
};

#endif   /* ----- #ifndef BUDDYLIST_FILE_HEADER_INC  ----- */

