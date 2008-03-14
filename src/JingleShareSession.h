/*
 * =====================================================================================
 *
 *       Filename:  JingleShareSession.h
 *
 *    Description:  Jingle File share Session
 *
 *        Version:  1.0
 *        Created:  2008年01月25日 14时31分59秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

class JingleSession;

namespace jingle {
/**
 * Implement a Jingle file share peer-to-peer session that is compatible with Google Talk share offering.
 * @cn 提供兼容Gtalk文件传输的会话
 */

class JingleShareSession: public JingleSession {

public:
        typedef std::list<XMPP::Jid> JidList;
        JingleShareSession(Client* client_, const JidList& peers_);
        virtual ~JingleShareSession();
        virtual std::string sessionType();


private:
        talk_base::scoped_ptr<cricket::FileShareSessionClient> file_share_session_client;



};




}
