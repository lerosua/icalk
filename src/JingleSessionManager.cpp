/*
 * =====================================================================================
 *
 *       Filename:  JingleSessionManager.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年01月24日 16时49分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */


#include "JingleSessionManager.h"
#include "JingleWatchSessionTask.h"

#define JINGLE_NS "http://www.google.com/session"
#define JINGLE_VOICE_SESSION_NS "http://www.google.com/session/phone"

JingleSessionManager::JingleSessionManager(Client* client_)
	:client(client_)
{



	//Init the port allocator(select best ports) with the Google STUN server to help
	cricket::SocketAddress* googleStunAddress = new cricket::SocketAddress("64.233.167.126",19302);
	// TODO: Define a relay server.
	portAllocator = new cricket::BasicPortAllocator(networkManager,googleStunAddress,0L);

	// Create the Session manager that manager peer-to-peer sessions.
	sessionThread = new cricket::Thread();
	cricketSessionManager = new cricket::SessionManager(portAllocator,sessionThread);
	cricketSessionManager->SignalRequestSignaling.connect(slotsProxy,
			&JingleSessionManager::OnSignalingRequest);
	cricketSessionManager->OnSignalingReady();

	sessionThread->Start();

}

JingleSessionManager::~JingleSessionManager()
{


}

JingleSession* JingleSessionManager::createSession(const std::string& sessionType_, const JidList& peers_)
{
	JingleSession* newSession = 0L;
	if( sessionType_ == JINGLE_VOICE_SESSION_NS)
	{
		newSession = new JingleVoiceSession(xx,peers_);
	}
	else if(sessionType_ == )
	{
	}

	if(newSession)
		sessionLis.append(newSession);

	return newSession;
}

JingleSession* JingleSessionManager::createSession(const std::string& sessionType_,const buzz::Jid& user)
{
	JingleSessionManager::JidList jidList;
	jidList.append(user);

	return createSession(sessionType_,jidList);
}

