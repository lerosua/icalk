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
#include <iostream> 
//#include <sigc++/compatibility.h>

#define server_discovery_ui DATA_DIR"/ui/service_discovery_window.glade"
typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class Bodies;

class MainWindow;

class AgentLine;

class ServerDiscoWindow: public Gtk::Window
{

public:
        ServerDiscoWindow(MainWindow* parent_);
        ~ServerDiscoWindow();
        void addAgent(const std::string& f_jid);
	/** 
	 * @brief 窗口的进度条，
	 * @param f_blink True:进度条来回闪动，False:进度条满
	 */
	void progress(const bool f_blink);

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

};

class AgentLine: public Gtk::TreeView
{

public:
        AgentLine();
        void addLine(const std::string& f_jid, const int f_type = 0);

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

