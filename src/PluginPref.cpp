/*
 * =====================================================================================
 *
 *       Filename:  PluginPref.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年10月14日 11时44分57秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#include <glib/gi18n.h>
#include <iostream>
#include "PluginPref.h"
#include "MainWindow.h"

using namespace std;

PluginPref::PluginPref(MainWindow * f_parent):parent(f_parent)
                , Gtk::Window(Gtk::WINDOW_TOPLEVEL)
		, m_plugin_manager(NULL)
		, m_Button_Quit(_("Close"))
{
	set_title(_("Plugins"));
	set_border_width(5);
        set_default_size(450, 320);

	set_transient_for(*parent);
	add(m_VBox);
	m_ScrolledWindow.add(m_TreeView);
	m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
	m_VBox.pack_start(m_ScrolledWindow);
	m_VBox.pack_start(m_ButtonBox,Gtk::PACK_SHRINK);

	m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
	m_ButtonBox.set_border_width(5);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
	m_Button_Quit.signal_clicked().connect( sigc::mem_fun(*this, &PluginPref::on_button_close));

	//Create the Tree Model;
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	m_TreeView.set_model(m_refTreeModel);

	//Add the TreeView's view columns:
	m_TreeView.append_column(_("ID"),m_Columns.m_plugin_id);
	m_TreeView.append_column_editable(_("Load"),m_Columns.m_plugin_load);
	m_TreeView.append_column(_("Name"),m_Columns.m_plugin_name);

#if 0
	//添加一个example来展示
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_Columns.m_plugin_id]=1;
	row[m_Columns.m_plugin_load]=true;
	row[m_Columns.m_plugin_name]="Icalk plugin example";
#endif

	//创建插件管理类
	m_plugin_manager = new PluginManager();
	m_plugin_manager->probe(G_MODULE_SUFFIX);

	//遍历插件
	const GList* m_plugin_list = m_plugin_manager->get_plugins_list();
	const GList *cur;
	GenericPlugin* plugin;
	int i=2;
	for (cur = m_plugin_list; cur != NULL; cur = cur->next)
	{
		plugin =(GenericPlugin*) cur->data;
		if(plugin )
		{
		     TalkPluginInfo* info=plugin->getPluginInfo();
		     DLOG("get plugin homepage  : %s\n",info->homepage);
			Gtk::TreeModel::Row row = *(m_refTreeModel->append());
			row[m_Columns.m_plugin_id]=i++;
			row[m_Columns.m_plugin_load]=true;
			row[m_Columns.m_plugin_name]=string(info->name);
		}

	}


        show_all();
}

PluginPref::~PluginPref()
{
}

void PluginPref::on_button_close()
{
	parent->on_plugin_close(this);
}

void PluginPref::on_button_config()
{
}
