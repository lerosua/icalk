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

#define ftwidget_ui DATA_DIR"/ui/ftwidget.glade"
using namespace std;

typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class MainWindow;
class XferLine;

/**
 * 显示文件传输状态的窗口
 */

class FTWidget: public Gtk::Window
{

public:
        FTWidget(MainWindow* parent_);
        ~FTWidget();
        void on_button_quit();
        void on_button_stop();
        void on_button_del();
	void on_button_continue();

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

		Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >m_icon;
		Gtk::TreeModelColumn < std::string> m_sid;
		Gtk::TreeModelColumn < Glib::ustring> m_filename;
		Gtk::TreeModelColumn <int > m_size;
		Gtk::TreeModelColumn <Glib::ustring> m_target;
		Gtk::TreeModelColumn <int > m_percent;
	};
	XferColumns m_columns;
private:
        MainWindow* parent;
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
/*
class XferLine:public Gtk::TreeView
{
	public:
		XferLine();
		~XferLine();

	private:
		struct XferColumns: public Gtk::TreeModel::ColumnRecord
	{
		XferColumns()
		{
			add(m_icon);
			add(m_sid);
			add(m_filename);
			add(m_size);
			add(m_target);
			add(m_percent);
		}

		Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >m_icon;
		Gtk::TreeModelColumn < std::string> m_sid;
		Gtk::TreeModelColumn < Glib::ustring> m_filename;
		Gtk::TreeModelColumn <int > m_size;
		Gtk::TreeModelColumn <Glib::ustring> m_target;
		Gtk::TreeModelColumn <int > m_percent;
	};
		XferColumns m_columns;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

};
*/
#endif
