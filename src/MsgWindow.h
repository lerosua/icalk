/*
 * =====================================================================================
 * 
 *       Filename:  MsgWindow.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  2007年06月13日 19时53分27秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef _MSGWINDOW_HH
#define _MSGWINDOW_HH
#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA
#include <gtkmm/accelgroup.h>


class GlademmData {

	Glib::RefPtr < Gtk::AccelGroup > accgrp;
      public:

	GlademmData(Glib::RefPtr < Gtk::AccelGroup > ag):accgrp(ag) {
	} Glib::RefPtr < Gtk::AccelGroup > getAccelGroup() {
		return accgrp;
	}
};
#endif				//GLADEMM_DATA

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <iostream>
#include <gtkmm/notebook.h>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include "MsgPage.h"

#define msg_ui  DATA_DIR"/ui/msg_window.glade"
typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class MsgPage;
/**
 * @brief 会话中的消息窗口。
 *
 */
class MsgWindow:public Gtk::Window {
      public:
	MsgWindow();

	void on_delete();
	/**
	 * @brief 响应删除窗口事件
	 */
	bool on_delete_event(GdkEventAny * events);
	/**
	 * @brief 整个聊天窗口的按键事件处理回调
	 * @param ev 事件参数
	 */
	bool on_key_press_event(GdkEventKey * ev);
	/**
	 * @brief 聊天窗口页上的关闭按钮的回调函数，用于关闭窗口
	 */
	void on_switch_page(GtkNotebookPage * page, guint index);
	void on_page_close_click();
	/**
	 * @brief 添加聊天窗口页
	 * @param page 需要删除的聊天窗口页
	 */
	void add_page(MsgPage & page);
	/**
	 * @brief 删除聊天窗口页
	 * @param page 需要删除的聊天窗口页
	 * @note 不需要直接调用，在MsgPage里的close函数里调用。
	 */
	void del_page(MsgPage & page);

	/**
	 * @brief 显示或者隐藏打字图标
	 * @param isShow 真或假
	 */
	void showTypeImage(bool isShow);
	/**
	 * @brief 显示或者隐藏加密图标
	 * @param isShow 真或假
	 */
	void showEncryptImage(bool isShow);

	/**
	 * @brief 获取当前聊天窗口（MsgPage）
	 */
	MsgPage *getCurrentPage() {
		return static_cast <
		    MsgPage * >(notebook->get_current()->get_child());
	}
	/**
	 * @brief 设置当前聊天窗口(MsgPage)
	 * @param page_ 需要设置的聊天窗口
	 */ void setCurrentPage(MsgPage * page_) {
		int num = notebook->page_num(*page_);
		notebook->set_current_page(num);
	}
	/**
	 * @brief 设置聊天窗口题目是否显示为有新消息
	 * @param mode 为真则显示为有新消息，为假则显示为普通
	 */
	void setNewMsgTitle(bool mode);
	~MsgWindow();
      private:
	MsgWindow(const MsgWindow & rhs);	//禁止拷贝
	MsgWindow & operator =(const MsgWindow rhs);	//禁止拷贝

      private:
	GlademmXML msg_xml;
	Gtk::Notebook * notebook;
	GlademmData *gmm_data;
};


#endif
