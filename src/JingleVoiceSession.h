/*
 * =====================================================================================
 *
 *       Filename:  JingleVoiceSession.h
 *
 *    Description:  Jingle 语音会话
 *
 *        Version:  1.0
 *        Created:  2008年01月23日 12时04分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua@gmail.com
 *        Company:  cyclone
 *
 * =====================================================================================
 */

#ifndef JINGLEVOICESESSION_H_
#define JINGLEVOICESESSION_H_

#include <xmpp.h> //XMPP::Jid

namespace cricket
{
	class Call;
}

class JingleSession;

namespace jingle
{

/**
 * Implement a Jingle voice peer-to-peer session that is compatible with Google Talk voice offering.
 * @cn 实现单对单的语音会话，兼容Gtalk提供的语音服务
 *
 * @author lerosua
 */
class JingleVoiceSession: public JingleSession
{
	public:
		JingleVoiceSession();
		~JingleVoiceSession();

	public:
		virtual void accept();
		virtual void decline();
		virtual void start();
		virtual void terminate();

	private:
		void setCall(cricket::Call* call);

};

}
#endif
