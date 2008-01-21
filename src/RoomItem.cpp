#include <gloox/client.h>
#include "RoomItem.h"


RoomItem::RoomItem(const ConferenceListItem& ci_):page(NULL)
{
	citem.nick = ci_.nick;
	citem.password = ci_.password;
	citem.name = ci_.name;
	citem.jid = ci_.jid;
	citem.autojoin = ci_.autojoin;
	std::string fulljid= citem.jid+"/"+citem.name;

	const JID nick_(fulljid);
	std::cout<<"room jid is "<<citem.jid<<std::endl;
	Client& jclient=Bodies::Get_Bodies().get_client();
	//TalkRoomHandler& roomhandler=Bodies::Get_Bodies().getRoomHandler();
	mucroom = new MUCRoom(&jclient,nick_,0,0);
	//mucroom = new MUCRoom(&jclient,nick_,&roomhandler,0);

	//mucroom->registerMUCRoomHandler(&roomhandler);
}

void RoomItem::join()
{
	TalkRoomHandler& roomhandler=Bodies::Get_Bodies().getRoomHandler();
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

/*
void setSubject(const Glib::ustring& sub) 
{
	subject = sub;
	if(NULL!=page)
		page->setSubject(subject);
}
*/
void RoomItem::setMemberList(const Disco::ItemList& listitem_)
{
	//listitem = listitem_;
	std::string name;
	std::string mid;
	Disco::ItemList::const_iterator iter=listitem_.begin();
	for(;iter != listitem_.end();iter++)
	{
		name=(*iter)->name();
		mid=(*iter)->jid().full();
		listitem.insert(pair<std::string,std::string>(name,mid));
	}
}
void RoomItem::addMember(const std::string& name , const std::string& mid)
{
	
		listitem.insert(pair<std::string,std::string>(name,mid));

}

void RoomItem::removeMember(const std::string& name)
{

	StringMap::iterator iter;
	iter = listitem.find(name);
	listitem.erase(iter);

}
