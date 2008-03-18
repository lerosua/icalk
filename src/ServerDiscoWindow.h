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

class ServerDiscoWindow: public Gtk::Window
{

public:
        ServerDiscoWindow(MainWindow* parent_);
        ~ServerDiscoWindow();

private:
        bool on_key_press_event(GdkEventKey* ev);
        void on_btGo_clicked();
        void on_btclose_clicked();

private:
        GlademmXML server_discovery_xml;
        MainWindow* parent;
        Gtk::ComboBoxEntry* nodeEntry;

};




#endif   /* ----- #ifndef SERVERDISCOVERYWINDOW_FILE_HEADER_INC  ----- */

