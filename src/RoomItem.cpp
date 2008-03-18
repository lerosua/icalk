#include <gloox/client.h>
#include "RoomItem.h"


RoomItem::RoomItem(const ConferenceListItem& ci_): page(NULL)
{
        citem.nick = ci_.nick;
        citem.password = ci_.password;
        citem.name = ci_.name;
        citem.jid = ci_.jid;
        citem.autojoin = ci_.autojoin;
        std::string fulljid = citem.jid + "/" + citem.name;

        const JID nick_(fulljid);
        Client& jclient = Bodies::Get_Bodies().get_client();
        mucroom = new MUCRoom(&jclient, nick_, 0, 0);

}

RoomItem::~RoomItem()
{
        delete mucroom;


}

void RoomItem::join()
{
        TalkRoomHandler& roomhandler = Bodies::Get_Bodies().getRoomHandler();
        mucroom->registerMUCRoomHandler(&roomhandler);
        mucroom->setNick(citem.nick);
        mucroom->join();
        mucroom->getRoomInfo();
        mucroom->getRoomItems();
}

void RoomItem::leave()
{
        mucroom->leave();
        mucroom->removeMUCRoomHandler();
}


void RoomItem::setMemberList(const Disco::ItemList& m_list_)
{
        std::string name;
        std::string mid;
        Disco::ItemList::const_iterator iter = m_list_.begin();

        for (;iter != m_list_.end();iter++) {
                name = (*iter)->name();
                Member member_;
                member_.id = (*iter)->jid().full();
                member_.affiliation = AffiliationNone;
                member_.role = RoleVisitor;
                member_.flags = 0;
                member_.status = "";
                member_.presence = Presence::Available;

                m_list.insert(pair<std::string, Member>(name, member_));
        }
}

void RoomItem::addMember(const std::string& name , const Member& member)
{

        m_list.insert(pair<std::string, Member>(name, member));

}

void RoomItem::removeMember(const std::string& name)
{

        MemberMap::iterator iter;
        iter = m_list.find(name);
        m_list.erase(iter);

}


