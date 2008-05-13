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
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#include <glib/gi18n.h>
#include "FTWidget.h"

FTWidget::FTWidget()
{
	set_title(_("File Translate"));
	set_border_width(5);
	set_default_size(400,200);
	//m_VBox= Gtk::manage(new Gtk::VBox());
	//m_ScrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());

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

	//Create the TreeModel;
	m_refTreeModel = Gtk::ListStore::create(m_columns);
	m_TreeView.set_model(m_refTreeModel);








