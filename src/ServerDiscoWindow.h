/*
* =====================================================================================
*
*       Filename:  ServerDiscoWindow.h
*
*    Description:  发现服务窗口
*
*        Version:  1.0
*        Created:  2007年09月30日 15时57分04秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  SERVERDISCOVERYWINDOW_FILE_HEADER_INC
#define  SERVERDISCOVERYWINDOW_FILE_HEADER_INC

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <sigc++/connection.h>
#include <glib/gi18n.h>
#include <iostream> 
//#include <sigc++/compatibility.h>

#define server_discovery_ui DATA_DIR"/ui/service_discovery_window.glade"
typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

enum { AGENT_AIM = 0, AGENT_ICQ, AGENT_MSN,
       AGENT_YAHOO, AGENT_QQ, AGENT_GADU,
       AGENT_IRC, AGENT_CONFERENCE, AGENT_RSS,
       AGENT_WEATHER, AGENT_SIP, AGENT_BYTESTREAMS,
       AGENT_PUBSUB,
       AGENT_HTTP_WS, AGENT_SMS, AGENT_SMTP,
       AGENT_JUD,
       AGENT_OTHER
     };
#define agent_jid 0
#define agent_info 1
#define agent_icon 2

class Bodies;

class MainWindow;

class AgentLine;

class ServerDiscoWindow: public Gtk::Window
{

public:
        ServerDiscoWindow(MainWindow* parent_);
        ~ServerDiscoWindow();
        /** 添加代理*/
        void addAgent(const std::string& f_jid);
        /**
         * @brief 设置所查询的服务器的信息
         * @param  f_node 所查询的服务器，比如 jabber.org
         * @param  f_server xmpp服务器,比如 ejabbered
         */
        void setLabel(const std::string& f_node, const std::string& f_server = "XMPP");
        /** 清除列表*/
        void clear();
        /**
         * @brief 进度条完成，
         */
        void final_progress();
        /** 进度条开始跳动*/
        bool on_progress();
        void showError();

private:
        bool on_key_press_event(GdkEventKey* ev);
        void on_btGo_clicked();
        void on_btclose_clicked();
        bool on_delete_event(GdkEventAny*);

private:
        GlademmXML server_discovery_xml;
        MainWindow* m_parent;
        AgentLine* agentline;
        Gtk::ProgressBar* m_progressbar;
        Gtk::ComboBoxEntry* m_nodeEntry;
        Gtk::Label* m_label;
        sigc::connection m_timeout;

};

class AgentLine: public Gtk::TreeView
{

public:
        AgentLine();
        /**
         * @brief 添加代理进列表
         * @param f_jid 代理的jid
         * @param f_type 代理的类型，比如msn的transport,icq的transport
         */
        void addLine(const std::string& f_jid, const int f_type = 0);
        /** 清除列表*/
        void clear();

protected:
        bool on_button_press_event(GdkEventButton *);

private:

struct AgentColumns: public Gtk::TreeModel::ColumnRecord
        {
                AgentColumns()
                {
                        add
                                (icon);

                        add
                                (name);

                        add
                                (jid);
                }

                Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >icon;
                Gtk::TreeModelColumn<std::string> name;
                Gtk::TreeModelColumn<std::string> jid;
        };

private:
        AgentColumns m_columns;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};



#endif   /* ----- #ifndef SERVERDISCOVERYWINDOW_FILE_HEADER_INC  ----- */

