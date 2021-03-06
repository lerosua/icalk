/*
* =====================================================================================
*
*       Filename:  TrayIcon.h
*
*    Description:  系统托盘的实现
*
*        Version:  1.0
*        Created:  2007年08月31日 20时46分47秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  TRAYICON_FILE_HEADER_INC
#define  TRAYICON_FILE_HEADER_INC

#include <gtkmm/statusicon.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/eventbox.h>


class MainWindow;
void on_statusicon_activated(GtkWidget*, gpointer object);
void on_statusicon_popup(GtkStatusIcon*, guint button, guint activate_time, gpointer object);
enum { STATUS_UNKNOW = 0, STATUS_ONLINE, STATUS_CHAT,
       STATUS_AWAY, STATUS_DND, STATUS_EX, STATUS_OFFLINE,
       STATUS_NEW_MSG };
/**
 * @brief 通知栏图标类
 */

class TrayIcon: public Gtk::StatusIcon
{

public:
        TrayIcon(MainWindow* wnd);
        ~TrayIcon();
        /** 状态改变时的相应更改函数*/
        void on_status_change(int status, Glib::ustring nickname, Glib::ustring msg_);
        void setBlinking(bool blinking = true);

private:
        MainWindow* _gwin;
        Glib::RefPtr<Gdk::Pixbuf> pixUser[8];
        Glib::ustring tooltip[8];
        int m_status;

protected:
        // Methods:
        friend void on_statusicon_popup(GtkStatusIcon*, guint button, guint activate_time, gpointer object);

};


#endif   /* ----- #ifndef TRAYICON_FILE_HEADER_INC  ----- */

