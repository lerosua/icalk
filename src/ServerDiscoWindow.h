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
static char* agent_type_info[19][3]={ 
	  { "aim",   _("AIM Transports"),"aim.png"}
	, { "icq",   _("ICQ Transports"),"icq.png"}
	, { "msn",   _("MSN Transports"), "msn.png" }
	, { "yahoo", _("Yahoo Transports"), "yahoo.png" }
	, { "qq",    _("QQ Transports"),    "qq.png" }
	, { "gadu-gadu", _("Gadu-Gadu Transports"), "gadu.png"}
	, { "irc",   _("IRC Transports"), "icq.png" }
	, { "conference", _("Conference"), "groupchat.png" }
	, { "rss",    _("RSS Transports"),  "rss.png"	}
	, { "weather", _("Weather"), "other.png" }
	, { "sip",     _("SIP"),     "other.png" }
	, { "bytestreams", _("Socks5 proxy"), "bytestreams.png" }
	, { "pubsub",      _("PubSub"),       "pubsub.png" }
	, { "http-ws" ,    _("Http-ws"),      "other.png" }
	, { "sms",         _("SMS Transports"), "sms.png" }
	, { "smtp",        _("SMTP Transports"), "mail.png" }
	, { "user",        _("User"),		 "jud.png" }
	, { "other",       "",           "other.png" }
};
enum { AGENT_AIM=0, AGENT_ICQ,AGENT_MSN,
		   AGENT_YAHOO, AGENT_QQ, AGENT_GADU,
		   AGENT_IRC,   AGENT_CONFERENCE, AGENT_RSS,
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
        void addAgent(const std::string& f_jid);
	/** 清除列表*/
	void clear();
	/** 
	 * @brief 窗口的进度条，
	 */
	void final_progress();
	bool on_progress();

private:
        bool on_key_press_event(GdkEventKey* ev);
        void on_btGo_clicked();
        void on_btclose_clicked();

private:
        GlademmXML server_discovery_xml;
        MainWindow* m_parent;
        AgentLine* agentline;
	Gtk::ProgressBar* m_progressbar;
        Gtk::ComboBoxEntry* m_nodeEntry;
        sigc::connection m_timeout;

};

class AgentLine: public Gtk::TreeView
{

public:
        AgentLine();
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
        Glib::RefPtr<Gtk::ListStore> m_liststore;
};



#endif   /* ----- #ifndef SERVERDISCOVERYWINDOW_FILE_HEADER_INC  ----- */

