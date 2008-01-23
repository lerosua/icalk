/*
 * =====================================================================================
 *
 *       Filename:  JingleWatchSessionTask.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年01月23日 16时37分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#ifndef JINGLEWATCHSESSIONTASK_H_
#define JINGLEWATCHSESSIONTASK_H_

#include <xmpp_tasks.h>

/**
 * This task watch for incoming Jingle session and notify manager.
 * @cn 用于监视进入的Jingle会话并通知会话管理
 */
class JingleWatchSessionTask: public XMPP::Task
{
	public:
		JingleWatchSessionTask();
		~JingleWatchSessionTask();
		
		bool take(const);

		void watchSession(const std::string& sessionType,const std::string& initiator);
};

#endif

