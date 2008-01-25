/*
 * =====================================================================================
 *
 *       Filename:  JingleSessionManager.h
 *
 *    Description:  jinglesession manager
 *
 *        Version:  1.0
 *        Created:  2008年01月23日 11时43分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#ifndef JINGLESESSIONMANAGER_H_
#define JINGLESESSIONMANAGER_H_

#include <xmpp.h>

using namespace gloox;
namespace cricket
{
	class SessionManager;
}

class JingleSession;
class Client;

namespace jingle
{

class JingleSessionManager
{

	public:
		typedef std::list<buzz::Jid> JidList;
		JingleSessionManager(Client* client);
		~JingleSessionManager();

		/**
		 * Get the (single) instance of the cricket session manager.
		 * @cn 获取一个cricket的会话管理
		 */
		cricket::SessionManager* cricketSessionManager();

		/**
		 * @brief create a new Jingle session. Returned pointer is managed
		 * by this class
		 */
		JingleSession* createSession(const std::string& sessionType,const JidList& peers);
		/**
		 * Override method that create a session for a one-to-one session
		 * @param sessionType the session you want to create. You must
		 * pass its XML namespace(ex: http://jabber.org/protocol/sessions/audio)
		 * @param user The JID of the user you want to begin a session with.
		 * @cn 重载这个创建一对一的会话
		 */
		JingleSession* createSession(const std::string& sessionType,const buzz::Jid& user);

		void removeSession(JingleSession* session);

		void incomingSession(const std::string& sessionType,JingleSession* session);


	private:
		std::list<JingleSession*> sessionList;
		JingleWatchSessionTask* watchSessionTask;
		cricket::NetworkManager* networkManager;
		cricket::BasicPortAllocator* portAllocator;
		cricket::Thread* sessionThread;
		cricket::SessionManager* cricketSessionManager;
		gloox::Client* client;





};


}
#endif
