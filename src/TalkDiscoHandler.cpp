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


void TalkDiscoHandler::handleDiscoInfoResult(Stanza * stanza, int context)
{
	printf("handleDiscoInfoResult}\n");
	//std::cout<<stanza->xml()<<std::endl;
}
void TalkDiscoHandler::handleDiscoItemsResult(Stanza * stanza, int context)
{
	printf("handleDiscoItemsResult\n");
}
void TalkDiscoHandler::handleDiscoError(Stanza * stanza, int context)
{
	printf("handleDiscoError\n");
}
