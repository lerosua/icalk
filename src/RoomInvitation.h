/*
* =====================================================================================
*
*       Filename:  RoomInvitation.h
*
*    Description:  被邀请进入聊天室
*
*        Version:  1.0
*        Created:  2008年04月25日 09时38分48秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  first_name last_name (fl), fl@my-company.com
*        Company:  my-company
*
* =====================================================================================
*/

#ifndef ROOMINVITED_H__
#define ROOMINVITED_H__

#include <iostream>
#include <gloox/mucinvitationhandler.h>
#include <gloox/client.h>

using namespace gloox;

class Bodies;

class RoomInvitation: public MUCInvitationHandler
{

public:
        RoomInvitation(Client* f_client);
        ~RoomInvitation();
        void handleMUCInvitation(const JID& room, const JID& from
                                 , const std::string& reason, const std::string& body, const std::string & password, bool cont,const std::string& thread);
        void handleMUCInvitation(const JID& room, const JID& from
			,const std::string& reason,const std::string& body
			,const std::string& pass,bool cont);

};




#endif

