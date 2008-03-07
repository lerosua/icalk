/*
 * =====================================================================================
 * 
 *       Filename:  TreeViewTooltips.h
 * 
 *    Description:  好友列表中的Tooltips
 * 
 *        Version:  1.0
 *        Created:  2007年10月09日 20时05分27秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  TREEVIEWTOOLTIPS_FILE_HEADER_INC
#define  TREEVIEWTOOLTIPS_FILE_HEADER_INC
#include <gtkmm.h>
#include "BuddyView.h"
/**
 * @brief 用于显示好友信息提示。
 * @note gtkmm2.20开始才为TreeView添加了Tooltips功能。而之前的版本则需要
 * 自己写控件处理。方法是通过获取TreeView鼠标移动和离开事件，然后在某停留处
 * 加上超时函数以显示一个窗口的方法显示Tooltips，而这个窗口则重画成Tooltips
 * 模样。
 */
class TreeViewTooltips:public Gtk::Window
{
	public:
		TreeViewTooltips(BuddyView* view);
		~TreeViewTooltips(){}
		/** 此函数用于绘制界面*/
		virtual bool on_expose_event(GdkEventExpose* event);

		void setLabel(const Glib::ustring& msg){
			label->set_markup(msg);
		}
		void setImage(const Glib::RefPtr<Gdk::Pixbuf> pixbuf){
			avatar->set(pixbuf);	
		}
		/** 隐藏Tooltips*/
		void hideTooltip();
		/**显示Tooltips，ev用于得到当前所在的好友栏*/
		void showTooltip(GdkEventMotion* ev);
		bool on_motion_event(GdkEventMotion* ev);
		bool on_leave_event(GdkEventCrossing* ev);


	private:
		BuddyView* buddyview; 
		Gtk::Label* label;
		Gtk::Image* avatar;


};


#endif   /* ----- #ifndef TREEVIEWTOOLTIPS_FILE_HEADER_INC  ----- */

