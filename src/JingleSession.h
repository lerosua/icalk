/*
 * =====================================================================================
 * 
 *       Filename:  jinglesession.h
 * 
 *    Description:  Define a Jingle Session.
 * 
 *        Version:  1.0
 *        Created:  2008年01月20日 22时09分05秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  JINGLESESSION_FILE_HEADER_INC
#define  JINGLESESSION_FILE_HEADER_INC

using namespace gloox;
class Client;

namespace jingle{
/**
 * @brief Base class for peer-to-peer session that use Jingle signaling.
 * @cn P2P会话的模板类，使用Jingle信号
 * @author lerosua <lerosua@gmail.com>
 */
class JingleSession
{

	public:
		typedef std::list<buzz::Jid> JidList;

		JingleSession(gloox::Client* client,const JidList& peers);
		virtual ~jingleSession();

		const buzz::Jid& myself() const;
		const JidList& peers() const;
		JidList& peers();
	public:
		/**
		 * Return the type of Session(ex:share-file,voice,video,games)
		 * Note that you must return return the XML namespace that 
		 * define the session: ex:(http://jabber.org/protocol/jingle/sessions/audio)
		 * @cn 返回会话的类型(例如：文件传输，语音，视频，游戏)
		 * 注意您必须返回定义会话的XML的名字空间：比如
		 *    http://jabber.org/protocol/jingle/sessions/audio)
		 */
		virtual std::string sessionType() =0;

		/**
		 * @brief Start a session with the give JID.
		 * You Should begin the negociation here
		 * @cn 以一个给定的JID开始一个会话,您必须在这里开始协商
		 */
		virtual void start() = 0;
		/**
		 * @brief Acept a session request.
		 * @cn 接受一个会话请求
		 */
		virtual void accept() = 0;
		/**
		 * @brief Decline a session request.
		 * @cn 拒绝一个会话请求
		 */
		virtual void decline() = 0;
		/**
		 * @brief Terminate a Jingle Session.
		 * @cn 结束一个Jingle会话
		 */
		virtual void terminate() = 0;

	private:
		buzz::Jid myself;
		JidList peers;
		gloox::Client* client;

};
}
#endif   /* ----- #ifndef JINGLESESSION_FILE_HEADER_INC  ----- */

