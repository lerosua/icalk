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
#include "TalkMsg.h"
#include "Bodies.h"
#include "Buddy.h"

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

}

Buddy::~Buddy()
{
        if (NULL != vcard) {
                delete vcard;
                vcard = NULL;
        }

        if (NULL != session) {
                delete session;
                session = NULL;
        }

        if (NULL != message_event_filter) {
                delete message_event_filter;
                message_event_filter = NULL;
        }

        if (NULL != chat_state_filter) {
                delete chat_state_filter;
                chat_state_filter = NULL;
        }

        if (NULL != page) {
                delete page;
                page = NULL;
        }
}
#if 0
MsgPage* Buddy::get_page()
        {
                if (NULL != page)
                        return page;

                const std::string label = jid.bare();

                return new MsgPage(label, this);
        }
#else
MsgPage* Buddy::get_page()
{
	return page;
}
#endif
MsgPage* Buddy::new_page()
{
	if (NULL == page) {
                        if (!nickname.empty())
                                page = new MsgPage(nickname, this);
                        else {
                                page = new MsgPage(jid.bare(), this);
                        }
                }
	return page;
}

void Buddy::set_jid(const JID& f_jid)
{
        jid.setJID(f_jid.full());
}

void Buddy::setType(int f_type)
{
        switch (f_type) {

        case 0:
                setType(TYPE_FRIEND);
                break;

        case 1:
                setType(TYPE_TRANPORT);
                break;

        case 2:
                setType(TYPE_GROUPCHAT);
                break;

        case 3:
                setType(TYPE_BOT);
                break;

        case 4:
                setType(TYPE_MSN);
                break;

        case 5:
                setType(TYPE_ICQ);
                break;

        case 6:
                setType(TYPE_YAHOO);
                break;

        case 7:
                setType(TYPE_OTHER);
                break;

defalut:
                break;
        }
}

void Buddy::setType(BuddyType f_type)
{
        if (f_type >= 0 && f_type < 9)
                type = f_type ;
        else
                type = TYPE_FRIEND;
}

void Buddy::set_nickname(const std::string& name_)
{
        nickname = name_ ;
        const JID& id = this->get_jid();
        /*还需要设置相应rosteritem的set_name()*/
        Bodies::Get_Bodies().get_client().rosterManager()->getRosterItem(id)->setName(name_);
}

void Buddy::set_vcard(const VCard* f_vcard)
{
        if (NULL != vcard)
                delete vcard;

        vcard = new VCard(*f_vcard);
}

void Buddy::refreshVcard()
{
        Bodies::Get_Bodies().getCardManage().fetch_vcard(jid.bare());
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

        if (server.empty()) {
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

        if (0 == m_resource.find("bot"))
                return TYPE_BOT;

        return TYPE_FRIEND;

}

void Buddy::new_session()
{
        if (NULL == session) {
                Client& jclient_ = Bodies::Get_Bodies().get_client();
                session = new MessageSession(&jclient_, jid);
                TalkMsg& handler_ = Bodies::Get_Bodies().getTalkMsg();
                set_session(session, &handler_);
        }
	else{
			Glib::ustring sender;
			if(nickname.empty())
				sender = jid.bare();
			else
				sender = nickname;

		MessageList::iterator iter = m_messagelist.begin();
		for( ; iter !=m_messagelist.end();iter++)
		{
			std::string f_msg = (*iter);
			page->showMessage(sender,f_msg);
			Bodies::Get_Bodies().promptMsg(false);
		}
		m_messagelist.clear();

	}
}
void Buddy::storeMessage(const Message& f_message)
{

	m_messagelist.push_back(f_message.body());

}

void Buddy::set_session(MessageSession* f_session, TalkMsg* handler)
{
        if (session != f_session) {
                close_session(false);
                session = f_session;
        }

        if (NULL != session) {
                session->registerMessageHandler(handler);

                message_event_filter = new MessageEventFilter(session);
                message_event_filter->registerMessageEventHandler(handler);

                chat_state_filter = new ChatStateFilter(session);
                chat_state_filter->registerChatStateHandler(handler);

                /* 这里还需要生成标签页*/

#if 0
                if (NULL == page) {
                        if (!nickname.empty())
                                page = new MsgPage(nickname, this);
                        else {
                                const char* label = session->target().bare().c_str();
                                page = new MsgPage(label, this);
                        }
                }
#endif
        }

}

void Buddy::close_session(bool closePage)
{
        if (NULL != session) {
                /**发出关闭对话框的信号给对方*/

                if (closePage) {
                        this->setChatState( ChatStateGone );
                        this->raiseMessageEvent(MessageEventCancel);
                }

                /** 关闭会话 */
                session->disposeMessageFilter(message_event_filter);

                session->disposeMessageFilter(chat_state_filter);

                message_event_filter = NULL;

                chat_state_filter = NULL;

                //printf(" 真正地disposeMessagesession\n");
                Bodies::Get_Bodies().get_client().disposeMessageSession(session);

                if (NULL != session) {
                        session = NULL;
                }

        }

        /** 关闭标签页 */
        if (closePage && (NULL != page)) {
                page = NULL;
        }
}

void Buddy::sendPicture()
{
        //std::string id = jid.full() + "/" + getResource();
        //JID f_jid(id);
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

void Buddy::sendFile(const std::string& filename)
{
        Bodies::Get_Bodies().getFThandler().handleFTSend(jid, filename);
}
