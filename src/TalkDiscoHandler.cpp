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


void TalkDiscoHandler::handleDiscoInfoResult(IQ * stanza, int context)
{
	printf("handleDiscoInfoResult}\n");
}
void TalkDiscoHandler::handleDiscoItemsResult(IQ * stanza, int context)
{
	printf("handleDiscoItemsResult\n");
}
void TalkDiscoHandler::handleDiscoError(IQ * stanza, int context)
{
	printf("handleDiscoError\n");
}


	void TalkDiscoHandler::handleDiscoInfo(const JID& from, const Disco::Info& info, int context)
{
	printf("handleDiscoInfo}\n");
}
	void TalkDiscoHandler::handleDiscoItems(const JID& from, const Disco::Items& items, int context)
{
	printf("handleDiscoItems}\n");
}
	void TalkDiscoHandler::handleDiscoError(const JID& from, const Error* error, int context)
{
}
	bool TalkDiscoHandler::handleDiscoSet(IQ* iq)
{
}
