/*
 * =====================================================================================
 *
 *       Filename:  BotCalk.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年03月13日 20时31分30秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include "BotCalk.h"
#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>



BotCalk::BotCalk():m_client(NULL), m_room(NULL)
{
	/*
	if((put_msg = open(FIFO_MSG,O_WRONLY))<0){
		printf("BotCalk:: can't open write fifo:%s\n",FIFO_MSG);
		exit(1);
	}
	if((send_msg=open(FIFO_SEND,O_RDONLY))<1){
		printf("BotCalk:: can't open read fifo:%s\n",FIFO_SEND);
		exit(1);
	}
	*/

}

BotCalk::~BotCalk()
{
//	close(put_msg);
//	close(send_msg);
//	remove(FIFO_MSG);
//	remove(FIFO_SEND);

	// cleanup
	delete m_room;
	delete m_client;
}

void BotCalk::start(const std::string & f_jid, const std::string & pwd,
		    const std::string & server, int port)
{
	JID jid(f_jid);
	m_client = new Client(jid, pwd);
	m_client->registerConnectionListener(this);
	m_client->setPresence(Presence::Available, -1);
	m_client->disco()->setVersion("BotCalk 0.10", GLOOX_VERSION,
				      "Linux");
	m_client->disco()->setIdentity("client", "bot");
	m_client->setCompression(false);
	m_client->setServer(server);
	m_client->setPort(port);
	//StringList ca;
	//ca.push_back( "/path/to/cacert.crt" );
	//m_client->setCACerts( ca );

	m_client->logInstance().registerLogHandler(LogLevelDebug,
						   LogAreaAll, this);

	JID nick("linuxcn@conference.jabber.org/BotCalk");
	m_room = new MUCRoom(m_client, nick, this, 0);

	if (m_client->connect(false)) {
		ConnectionError ce = ConnNoError;
		while (ce == ConnNoError) {
			ce = m_client->recv();
		}
		printf("ce: %d\n", ce);
	}

}

 void BotCalk::onConnect()
{
	printf("connected!!!\n");
	m_room->join();
	m_room->getRoomInfo();
	m_room->getRoomItems();
}

 void BotCalk::onDisconnect(ConnectionError e)
{
	printf("message_test: disconnected: %d\n", e);
	if (e == ConnAuthenticationFailed)
		printf("auth failed. reason: %d\n", m_client->authError());
}

 bool BotCalk::onTLSConnect(const CertInfo & info)
{
	printf
	    ("status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n",
	     info.status, info.issuer.c_str(), info.server.c_str(),
	     info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
	     info.compression.c_str());
	return true;
}

 void BotCalk::handleLog(LogLevel level, LogArea area,
				const std::string & message)
{
	//printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

void BotCalk::handleMUCParticipantPresence(MUCRoom * /*room */ ,
					   const MUCRoomParticipant
					   participant,
					   Presence::PresenceType presence)
{
	printf("odd.................\n");
}

 void BotCalk::handleMUCParticipantPresence(MUCRoom * /*room */ ,
						   const MUCRoomParticipant
						   participant,
						   const Presence &
						   presence)
{
	if (presence.presence() == Presence::Available)
		printf("!!!!!!!!!!!!!!!! %s is in the room, too\n",
		       participant.nick->resource().c_str());
	else if (presence.presence() == Presence::Unavailable)
		printf("!!!!!!!!!!!!!!!! %s left the room\n",
		       participant.nick->resource().c_str());
	else
		printf("Presence is %d of %s\n", presence.presence(),
		       participant.nick->resource().c_str());
}

 void BotCalk::handleMUCMessage(MUCRoom * /*room */ ,
				       const Message & msg, bool priv)
{
	printf("%s said: '%s' (history: %s, private: %s)\n",
	       msg.from().resource().c_str(), msg.body().c_str(),
	       msg.when()? "yes" : "no", priv ? "yes" : "no");
	
	if(priv){
		m_room->send("hi i am BotCalk, just a Bot");
	}
	if(!msg.when()){
		std::string str_t = msg.from().resource()+msg.body();

		//if(write(put_msg,str_t.c_str(),sizeof(str_t))<0){
		//	printf("write fifo error\n");
		//}
	}
}

 void BotCalk::handleMUCSubject(MUCRoom * /*room */ ,
				       const std::string & nick,
				       const std::string & subject)
{
	if (nick.empty())
		printf("Subject: %s\n", subject.c_str());
	else
		printf("%s has set the subject to: '%s'\n", nick.c_str(),
		       subject.c_str());
}

 void BotCalk::handleMUCError(MUCRoom * /*room */ ,
				     StanzaError error)
{
	printf("!!!!!!!!got an error: %d", error);
}

 void BotCalk::handleMUCInfo(MUCRoom * /*room */ , int features,
				    const std::string & name,
				    const DataForm * infoForm)
{
	printf("features: %d, name: %s, form xml: %s\n",
	       features, name.c_str(), infoForm->tag()->xml().c_str());
}

 void BotCalk::handleMUCItems(MUCRoom * /*room */ ,
				     const Disco::ItemList & items)
{
	Disco::ItemList::const_iterator it = items.begin();
	for (; it != items.end(); ++it) {
		printf("%s -- %s is an item here\n",
		       (*it)->jid().full().c_str(), (*it)->name().c_str());
	}
}

 void BotCalk::handleMUCInviteDecline(MUCRoom * /*room */ ,
					     const JID & invitee,
					     const std::string & reason)
{
	printf("Invitee %s declined invitation. reason given: %s\n",
	       invitee.full().c_str(), reason.c_str());
}

 bool BotCalk::handleMUCRoomCreation(MUCRoom * room)
{
	printf("room %s didn't exist, beeing created.\n",
	       room->name().c_str());
	return true;
}
