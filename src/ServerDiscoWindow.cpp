/*
 * =====================================================================================
 *
 *       Filename:  ServerDiscoWindow.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年09月30日 16时02分04秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */
#include "ServerDiscoWindow.h"
#include "Bodies.h"


ServerDiscoWindow::ServerDiscoWindow(MainWindow * parent_):parent(parent_)
{
	server_discovery_xml = Gnome::Glade::Xml::create(server_discovery_ui,"vBox");
	Gtk::VBox* vBox=dynamic_cast<Gtk::VBox*>(server_discovery_xml->get_widget("vBox"));
	add(*vBox);
	set_transient_for(*parent);

	nodeEntry = dynamic_cast<Gtk::ComboBoxEntry*>(server_discovery_xml->get_widget("address_comboboxentry"));
	Gtk::Button* btGo    = dynamic_cast<Gtk::Button*>(server_discovery_xml->get_widget("browse_button"));
	btGo->signal_clicked().
		connect(sigc::mem_fun(*this,&ServerDiscoWindow::on_btGo_clicked));

	Gtk::Button* btClose = dynamic_cast<Gtk::Button*>(server_discovery_xml->get_widget("close_button"));
	btClose->signal_clicked().
	    connect(sigc::mem_fun(*this, &ServerDiscoWindow::on_btclose_clicked));

	show_all();
}

ServerDiscoWindow::~ServerDiscoWindow()
{}

void ServerDiscoWindow::on_btGo_clicked()
{

	std::string node=nodeEntry->get_entry()->get_text();
	Bodies::Get_Bodies().disco_node(node);

}

void ServerDiscoWindow::on_btclose_clicked()
{
	delete this;
}
bool ServerDiscoWindow::on_key_press_event(GdkEventKey* ev)
{
if (ev->type != GDK_KEY_PRESS) 
	return Gtk::Window::on_key_press_event(ev);
	switch(ev->keyval) {
	    case GDK_Escape: 
		    on_btclose_clicked();
		    break;
	    default:
		    return Gtk::Window::on_key_press_event(ev);
	}
return true;
}
