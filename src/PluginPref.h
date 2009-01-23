/*
 * =====================================================================================
 *
 *       Filename:  PluginPref.h
 *
 *    Description:  插件管理窗口
 *
 *        Version:  1.0
 *        Created:  2008年10月14日 11时12分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */


#ifndef PLUGIN_PREF_HREADER_INC
#define PLUGIN_PREF_HREADER_INC

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeselection.h>
#include <functional>
#include <vector>
#include "plugin.h"


using namespace std;

class MainWindow;
class PluginManager;

/**
 * @brief 插件管理窗口
 */
class PluginPref:public Gtk::Window
{
	public:
		PluginPref(MainWindow* f_parent);
		virtual ~PluginPref();
		void on_button_config();
		void on_button_close();

	protected:
		class ModelColumns:public Gtk::TreeModel::ColumnRecord
	{
		public:
			ModelColumns()
			{
				add(m_plugin_id);
				add(m_plugin_load);
				add(m_plugin_name);
			}

			Gtk::TreeModelColumn<unsigned int> m_plugin_id;
			Gtk::TreeModelColumn<Glib::ustring> m_plugin_name;
			Gtk::TreeModelColumn<bool> m_plugin_load;
	};
		ModelColumns m_Columns;

		//Child widgets;
		Gtk::VBox m_VBox;
		Gtk::ScrolledWindow m_ScrolledWindow;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
		Gtk::HButtonBox m_ButtonBox;
		Gtk::Button m_Button_Quit;

	private:
		MainWindow* parent;
		PluginManager* m_plugin_manager;
};


#endif
