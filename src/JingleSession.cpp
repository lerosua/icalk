/*
* =====================================================================================
*
*       Filename:  JingleSession.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2008年01月24日 10时50分01秒
*       Revision:  none
*       Compiler:  g++
*
*         Author:  lerosua@gmail.com
*        Company:  cyclone
*
* =====================================================================================
*/

#include "JingleSession.h"

namespace jingle
{

JingleSession::JingleSession(gloox::Client* client_, const JidList& t_peers)
                : peers(t_peers), client(client_)
{
        myself = client->jid();
}

JingleSession::~JingleSession()
{}

JingleSession::JidList& JingleSession::peers()
{
        return peers;
}





}
