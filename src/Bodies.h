/*
 * =====================================================================================
 * 
 *       Filename:  Bodies.h
 * 
 *    Description:  主类,负责创建其它各个功能类.
 * 
 *        Version:  1.0
 *        Created:  2007年06月22日 20时59分48秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  wind (xihe), xihels@gmail.com
 *        Company:  cyclone
 * 
 * =====================================================================================
 */

#ifndef _BODIES_H_
#define _BODIES_H_

#include <memory>
#include <gtkmm.h>
#include <sigc++/connection.h>
#include <gloox/client.h>
#include <gloox/connectiontcpclient.h>
#include <fstream>
#include <list>
#include "Buddy.h"
#include "BuddyList.h"
#include "TalkMsg.h"
#include "TalkConnect.h"
#include "MainWindow.h"
#include "MsgWindow.h"
#include "BuddyView.h"
#include "TalkCard.h"
#include "TalkRoomHandler.h"
#include "TrayIcon.h"
#include "TalkFT.h"
#include "ConfXml.h"
#include "TalkDiscoHandler.h"
//#include "TalkIbbStreamHandler.h"
using std::list;

typedef list<std::string> USERLIST;
/**
 * @brief 用于集合各种功能的工厂
 */
class Bodies 
{
	private:
		Bodies();
	public:
		~Bodies();
		/**得到全局的Bodies类*/
		static Bodies& Get_Bodies();
		/**进行登录的函数,程序由此开始*/
		bool login(const std::string name,const std::string passwd);
		/**退出登录*/
		void logout();
		/**读取文件中的Account结构*/
		void loadAccountTag();
		/**保存Account结构到文件中*/
		void saveAccountTag();
		/** 得到登录时用的用户列表*/
		USERLIST& getUserList();
		/** 保存登录过的用户列表*/
		void saveUserList(const std::string& jid);

		/**得到好友列表的窗口*/
		MainWindow& 	get_main_window() { return *main_window; }
		/**得到聊天窗口*/
		MsgWindow&	get_msg_window()  { return *msg_window; }
		/**得到好友列表类*/
		BuddyList& 	get_buddy_list()  { return buddy_list; }
		/**得到消息处理类*/
		TalkMsg& 	get_talkmsg() 	  { return talkmsg; }
		/**得到client类*/
		Client&		get_client()	  { return *jclient; }
		/**得到聊天室处理类*/
		TalkRoomHandler& getRoomHandler() { return roomHandler; }
		/**得到带内数据传输处理类*/
		//TalkIbbStreamHandler& getIbbStreamHandler() { return IBBSHandler; }
		/**得到本人jid*/
		JID&		get_jid()	  { return *jid; }
		/** 从配置文件中得到相应的项目*/
		const std::string	 getAccountTag(const std::string& name)
		{
			Tag* tmpTag= accountTag->findChild(name);
			if(tmpTag == NULL){
				//printf("findChild error\n");	
				return std::string();
			}
			return tmpTag->cdata();
		}
		/**设置配置文件中相应的项目*/
		void setAccountTag(const std::string& name, const std::string& value)
		{
			Tag* tmpTag= accountTag->findChild(name);
			if(tmpTag == NULL){
				tmpTag= new Tag(name,value);
				accountTag->addChild(tmpTag);
			}
			else{
				tmpTag->setCData(value);
			}
			saveAccountTag(); 
		}

		/**设置本人的状态与签名档,并且保存到配置文件中*/
		void		set_status(Presence::PresenceType status_,Glib::ustring msg_=""){ 
					jclient->setPresence(status_,1,msg_);
					statusIcon->on_status_change(status_, jid->username(), msg_);
					//setAccountTag("status",status_);
					setAccountTag("message",msg_);
						}
		/**得到书签处理类*/
		//TalkBookMark&    get_bookmark()   { return *bookMark; }
		/**得到VCard管理类*/
		TalkCard&	get_cardManage()  { return cardManage; }
		/**设置本人的VCard信息*/
		void		set_vcard(VCard*);
		/**得到本人的VCard*/
		VCard*	get_vcard() const { return vcard; }
		/**发出获取本人VCard信息的命令*/
		void		fetch_self_vcard()  { cardManage.fetch_vcard(*jid); }
		void disco_test();
		void disconnect(){ connectIO.disconnect();}

	private:
		/**
		 * @brief 程序主循环函数。是TalkMsg类无阻塞连接后需要调用的j->recv()函数的壳。
		 *
		 */
		bool callback(Glib::IOCondition condition);
		/**
		 * @brief 主体中用于连接的函数，非阻塞型连接.
		 * @param name 用户名
		 * @param passwd 用户密码
		 * @return 先不用理这个了。
		 */
		int connect(const char *name, const char* passwd);
		
	private:
		JID* jid;
		std::auto_ptr<Client>     jclient;
		MainWindow*		main_window;
		MsgWindow*		msg_window;
		TrayIcon*		statusIcon;

		BuddyList  buddy_list;

		TalkConnect talkconnect;
		TalkMsg       talkmsg;
		TalkRoomHandler   roomHandler;
		//TalkDiscoHandler  discohandler;
		TalkCard   cardManage;
		TalkFT*	   talkFT;
		//TalkIbbStreamHandler IBBSHandler;
		VCard*	   vcard;
		//用于保存account结构的Tag
		Tag*	   accountTag;
		USERLIST   userlist;
		sigc::connection connectIO;

};

#endif // _BODIES_H_

