/*
* =====================================================================================
*
*       Filename:  RoomItem.h
*
*    Description:
*
*        Version:  1.0
*        Created:  2007年08月22日 19时20分08秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  ROOMITEM_FILE_HEADER_INC
#define  ROOMITEM_FILE_HEADER_INC

#include <gloox/mucroom.h>
#include <gloox/bookmarkhandler.h>
#include "MsgPage.h"
#include "Bodies.h"
#include "icalk.h"

using namespace gloox;


class TalkRoomHandler;

/**
 * 聊天室类，一个RoomItem对应一个聊天室，聊天室包含聊天会话，成员列表等。
 */

class RoomItem
{

public:
        RoomItem(const ConferenceListItem& ci_);
        ~RoomItem();

        /** 得到聊天室的JID号 */
        const std::string& getRoomJID() const
        {
                return m_citem.jid;
        }

        /** 进入聊天室 */
        void join();
        /** 离开聊天室 */
        void leave();
        /** 邀请某人进聊天室*/
        void invite(const JID& invitee, const std::string& reason);
        MsgPage* getPage()
        {
                return m_page;
        }

        void setPage(MsgPage* f_page)
        {
                m_page = f_page;
        }

        /** 关闭页面 */
        void closePage()
        {
                m_page = NULL;
        }

        /** 发送信息 */
        void send(Glib::ustring& text)
        {
                m_mucroom->send(text);
        }

        /** 初始化聊天室成员列表 */
        void setMemberList(const Disco::ItemList& m_list_);
        /** 获取聊天室成员列表 */
        MemberMap& getMemberList()
        {
                return m_list;
        }

        /** 添加成员进聊天室列表 */
        void addMember(const std::string& name , const Member& member);
        /** 从聊天室成员列表里删除名为name的成员 */
        void removeMember(const std::string& name);
        /** 设置聊天室主题 */
        void setSubject(const Glib::ustring& sub)
        {
                m_subject = sub;
        }

        /** 获取聊天室主题 */
        Glib::ustring& getSubject()
        {
                return m_subject;
        }

private:
        MUCRoom* m_mucroom;
        ConferenceListItem m_citem;
        MsgPage* m_page;
        Glib::ustring m_subject;
        MemberMap m_list;
};






#endif   /* ----- #ifndef ROOMITEM_FILE_HEADER_INC  ----- */

