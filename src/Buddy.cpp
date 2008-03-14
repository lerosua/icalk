/*
* =====================================================================================
*
*       Filename:  Buddy.cpp
*
*    Description:  好友的数据结构
*
*        Version:  1.0
*        Created:  2007年06月24日 22时27分15秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:  
*
* =====================================================================================
*/

#include <iostream>
#include <fstream>
#include <string>
#include <glib/gi18n.h>
#include "Buddy.h"
#include "TalkMsg.h"
#include "Bodies.h"

/** 带内数据传输图片的大小*/
#define PICZIAE 102400 
//#define PICZIAE 40960

Buddy::Buddy(const RosterItem& item):
                jid(item.jid()),
                nickname(item.name()),
                subscription(item.subscription()),
                groups(item.groups()),
                customSmile("Picture"),
                vcard(NULL),
                session(NULL),
                message_event_filter(NULL),
                chat_state_filter(NULL),
                page(NULL)
{
        status = Presence::Unavailable;
        type = TYPE_FRIEND;
        logo = Gdk::Pixbuf::create_from_file(DATA_DIR"/images/default.png");

        /*
        std::cout 
         << "username = " << jid.username() << std::endl
         << "server = " << jid.server() << std::endl
         << "resource = " << jid.resource() << std::endl
         << "serverRaw = " << jid.serverRaw() << std::endl
         << "full = "<< jid.full() << std::endl
         << "subscription = " << subscription << std::endl;
         */

}

Buddy::~Buddy()
{
        if (NULL != vcard)
                delete vcard;

        delete session;

        delete message_event_filter;

        delete chat_state_filter;

        if (NULL != page)
                delete page;

        page = NULL;

        vcard = NULL;
}


void Buddy::set_nickname(const std::string& name_)
{
        nickname = name_ ;
        const JID& id = this->get_jid();
        /*还需要设置相应rosteritem的set_name()*/
        Bodies::Get_Bodies().get_client().rosterManager()->getRosterItem(id)->setName(name_);
}

void Buddy::set_vcard(const VCard* vcard_)
{
        if (NULL != vcard)
                delete vcard;

        vcard = new VCard(*vcard_);
}

void Buddy::refreshVcard()
{
        Bodies::Get_Bodies().get_cardManage().fetch_vcard(jid.bare());
}

void Buddy::refreshinfo()
{
        Bodies::Get_Bodies().get_main_window().get_buddy_view().refreshBuddyStatus(jid.bare());
}

BuddyType Buddy::guessType()const
{
        /**用户名为空，则可能为transport*/

        if (jid.username().empty())
                return TYPE_TRANPORT;

        const std::string& server = jid.server();

        if (server.empty())
        {
                printf(" %s 's server empty\n", jid.username().c_str());
        }

        /** 服务器开头为”msn.“ 则可能为MSN用户*/
        if (0 == server.find("msn."))
                return TYPE_MSN;

        if (0 == server.find("icq."))
                return TYPE_ICQ;

        if (0 == server.find("yahoo."))
                return TYPE_YAHOO;

        /** 资源开头为conference，则可能为Gtalk群机器人*/
        if (0 == m_resource.find("conference"))
                return TYPE_GROUPCHAT;

        return TYPE_FRIEND;

}

void Buddy::new_session()
{
        if (NULL == session)
        {
                Client& jclient_ = Bodies::Get_Bodies().get_client();
                session = new MessageSession(&jclient_, jid);
                TalkMsg& handler_ = Bodies::Get_Bodies().get_talkmsg();
                set_session(session, &handler_);
        }
}

void Buddy::set_session(MessageSession* session_, TalkMsg* handler)
{
        if (session != session_)
        {
                close_session(false);
                session = session_;
        }

        if (NULL != session)
        {
                session->registerMessageHandler(handler);

                message_event_filter = new MessageEventFilter(session);
                message_event_filter->registerMessageEventHandler(handler);

                chat_state_filter = new ChatStateFilter(session);
                chat_state_filter->registerChatStateHandler(handler);

                /* 这里还需要生成标签页*/

                if (NULL == page)
                {
                        if (!nickname.empty())
                                page = new MsgPage(nickname, this);
                        else
                        {
                                const char* label = session->target().bare().c_str();
                                page = new MsgPage(label, this);
                        }
                }
        }

}

void Buddy::close_session(bool closePage)
{
        if (NULL != session)
        {
                /**发出关闭对话框的信号给对方*/

                if (closePage)
                {
                        this->setChatState( ChatStateGone );
                        this->raiseMessageEvent(MessageEventCancel);
                }

                /** 关闭会话 */
                session->disposeMessageFilter(message_event_filter);

                session->disposeMessageFilter(chat_state_filter);

                //printf(" 真正地disposeMessagesession\n");
                Bodies::Get_Bodies().get_client().disposeMessageSession(session);

                if (NULL != session)
                {
                        session = NULL;
                }

        }

        /** 关闭标签页 */
        if (closePage && (NULL != page))
        {
                page = NULL;
        }
}

void Buddy::sendPicture()
{
        std::string id = jid.full() + "/" + getResource();
        JID jid_(id);
}

void Buddy::sendPicture(const std::string& filename)
{
        /**加载图片进数据*/

        Bodies::Get_Bodies().getFThandler().handleFTSend(jid, filename);
        /*
           std::ifstream fin(filename.c_str(),std::ios::binary);
           fin.seekg(0,std::ios::end);
           int filesize=fin.tellg();
           fin.seekg(0,std::ios::beg);
           if(filesize<PICZIAE)
           {
         std::copy((std::istreambuf_iterator<char>(fin)),
           std::istreambuf_iterator<char>(),
           std::inserter(customSmile,customSmile.begin()));
         page->showPicture(filename.c_str(),true);
           }
           else
           {
            printf("图片大小超过40K\n");
            //std::string msg="图片大小超过100K,默认将不发送";
            std::string msg=_("picture size must be under 100k bytes");
            Gtk::MessageDialog dialog("alarm",false,
              Gtk::MESSAGE_INFO);
            dialog.set_secondary_text(msg);
            dialog.run();
            return;
           }
           fin.close();

        */
}

