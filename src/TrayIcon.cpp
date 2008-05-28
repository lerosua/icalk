#include <glib/gi18n.h>
#include "TrayIcon.h"
#include "pixmaps.h"
#include "MainWindow.h"


TrayIcon::TrayIcon(MainWindow* wnd)
                : _gwin(wnd)
                , m_status(STATUS_UNKNOW)
{
        pixUser[STATUS_UNKNOW] = getPix("status_offline.png");
        pixUser[STATUS_ONLINE] = getPix("status_online.png");
        pixUser[STATUS_CHAT] = getPix("status_online.png");
        pixUser[STATUS_AWAY] = getPix("status_away.png");
        pixUser[STATUS_DND] = getPix("status_dnd.png");
        pixUser[STATUS_EX] = getPix("status_ex.png");
        pixUser[STATUS_OFFLINE] = getPix("status_offline.png");
        pixUser[STATUS_NEW_MSG] = getPix("status_msg.png");
        tooltip[STATUS_UNKNOW] = _("(Unknow)  ");
        tooltip[STATUS_ONLINE] = _("(Online)  ");
        tooltip[STATUS_CHAT] = _("(Chat)  ");
        tooltip[STATUS_AWAY] = _("(Away)  ");
        tooltip[STATUS_DND] = _("(Don't Disturb)  ");
        tooltip[STATUS_EX] = _("(Extend away)  ");
        tooltip[STATUS_OFFLINE] = _("(Offline)  ");
        tooltip[STATUS_NEW_MSG] = _("New Message");
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
        (pixUser[STATUS_OFFLINE]);
}

TrayIcon::~TrayIcon()
{}

void TrayIcon::setBlinking(bool blinking)
{
        if (blinking)
                this->set
                (pixUser[STATUS_NEW_MSG]);
        else
                this->set
                (pixUser[m_status]);

        this->set_blinking(blinking);

}

void TrayIcon::on_status_change(int f_status, Glib::ustring nickname, Glib::ustring f_msg)
{

        if (!this->get_blinking())
                this->set
                (pixUser[f_status]);

        // Set the tooltip
        this->set_tooltip(nickname + tooltip[f_status] + f_msg);

        m_status = f_status;
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
