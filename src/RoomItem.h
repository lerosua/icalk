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

using namespace gloox;
//class Bodies;
class TalkRoomHandler;

class RoomItem{
	public:
		RoomItem(const ConferenceListItem& ci_);

		const std::string& getRoomJID() const { return citem.jid;}
		void join();
		void leave();
		MsgPage* getPage(){ return page; }
		void setPage(MsgPage* page_){ page = page_; }
		void closePage(){page=NULL;}
		void send(Glib::ustring& text){ mucroom->send(text); }
		void setMemberList(const Disco::ItemList& listitem_);
		//const StringMap& getMemberList() const { return listitem; }
		StringMap& getMemberList() { return listitem; }
		void addMember(const std::string& name , const std::string& mid);
		void removeMember(const std::string& name);
		void setSubject(const Glib::ustring& sub) { subject = sub;  }
		Glib::ustring& getSubject() { return subject; }

	private:
		MUCRoom* mucroom;
		ConferenceListItem citem;
		MsgPage* page;
		Glib::ustring subject;
		StringMap listitem;
};






#endif   /* ----- #ifndef ROOMITEM_FILE_HEADER_INC  ----- */

