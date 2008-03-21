#include <gloox/client.h>
#include "RoomItem.h"


RoomItem::RoomItem(const ConferenceListItem& ci_): m_page(NULL)
{
        m_citem.nick = ci_.nick;
        m_citem.password = ci_.password;
        m_citem.name = ci_.name;
        m_citem.jid = ci_.jid;
        m_citem.autojoin = ci_.autojoin;
        std::string fulljid = m_citem.jid + "/" + m_citem.name;

        const JID nick_(fulljid);
        Client& jclient = Bodies::Get_Bodies().get_client();
        m_mucroom = new MUCRoom(&jclient, nick_, 0, 0);

}

RoomItem::~RoomItem()
{
        delete m_mucroom;


}

void RoomItem::join()
{
        TalkRoomHandler& roomhandler = Bodies::Get_Bodies().getRoomHandler();
        m_mucroom->registerMUCRoomHandler(&roomhandler);
        m_mucroom->setNick(m_citem.nick);
        m_mucroom->join();
        m_mucroom->getRoomInfo();
        m_mucroom->getRoomItems();
}

void RoomItem::leave()
{
        m_mucroom->leave();
        m_mucroom->removeMUCRoomHandler();
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


