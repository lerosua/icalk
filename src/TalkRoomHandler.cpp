/*
 * =====================================================================================
 *
 *       Filename:  TalkRoomHandler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年05月24日 19时43分28秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua xihels, lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <glib/gi18n.h>
#include "Bodies.h"
#include "TalkRoomHandler.h"


void TalkRoomHandler::joinRoom()
{
	ROOMLIST::const_iterator iter = roomList.begin();
	for(; iter != roomList.end();iter++)
		(*iter).second->join();


}
void TalkRoomHandler::addRoomItem(const std::string& jid, RoomItem* item)
{
	roomList.insert(roomList.end(), ROOMLIST::value_type(jid, item));
}

RoomItem* TalkRoomHandler::findRoom(const std::string& id) const
{
	ROOMLIST::const_iterator iter = roomList.find(id); 
	if ( iter == roomList.end())
		return NULL;
	return (*iter).second;
}

RoomItem* TalkRoomHandler::findRoom(const MUCRoom* room)const
{
	std::string id = room->name()+"@"+room->service();
	return findRoom(id);
}

void TalkRoomHandler::handleMUCParticipantPresence(MUCRoom * room ,
					    const MUCRoomParticipant
					    participant, Presence::PresenceType presence)
{
	RoomItem* item = findRoom(room);
	MsgPage* page = item->getPage();
	std::string name = participant.nick->resource();
	std::string mid = participant.nick->bare()+"/"+participant.nick->resource();
	
	StringMap memberlist = item->getMemberList();
	StringMap::iterator iter;
	iter = memberlist.find(name);
	if(iter == memberlist.end())
	{
		item->addMember(name,mid);
		if(NULL!=page)
		{
		std::string msg_ = participant.nick->resource()+_(" join in the room");
		//page->showSystemMsg(msg_);
		page->showStatusBarMsg(msg_);
		}
	}
	if (presence == Presence::Unavailable)
	{
		item->removeMember(name);
	if(NULL!=page)
		{
			std::string msg_ = participant.nick->resource()+_(" leave the room");
			//page->showSystemMsg(msg_);
			page->showStatusBarMsg(msg_);
		}
	}

	if(NULL!=page)
		page->refreshMember();
	/*
	if (presence == PresenceAvailable)
	{

		printf("!!!!!!!!!!!!!!!! %s is in the room, too\n",
		       participant.nick->resource().c_str());
	}
	else if (presence == Presence::Unavailable)
	{

		printf("!!!!!!!!!!!!!!!! %s left the room\n",
		       participant.nick->resource().c_str());
	}
	else
		printf("Presence::PresenceType is %d of %s\n", presence,
		       participant.nick->resource().c_str());
		       */
}

/*
void TalkRoomHandler::handleMUCMessage(MUCRoom * room  ,
				const std::string & nick,
				const std::string & message,
				bool history,
				const std::string & when  , bool priv)
{
	printf("%s said: '%s' (history: %s, private: %s)\n", nick.c_str(),
	       message.c_str(), history ? "yes" : "no",
	       priv ? "yes" : "no");
	RoomItem* item = findRoom(room);
	MsgPage* page = item->getPage();
	if(NULL==page)
	{
			const std::string  label= room->name()+"@"+room->service();
			MsgPage* page_=new MsgPage(label,item,1);
			item->setPage(page_);
			page = item->getPage();
			page->refreshMember();
			page->setSubject();
			Bodies::Get_Bodies().get_msg_window().add_page(*page);
	}
		if(priv)
		{
			Glib::ustring msg_ =_("said to you: ") +message;
			page->showMessage(nick,msg_);
			return;
		}
		if(history)
			page->showHistroy(nick,message);
		else
			page->showMessage(nick,message);
}
*/
void TalkRoomHandler::handleMUCMessage(MUCRoom* room,
		const Message& msg, bool priv)
{

	const std::string nick=msg.from().resource();
	bool history= msg.when()?true:false;
	printf("%s said: '%s' (history: %s, private: %s)\n", nick.c_str(),
			msg.body().c_str(),history ?"yes":"no",
			priv ? "yes":"no");
	RoomItem* item = findRoom(room);
	MsgPage* page = item->getPage();
	if(NULL==page)
	{
			const std::string  label= room->name()+"@"+room->service();
			MsgPage* page_=new MsgPage(label,item,1);
			item->setPage(page_);
			page = item->getPage();
			page->refreshMember();
			page->setSubject();
			Bodies::Get_Bodies().get_msg_window().add_page(*page);
	}
	if(priv)
		{
			Glib::ustring msg_ =_("said to you: ") +message;
			page->showMessage(nick,msg_);
			return;
		}
		if(history)
		{
			const DelayedDelivery* dd=msg.when();
			if(dd)
				printf("message was sent at %s\n",dd->stamp().c_str());
			page->showHistroy(nick,msg.body());
		}
		else
			page->showMessage(nick,msg.body());



}
void TalkRoomHandler::handleMUCSubject(MUCRoom * room ,
				const std::string & nick,
				const std::string & subject)
{
	RoomItem* item = findRoom(room);
	MsgPage* page = item->getPage();
	Glib::ustring msg = nick+_(" has set the Subject: ")+subject;
	item->setSubject(subject);
	if(page !=NULL)
	{
		page->setSubject();
		page->showSystemMsg(msg);
		
	}

}
void TalkRoomHandler::handleMUCError(MUCRoom * room , StanzaError error)
{
	printf(" room %s got an error: %d\n",room->name().c_str(), error);
	room->leave();
}

void TalkRoomHandler::handleMUCInfo(MUCRoom * room , int features,
			     const std::string & name,
			     const DataForm * infoForm)
{
	//printf("features: %d, name: %s, form xml: %s\n", features,
	//      name.c_str(), infoForm->tag()->xml().c_str());
}
void TalkRoomHandler::handleMUCItems(MUCRoom *  room  ,
			      const Disco::ItemList & items)
{
	RoomItem* item = findRoom(room);
	item->setMemberList(items);

	/*
	StringMap::const_iterator it = items.begin();
	for (; it != items.end(); ++it) {
		printf("==================%s -- %s is an item here\n", (*it).first.c_str(),
		       (*it).second.c_str());
	}
	*/
}



void TalkRoomHandler::handleMUCInviteDecline(MUCRoom * room ,
				      const JID & invitee,
				      const std::string & reason)
{
	printf("Invitee %s declined invitation. reason given: %s\n",
	       invitee.full().c_str(), reason.c_str());
}
bool TalkRoomHandler::handleMUCRoomCreation(MUCRoom * room)
{
	printf("room %s didn't exist, beeing created.\n",
	       room->name().c_str());
	return true;
}
