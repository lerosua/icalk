/*
 * =====================================================================================
 * 
 *       Filename:  TalkMenu.h
 * 
 *    Description:  菜单创建类集合
 * 
 *        Version:  1.0
 *        Created:  2007年07月21日 22时36分02秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  TALKMENU_FILE_HEADER_INC
#define  TALKMENU_FILE_HEADER_INC

#include <gtkmm.h>
#include <gtkmm/menu.h>

class MainWindow;
/** 托盘图标的右键菜单*/
class TrayMenu
{
	public:
		TrayMenu(MainWindow& wnd);
		void popup(guint button,guint time);
	private:
	     Glib::RefPtr<Gtk::UIManager> m_refUIManager;
};

/** 好友列表中的右键菜单*/
class BuddyMenu : public Gtk::Menu {
	public:
		BuddyMenu(MainWindow& wnd);
};

/** 列表中聊天室的右键菜单*/
class RoomMenu : public Gtk::Menu {
	public:
		RoomMenu(MainWindow& wnd);
};

/** @ref MainWindow 中的系统菜单*/
class SystemMenu: public Gtk::Menu {
	public:
		SystemMenu(MainWindow& wnd);
};

#endif   /* ----- #ifndef TALKMENU_FILE_HEADER_INC  ----- */

