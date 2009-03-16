/*
 * =====================================================================================
 * 
 *       Filename:  BotCalk.h
 * 
 *    Description:  icalk聊天室机器人
 * 
 *        Version:  1.0
 *        Created:  2009年03月13日 20时22分51秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  BOTCALK_FILE_HEADER_INC
#define  BOTCALK_FILE_HEADER_INC

#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/mucroomhandler.h>
#include <gloox/mucroom.h>
#include <gloox/disco.h>
#include <gloox/presence.h>
#include <gloox/message.h>
#include <gloox/dataform.h>
#include <gloox/gloox.h>
#include <gloox/lastactivity.h>
#include <gloox/loghandler.h>
#include <gloox/logsink.h>
using namespace gloox;

#include <stdio.h>
#include <locale.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include <windows.h>
#endif


#define FIFO_MSG "/tmp/bot_fifo_msg"
#define FIFO_SEND "/tmp/bot_fifo_send"
#define PERMS 0666


class BotCalk : public ConnectionListener, LogHandler, MUCRoomHandler
{
  public:
	    BotCalk(); 
	    virtual ~BotCalk();
	    void start(const std::string& jid,const std::string& pwd,const std::string& server,int port=5222);
	    void onConnect();
	    void onDisconnect( ConnectionError e );
	    bool onTLSConnect( const CertInfo& info );
	    void handleLog( LogLevel level, LogArea area, const std::string& message );
	    virtual void handleMUCParticipantPresence( MUCRoom * /*room*/,
					    const MUCRoomParticipant participant,
					    Presence::PresenceType presence );

	    virtual void handleMUCParticipantPresence( MUCRoom * /*room*/, 
				    const MUCRoomParticipant participant,
				    const Presence& presence );
	    virtual void handleMUCMessage( MUCRoom* /*room*/, const Message& msg, bool priv );

	    virtual void handleMUCSubject( MUCRoom * /*room*/, const std::string& nick, const std::string& subject );
	    virtual void handleMUCError( MUCRoom * /*room*/, StanzaError error );
	    virtual void handleMUCInfo( MUCRoom * /*room*/, int features, const std::string& name,
                                    const DataForm* infoForm );
	    virtual void handleMUCItems( MUCRoom * /*room*/, const Disco::ItemList& items );
	    virtual void handleMUCInviteDecline( MUCRoom * /*room*/, const JID& invitee, const std::string& reason );
	    virtual bool handleMUCRoomCreation( MUCRoom *room );



  private:
	    Client *m_client;
	    MUCRoom *m_room;
	    int put_msg;
	    int send_msg;
};
#endif   /* ----- #ifndef BOTCALK_FILE_HEADER_INC  ----- */

