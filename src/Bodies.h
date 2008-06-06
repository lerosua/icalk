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
//#include "MainWindow.h"
#include "MsgWindow.h"
#include "BuddyView.h"
#include "TalkCard.h"
#include "TalkRoomHandler.h"
#include "RoomInvitation.h"
#include "TrayIcon.h"
#include "TalkFT.h"
#include "ConfXml.h"
#include "TalkDiscoHandler.h"
#include "icalk.h"
using std::list;

using namespace std;
//class TalkFT;

class MainWindow;
//class TalkCard;

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
        MainWindow& get_main_window()
        {
                return *main_window;
        }

        /**得到聊天窗口*/
        MsgWindow& get_msg_window()
        {
                return *msg_window;
        }

        /**得到好友列表类*/
        BuddyList& get_buddy_list()
        {
                return buddy_list;
        }

        /**得到消息处理类*/
        TalkMsg& getTalkMsg()
        {
                return m_talkMsg;
        }

        /**得到client类*/
        Client& get_client()
        {
                return *m_client;
        }

        /**得到聊天室处理类*/
        TalkRoomHandler& getRoomHandler()
        {
                return m_roomHandler;
        }

        /**得到本人jid*/
        const JID& get_jid() const
        {
                return m_jid;
        }

        /** 从配置文件中得到相应的项目*/
        const std::string getAccountTag(const std::string& name) ;


        /**设置配置文件中相应的项目*/
        void setAccountTag(const std::string& name, const std::string& value) ;

        /**设置本人的状态与签名档,并且保存到配置文件中*/
        void set_status(Presence::PresenceType f_status, Glib::ustring f_msg = "") ;

        /** 返回文件传输处理类 */
        TalkFT& getFThandler()
        {
                return *m_talkFT;
        }

        /**得到书签处理类*/
        //TalkBookMark&    get_bookmark()   { return *bookMark; }
        /**得到VCard管理类*/
        TalkCard& getCardManage()
        {
                return *m_cardManage;
        }

        /**设置本人的VCard信息*/
        void set_vcard(const VCard*);
        /**得到本人的VCard*/
        const VCard* get_vcard() const
        {
                return m_vcard;
        }

        /**发出获取本人VCard信息的命令*/
        void fetch_self_vcard() ;

        /** 提示是否有新消息，调用TrayIcon的闪动显示*/
        void promptMsg(const Glib::ustring& f_jid, bool f_new);
        void disco_node(const std::string& node);
        void disconnect();

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
         * @param server 连接服务器的地址
         * @param port 连接服务器的端口，默认 5222
         * @return 先不用理这个了。
         */
        int connect(const string& name, const string& passwd, const string& server, const int port);

private:
        JID m_jid;
        std::auto_ptr<Client> m_client;
        MainWindow* main_window;
        MsgWindow* msg_window;
        TrayIcon* statusIcon;
        int msg_count; //用于消息计数

        BuddyList buddy_list;

        TalkConnect m_talkConnect;
        TalkMsg m_talkMsg;
        TalkRoomHandler m_roomHandler;
        TalkDiscoHandler m_discoHandler; // it can look over the server now
        RoomInvitation m_roominvitation;
        TalkCard* m_cardManage;
        TalkFT* m_talkFT;
        VCard* m_vcard;
        //用于保存account结构的Tag
        Tag* accountTag;
        USERLIST userlist;
        sigc::connection connectIO;

protected:
        /**进行登录的函数,程序由此开始*/
        // 开始建立连接，由 MainWindow 回调
        virtual bool on_login(string f_name, string f_passwd, string f_server, int f_port);
        // 连接建立成功，由 TalkConnect 回调
        virtual void has_login();
        //退回登录界面，重新登录
        virtual void relogin();
};

#endif // _BODIES_H_

