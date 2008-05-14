#include <glib/gi18n.h>
#include "TrayIcon.h"
#include "pixmaps.h"
#include "MainWindow.h"


TrayIcon::TrayIcon(MainWindow* wnd)
                : _gwin(wnd)
{
        pixUser[0] = getPix("status_offline.png");
        pixUser[1] = getPix("status_online.png");
        pixUser[2] = getPix("status_online.png");
        pixUser[3] = getPix("status_away.png");
        pixUser[4] = getPix("status_dnd.png");
        pixUser[5] = getPix("status_ex.png");
        pixUser[6] = getPix("status_offline.png");
        tooltip[0] = _("(Unknow)  ");
        tooltip[1] = _("(Online)  ");
        tooltip[2] = _("(Chat)  ");
        tooltip[3] = _("(Away)  ");
        tooltip[4] = _("(Don't Disturb)  ");
        tooltip[5] = _("(Extend away)  ");
        tooltip[6] = _("(Offline)  ");
        /*
         tooltip[0] = "(未知状态)  ";
         tooltip[1] = "(在线)  ";
         tooltip[2] = "(唠叨)  ";
         tooltip[3] = "(离开)  ";
         tooltip[4] = "(请勿打扰)  ";
         tooltip[5] = "(远远离开)  ";
         tooltip[6] = "(未连接)  ";
        */

        GtkStatusIcon* gobj_StatusIcon = this->gobj();
        g_signal_connect(G_OBJECT(gobj_StatusIcon), "activate", G_CALLBACK(on_statusicon_activated), _gwin);
        g_signal_connect(G_OBJECT(gobj_StatusIcon), "popup-menu", G_CALLBACK(on_statusicon_popup), _gwin);

        this->set
        (pixUser[6]);
}

TrayIcon::~TrayIcon()
{}

void TrayIcon::setBlinking(bool blinking)
{
        this->set_blinking(blinking);
}

void TrayIcon::on_status_change(int status, Glib::ustring nickname, Glib::ustring msg_)
{

        this->set
        (pixUser[status]);

        // Set the tooltip
        this->set_tooltip(nickname + tooltip[status] + msg_);

}

void on_statusicon_activated(GtkWidget* widget, gpointer object)
{
        MainWindow* wnd = static_cast<MainWindow*>(object);
        wnd->toggle_visibility();
}

void on_statusicon_popup(GtkStatusIcon* status_icon, guint button,
                         guint activate_time, gpointer object)
{
        return static_cast<MainWindow*>(object)->show_tray_menu(button, activate_time);
}
