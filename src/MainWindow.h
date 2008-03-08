/*
 * =====================================================================================
 * 
 *       Filename:  MainWindow.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  2007年05月24日 19时46分23秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef _MAINWINDOW_HH
#define _MAINWINDOW_HH



#include <gtkmm.h>
#include <libglademm/xml.h>
#include <iostream>
#include <sigc++/connection.h>
#include "TalkMenu.h"
#include "StatusMsgWidget.h"
#include "BuddyInfoWindow.h"
#include "ServerDiscoWindow.h"

#define main_ui	    DATA_DIR"/ui/main_window.glade"
typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;
class Bodies;
class BuddyView;
/** 标识当前窗口为已连接后的页*/
#define LOGIN_FINIAL	2
/** 标识当前窗口为正在连接的页*/
#define LOGIN_LOADING   1
/** 标识当前窗口为未连接的页*/
#define LOGIN_INIT	0
/**
 * @brief 好友列表窗口，主窗口
 */
class MainWindow:public Gtk::Window
{
	public:
		MainWindow(Bodies& bodies_);

		/** 用于初始化一些菜单的动作。*/
		void on_initalize();

		/**返回好友列表UI类*/
		BuddyView& get_buddy_view() { return *list_view; }

		/**构造状态栏的列表需要的类*/
		class ModelColumns:public Gtk::TreeModel::ColumnRecord
		{
		public:
			ModelColumns(){add(col_status);}
			Gtk::TreeModelColumn<Glib::ustring> col_status;
		};
		ModelColumns m_Columns;

		/** 状态栏改变时回调*/
		void on_combo_change();
		/** 当签名改变时回调*/
		void on_entryStatus_change();
		/** 返回主窗口右上角的Logo*/
		Glib::RefPtr<Gdk::Pixbuf> getLogo(){ return logo;}
		/** 设置窗口右上角的Logo图标*/
		void set_logo(const std::string& iconpath);
		/** 设置窗口最上边标签的信息，一般是本ID*/
		void set_label();
		/** 接收到删除事件时的回调*/
		bool on_delete_event(GdkEventAny*);
		/** 退出程序处理函数*/
		void on_quit();
		/** 清除登录窗口中所有entry中的数据*/
		void on_clean_entry();
		/** 处理窗口按钮事件, 主要是为响应签名档的设置事件*/
		bool on_key_press_event(GdkEventKey* ev);
		virtual ~MainWindow();

	public:
		/** 右上角logo的按钮点击时跳出选择文件窗口*/
		void on_btnLogo_clicked();
		/** 主窗口左下的系统按钮的回调*/
		void on_btnSystem_clicked();
		/** 主窗口下面按钮，签名消息管理按键*/
		void on_btstatusmsgmanager_clicked();
		/** 主窗口下面按钮第三个*/
		void on_btnAudio_clicked();
		/** 展开或折叠好友列表快捷按钮*/
		void on_btlistexpand_clicked();
		/** 显示或隐藏离线好友快捷按钮*/
		void on_btlistshowoffline_clicked();
		void on_popup_menu_pos(int& x, int& y, bool& push_in, int id);
		/** 显示TrayIcon图标的右键菜单*/
		void show_tray_menu(guint button,guint activate_time);

		/** 打开聊天窗口与好友聊天*/
		void on_buddyChat_activate();
		/** 发出编辑好友昵称的信号*/
		void on_buddyNameEdit_activate();
		/** 查看好友聊天记录时回调*/
		void on_buddyLog_activate();
		/** 查看好友资料时回调。窗口返回VCard*/
		void on_buddyInfo_activate();
		/** 删除好友回调，会跳出窗口询问是否删除*/
		void on_buddyRemove_activate();
		/** 更改好友类型窗口*/
		void on_buddyType_activate();
		/** 添加好友对话窗口*/
		void on_addBuddy_activate();
		/** 添加聊天室的窗口*/
		void on_addRoom_activate();
		/** 查找好友的窗口回调*/
		void on_findBuddy_activate();
		/** 查找服务的窗口回调*/
		void on_serverDisco_activate();
		/** 刷新好友列表*/
		void on_freshLIst_activate();
		/** 显示关于窗口，关于窗口有作者信息，版权等*/
		void on_about_activate();
		/** 显示首选项窗口*/
		void on_setPrefer_activate();
		/** 显示或隐藏窗口*/
		void toggle_visibility();
		/** 设置是否静音*/
		void on_sound_activate();
		/** 是否显示离线好友*/
		void on_show_all_friends();
		/** 屏蔽房间，等于不自动加入聊天室*/
		void on_block_room();
		/** 删除房间的回调*/
		void on_delRoom_activate();
		/** 屏蔽好友，将收不到好友信息*/
		void on_block_friend();

		/**登录成功后显示列表页*/
		void on_login_finial();
		/** 登录中，显示正在登录的标签页*/
		void on_logining()    { main_notebook->set_current_page(LOGIN_LOADING);
					groalSet.STATUS=LOGIN_LOADING;
		}
		/** 重新登录，显示登录框的页*/
		void on_relogin()	{ main_notebook->set_current_page(LOGIN_INIT);
					groalSet.STATUS=LOGIN_INIT;
					}
		/** 在登录中取消登录*/
		void on_logining_cancel();
		/** 登录处理函数，将跳到@link Bodies @endlink的login函数中进行登录处理*/
		void on_login();
		/** 当登录窗口里帐户框口变化时*/
		void on_account_changed();
		/**初始化房间*/
		void initRoom();
	public:
		/** 获取当前的签名消息*/
		Glib::ustring getStatusMsg() const { return statusEntry->get_text();}
		/** 设置当前的签名消息*/
		void setStatusMsg(const std::string& msg); 
		/** 添加状态签名超时函数，用于设置N分钟就换一次签名*/
		void addStatusMsgTimeout();
		/** 删除状态签名超时*/
		void delStatusMsgTimeout();
		/** 状态签名超时函数*/
		bool statusMsgTimeout();
		/** 返回是否设置了状态签名超时*/
		bool isMsgTimeout(){ return msgTimeout.connected(); }

	private:
		/** 全局变量集合*/
		typedef struct GroalSet{
			bool SHOWALLFRIEND; /**显示离线好友*/
			bool MUTE;		/** 静音*/
			int STATUS;		/**登录状态*/
			int TIMECOUNT;		/**消息超时的计时器*/
		}GroalSet;

	private:
		Bodies& bodies;
		BuddyView* list_view;
		GlademmXML main_xml;
		Gtk::ComboBox* statusCombo;
		Gtk::Entry*    statusEntry;
		Gtk::Button*   buttonSystem;
		Glib::RefPtr<Gdk::Pixbuf> logo;
		Gtk::Notebook* main_notebook;
		Gtk::ComboBoxEntryText* comboAccount;
		Gtk::Entry*	entryPasswd;
		Gtk::CheckButton* keeppasswd;
		Gtk::CheckButton* keepMe;
		Gtk::Entry* entryServer;
		Gtk::Entry* entryPort;
		sigc::connection msgTimeout;
		GroalSet groalSet;

		int win_x, win_y;

		SystemMenu	systemMenu;
		TrayMenu	trayMenu;

};

#endif
