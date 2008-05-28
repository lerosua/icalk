/*
* =====================================================================================
*
*       Filename:  BuddyList.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2007年06月18日 星期一 10时28分37秒
*       Revision:  none
*       Compiler:  g++
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/
#include <glib/gi18n.h>
#include <gtkmm/dialog.h>
#include "BuddyList.h"
#include "Buddy.h"
#include "Bodies.h"
#include "TalkCard.h"
#include "icalk.h"

BuddyList::BuddyList()
{
        buddy_msg.clear();

}

Buddy* BuddyList::find_buddy(const Glib::ustring& id) const
{
        BUDDY_MAP::const_iterator iter = buddy_map.find(id);

        if ( iter == buddy_map.end())
                return NULL;

        return (*iter).second;

}

BuddyList::~BuddyList()
{
        BUDDY_MAP::iterator iter = buddy_map.begin();

        for (; iter != buddy_map.end();iter++) {
                //delete iter->second;
                buddy_map.erase(iter);
        }

        BUDDY_NEW_MSG::iterator iter2 = buddy_msg.begin();

        for (; iter2 != buddy_msg.end();iter2++) {
                //delete iter->second;
                buddy_msg.erase(iter2);
        }
}

void BuddyList::handleItemSubscribed(const JID & jid)
{
        DLOG( "subscribed %s\n", jid.bare().c_str() );
}

void BuddyList::handleItemAdded(const JID & jid)
{
        //std::string jid_str = jid.bare().c_str();
        const std::string& jid_str = jid.bare();

        if ( NULL == find_buddy(jid_str) ) {
                /** 将新的好友加入到BuddyMap里。*/
                RosterItem* item = Bodies::Get_Bodies().get_client().rosterManager()->getRosterItem(jid_str);
                buddy_map.insert(buddy_map.end(), BUDDY_MAP::value_type(item->jid(), new Buddy(*item)));
                Bodies::Get_Bodies().getCardManage().fetch_vcard(jid_str);
                Bodies::Get_Bodies().get_main_window().get_buddy_view().add(jid_str);
        }
}

void BuddyList::handleItemUnsubscribed(const JID & jid)
{
        DLOG( "ItemUnsubscribed  %s\n", jid.bare().c_str() );
}

void BuddyList::handleItemRemoved(const JID & jid)
{
        DLOG( "ItemRemoved   %s\n", jid.bare().c_str() );
        const std::string& jid_str = jid.bare();
        Bodies::Get_Bodies().get_main_window().get_buddy_view().remove(jid_str);
        BUDDY_MAP::iterator iter = buddy_map.find(jid_str);

        if ( iter != buddy_map.end() ) {
                delete (*iter).second;
                buddy_map.erase(iter);
        }

}

void BuddyList::handleItemUpdated(const JID & jid)
{
        DLOG( "ItemUnpdated %s\n", jid.bare().c_str() );
}

void BuddyList::handleRoster(const Roster & roster)
{
        Roster::const_iterator it = roster.begin();

        for (; it != roster.end(); ++it) {

                const RosterItem* item = (*it).second;
                buddy_map.insert(buddy_map.end(), BUDDY_MAP::value_type(item->jid(), new Buddy(*item)));

        }

}

void BuddyList::handleRosterPresence(const RosterItem & item,
                                     const std::string & resource,
                                     Presence::PresenceType presence,
                                     const std::string & msg)
{
        Glib::ustring jid(item.jid());
        Buddy* buddy = Bodies::Get_Bodies().get_buddy_list().find_buddy(jid);

        if (Presence::Unavailable == presence) {
                if (buddy->get_status() == Presence::Unavailable)
                        return ;
                else
                        buddy->set_sign_msg(_("offline"));
        } else {
                buddy->set_sign_msg(msg);
                Bodies::Get_Bodies().getCardManage().fetch_vcard(item.jid());
        }

        buddy->set_status(presence);

        buddy->setResource(resource);
        Bodies::Get_Bodies().get_main_window().get_buddy_view().refreshBuddyStatus(jid);

}

void BuddyList::handleSelfPresence(const RosterItem & item,
                                   const std::string & resource,
                                   Presence::PresenceType presence,
                                   const std::string & msg )
{
        DLOG( "self presence received: %s/%s -- %d\n", item.jid().c_str(), resource.c_str(), presence );

}

bool BuddyList::handleSubscriptionRequest(const JID & jid,
                const std::string& msg )
{
        DLOG(" %s subscriptionRequest with %s\n", jid.bare().c_str(), msg.c_str());
        Gtk::MessageDialog dialog(_("Information from stranger"), false /*use markup*/, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
        Glib::ustring msg_text = jid.bare() + _("Ask for a friend , press OK to add");
        dialog.set_secondary_text(msg_text);
        int result = dialog.run();

        switch (result) {
        case(Gtk::RESPONSE_OK): {
                        //对方请求加为好友。
                        StringList groups;
                        JID id(jid);
                        Bodies::Get_Bodies().get_client().rosterManager()->subscribe( id , "", groups, "");
                        return true;
                        break;
                }

        case(Gtk::RESPONSE_CANCEL): {
                        return false;
                        break;
                }

        default: {
                        return false;
                        break;
                }
        }

        return false;
}

bool BuddyList::handleUnsubscriptionRequest(const JID & jid,
                const std::string& msg )
{
        DLOG(" %s UnsubscriptionRequest with %s\n", jid.bare().c_str(), msg.c_str());
        Gtk::MessageDialog dialog(_("The message from friend"));
        Glib::ustring msg_text = jid.bare() + _("Delete you from his buddy list");
        dialog.set_secondary_text(msg_text);
        dialog.run();
        return true;
}

void BuddyList::handleNonrosterPresence(const Presence& stanza)
{
        DLOG("接收到状态并不在列表中的人 %s \n", stanza.from().full().c_str());
}

void BuddyList::handleNonrosterPresence(Presence * stanza)
{
        DLOG("接收到状态并不在列表中的人 %s \n", stanza->from().full().c_str());
}


void BuddyList::handleRosterError(const IQ& iq)
{

        std::cout << "handleRosterError " << iq.tag()->xml() << std::endl;

}

void BuddyList::addNewMsgBuddy(const Glib::ustring& f_jid)
{

        BUDDY_NEW_MSG::iterator result = std::find(buddy_msg.begin(), buddy_msg.end(), f_jid);

        if (result == buddy_msg.end()) {
                buddy_msg.push_back(f_jid);
                Bodies::Get_Bodies().get_main_window().get_buddy_view().newMsgBuddy(f_jid, true);

        }
}

void BuddyList::delNewMsgBuddy(const Glib::ustring& f_jid)
{
        BUDDY_NEW_MSG::iterator result = std::find(buddy_msg.begin(), buddy_msg.end(), f_jid);

        if (result != buddy_msg.end()) {
                buddy_msg.erase(result);
                Bodies::Get_Bodies().get_main_window().get_buddy_view().newMsgBuddy(f_jid, false);
        }

}

bool BuddyList::popNewMsgBuddy()
{
        if (buddy_msg.empty())
                return false;

        do {
                BUDDY_NEW_MSG::iterator iter = buddy_msg.begin();

                if (iter == buddy_msg.end())
                        break;

                const Glib::ustring& f_jid = (*iter);

                Buddy* f_buddy = find_buddy(f_jid);

                if (NULL != f_buddy) {
                        f_buddy->new_page();
                }
        } while (true);

        buddy_msg.clear();

        return true;
}
