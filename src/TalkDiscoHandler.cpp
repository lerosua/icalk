/*
* =====================================================================================
*
*       Filename:  TalkDiscoHandler.cpp
*
*    Description:  server discovery handler class
*
*        Version:  1.0
*        Created:  2007年09月30日 18时49分34秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#include "TalkDiscoHandler.h"
#include "Bodies.h"
#include "ServerDiscoWindow.h"
#include "icalk.h"


void TalkDiscoHandler::handleDiscoInfoResult(IQ * stanza, int context)
{
        DLOG("handleDiscoInfoResult}\n");
}

void TalkDiscoHandler::handleDiscoItemsResult(IQ * stanza, int context)
{
        DLOG("handleDiscoItemsResult\n");
}

void TalkDiscoHandler::handleDiscoError(IQ * stanza, int context)
{
        DLOG("handleDiscoError\n");
}


void TalkDiscoHandler::handleDiscoInfo(const JID& from, const Disco::Info& info, int context)
{
        DLOG("handleDiscoInfo}\n");
}

void TalkDiscoHandler::handleDiscoItems(const JID& from, const Disco::Items& items, int context)
{
        DLOG("handleDiscoItems}\n");
        const Disco::ItemList& list = items.items();
        Disco::ItemList::const_iterator iter = list.begin();

        ServerDiscoWindow* discowindow = Bodies::Get_Bodies().get_main_window().get_disco_window();

        if (NULL == discowindow)
                return ;

        for (; iter != list.end(); ++iter) {
                PBUG("jid = %s\n", (*iter)->jid().full().c_str());
                discowindow->addAgent((*iter)->jid().full());

        }
	discowindow->progress(false);
}

void TalkDiscoHandler::handleDiscoError(const JID& from, const Error* error, int context)
{}

bool TalkDiscoHandler::handleDiscoSet(IQ* iq)
{}
