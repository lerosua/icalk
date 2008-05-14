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

//#define ftwidget_ui DATA_DIR"/ui/ftwidget.glade"
using namespace std;

//typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class MainWindow;

/**
 * 显示文件传输状态的窗口
 */

class FTWidget: public Gtk::Window
{

public:
        FTWidget(MainWindow* parent_);
        ~FTWidget();
	/** 窗口退出*/
        void on_button_quit();
	/** 对于文件传输窗口里的所选项的传输过程的停止*/
        void on_button_stop();
	/** 对于文件传输窗口里的所选项的删除*/
        void on_button_del();
	/** 对于文件传输窗口里的所选项的传输继续，如果传输未完成的情况下*/
	void on_button_continue();
        bool on_key_press_event(GdkEventKey* ev);
        bool on_delete_event(GdkEventAny*);

	protected:
		class XferColumns: public Gtk::TreeModel::ColumnRecord
	{
		public:
		XferColumns()
		{
			add(m_icon);
			add(m_sid);
			add(m_filename);
			add(m_size);
			add(m_target);
			add(m_percent);
		}

		/** 传输过程的图标显示*/
		Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >m_icon;
		/** 此传输的sid标识*/
		Gtk::TreeModelColumn < std::string> m_sid;
		/** 传输的文件的名字*/
		Gtk::TreeModelColumn < Glib::ustring> m_filename;
		/** 传输文件的大小*/
		Gtk::TreeModelColumn <int > m_size;
		/** 传输的接收方*/
		Gtk::TreeModelColumn <Glib::ustring> m_target;
		/** 传输进行时的完成的文件百分比*/
		Gtk::TreeModelColumn <int > m_percent;
	};
	XferColumns m_columns;
private:
        MainWindow* m_parent;
	Gtk::TreeView m_TreeView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	Gtk::ScrolledWindow m_ScrolledWindow;
	Gtk::VBox m_VBox;
	Gtk::HButtonBox m_ButtonBox;
	Gtk::Button* m_Button_Quit;
	Gtk::Button* m_Button_Stop;
	Gtk::Button* m_Button_Delete;
	Gtk::Button* m_Button_Continue;

};

#endif
