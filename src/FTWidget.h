/*
* =====================================================================================
*
*       Filename:  FTWidget.h
*
*    Description:  File transport window
*
*        Version:  1.0
*        Created:  2008年03月18日 09时16分26秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef FTWIDGET_H_
#define FTWIDGET_H_
#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeselection.h>

using namespace std;

typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class MainWindow;

/**
 * 显示文件传输状态的窗口
 */

class FTWidget: public Gtk::Window
{

public:
        FTWidget(MainWindow* parent_);
        ~FTWidget();
        void on_button_ok();
        void on_button_cancel();
        void on_button_del();

private:
        MainWindow* parent;



};

#endif
