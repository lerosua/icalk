/*
 * =====================================================================================
 *
 *       Filename:  FTWidget.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月13日 17时27分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <glib/gi18n.h>
#include "FTWidget.h"
#include "MainWindow.h"

FTWidget::FTWidget(MainWindow* f_parent):m_parent(f_parent)
{
	set_title(_("File Translate"));
	set_border_width(5);
	set_default_size(400,200);
	//m_VBox= Gtk::manage(new Gtk::VBox());
	//m_ScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());

        set_transient_for(*m_parent);
	add(m_VBox);
	m_ScrolledWindow.add(m_TreeView);
	m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_VBox.pack_start(m_ScrolledWindow);
	m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
	m_ButtonBox.set_border_width(5);
	m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);

	m_Button_Continue = Gtk::manage(new Gtk::Button(_("Continue")));
	m_Button_Stop =     Gtk::manage(new Gtk::Button(_("Stop")));
	m_Button_Delete =   Gtk::manage(new Gtk::Button(_("Delete")));
	m_Button_Quit   =   Gtk::manage(new Gtk::Button(_("Quit")));

	m_ButtonBox.pack_start(*m_Button_Continue, Gtk::PACK_SHRINK);
	m_ButtonBox.pack_start(*m_Button_Stop,     Gtk::PACK_SHRINK);
	m_ButtonBox.pack_start(*m_Button_Delete,   Gtk::PACK_SHRINK);
	m_ButtonBox.pack_start(*m_Button_Quit,     Gtk::PACK_SHRINK);

	m_Button_Continue->signal_clicked().connect(sigc::mem_fun(*this,
				&FTWidget::on_button_continue));
	m_Button_Stop->signal_clicked().connect(sigc::mem_fun(*this,
				&FTWidget::on_button_stop));
	m_Button_Delete->signal_clicked().connect(sigc::mem_fun(*this,
				&FTWidget::on_button_del));
	m_Button_Quit->signal_clicked().connect(sigc::mem_fun(*this,
				&FTWidget::on_button_quit));

	//设置默认的按钮状态
	m_Button_Continue->set_sensitive(false);
	m_Button_Stop->set_sensitive(false);
	m_Button_Delete->set_sensitive(false);
	//Create the TreeModel;
	m_refTreeModel = Gtk::ListStore::create(m_columns);
	m_TreeView.set_model(m_refTreeModel);

	//Add the TreeView's view columns;
	m_TreeView.append_column("",m_columns.m_icon);
	
	//Display a progress bar instread of a decimal number:
	Gtk::CellRendererProgress* cell = new Gtk::CellRendererProgress;
	int cols_count = m_TreeView.append_column(_("percent"),*cell);
	Gtk::TreeViewColumn* pColumn = m_TreeView.get_column(cols_count - 1);
	if(pColumn)
	{
#ifdef GLIBMM_PROPERTIES_ENABLED
		pColumn->add_attribute(cell->property_value(), m_columns.m_percent);
#else
		pColumn->add_attribute(*cell,"value",m_columns.m_percent);
#endif
	}
	
	m_TreeView.append_column(_("Filename"),m_columns.m_filename);
	m_TreeView.append_column_numeric(_("size"),m_columns.m_size, "%010d");
	

	show_all();
}

FTWidget::~FTWidget()
{

}

bool FTWidget::on_delete_event(GdkEventAny*)
{
	m_parent->on_fileXer_close(this);
}
void FTWidget::on_button_quit()
{
	m_parent->on_fileXer_close(this);
}

void FTWidget::on_button_del()
{
}

void FTWidget::on_button_stop()
{
}

void FTWidget::on_button_continue()
{
}




bool FTWidget::on_key_press_event(GdkEventKey* ev)
{
        if (ev->type != GDK_KEY_PRESS)
                return Gtk::Window::on_key_press_event(ev);

        switch (ev->keyval) {

        case GDK_Escape:
		on_button_quit();
                break;

        case GDK_Return:
        case GDK_KP_Enter:
                break;

        default:
                return Gtk::Window::on_key_press_event(ev);
        }

        return true;
}


