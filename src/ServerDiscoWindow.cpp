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
#include <glib/gi18n.h>
#include "ServerDiscoWindow.h"
#include "Bodies.h"
#include "MainWindow.h"


ServerDiscoWindow::ServerDiscoWindow(MainWindow * parent_): m_parent(parent_)
                , Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{
        server_discovery_xml = Gnome::Glade::Xml::create(server_discovery_ui, "vBox");
        Gtk::VBox* vBox = dynamic_cast<Gtk::VBox*>(server_discovery_xml->get_widget("vBox"));

        add
                (*vBox);

        set_transient_for(*m_parent);

        m_nodeEntry = dynamic_cast<Gtk::ComboBoxEntry*>(server_discovery_xml->get_widget("address_comboboxentry"));

        Gtk::Button* btGo = dynamic_cast<Gtk::Button*>(server_discovery_xml->get_widget("browse_button"));

        btGo->signal_clicked().
        connect(sigc::mem_fun(*this, &ServerDiscoWindow::on_btGo_clicked));

        Gtk::Button* btClose = dynamic_cast<Gtk::Button*>(server_discovery_xml->get_widget("close_button"));

        btClose->signal_clicked().
        connect(sigc::mem_fun(*this, &ServerDiscoWindow::on_btclose_clicked));
	m_progressbar = dynamic_cast<Gtk::ProgressBar*>(server_discovery_xml->get_widget("services_progressbar"));


        Gtk::ScrolledWindow * scrolledwin = dynamic_cast<Gtk::ScrolledWindow*>(server_discovery_xml->get_widget("services_scrollwin"));

        agentline = Gtk::manage(new class AgentLine);

        scrolledwin->add
        (*agentline);


        set_default_size(450, 320);

        show_all();
}

ServerDiscoWindow::~ServerDiscoWindow()
{}

void ServerDiscoWindow::addAgent(const std::string& f_jid)
{
        agentline->addLine(f_jid);
}

void ServerDiscoWindow::progress(const bool f_blink)
{
	if(f_blink)
	{
		m_progressbar->pulse();
	}
	else
		m_progressbar->set_fraction(1.0);
}

void ServerDiscoWindow::on_btGo_clicked()
{

        std::string node = m_nodeEntry->get_entry()->get_text();
	if(node.empty())
		return;
        Bodies::Get_Bodies().disco_node(node);
	progress(true);

}

void ServerDiscoWindow::on_btclose_clicked()
{
        //delete this;
        m_parent->on_serverDisco_close(this);
}

bool ServerDiscoWindow::on_key_press_event(GdkEventKey* ev)
{
        if (ev->type != GDK_KEY_PRESS)
                return Gtk::Window::on_key_press_event(ev);

        switch (ev->keyval) {

        case GDK_Escape:
                on_btclose_clicked();
                break;

        default:
                return Gtk::Window::on_key_press_event(ev);
        }

        return true;
}

AgentLine::AgentLine()
{
        AgentLine *agentline = this;
        agentline->set_name("icalk_statusmsg_listview");
        agentline->set_flags(Gtk::CAN_FOCUS);
        agentline->set_headers_visible(false);
        agentline->set_rules_hint(false);

        m_liststore = Gtk::ListStore::create(m_columns);
        agentline->set_model(m_liststore);
        agentline->append_column("icon", m_columns.icon);
        agentline->append_column("name", m_columns.name);
        agentline->show();


}

void AgentLine::addLine(const std::string& f_jid, const int f_type)
{
        Gtk::TreeModel::iterator iter = m_liststore->append();
        (*iter)[m_columns.name] = f_jid;
        (*iter)[m_columns.jid] = f_jid;
}

bool AgentLine::on_button_press_event(GdkEventButton * ev)
{
        bool result = Gtk::TreeView::on_button_press_event(ev);

        Glib::RefPtr < Gtk::TreeSelection > selection =
                this->get_selection();
        Gtk::TreeModel::iterator iter = selection->get_selected();

        if (!selection->count_selected_rows())
                return result;


        Gtk::TreeModel::Path path(iter);

        Gtk::TreeViewColumn * tvc;

        int cx, cy;

        /** get_path_at_pos() 是为确认鼠标是否在选择行上点击的*/
        if (!this->
                        get_path_at_pos((int) ev->x, (int) ev->y, path, tvc, cx, cy))
                return FALSE;

        if ((ev->type == GDK_2BUTTON_PRESS ||
                        ev->type == GDK_3BUTTON_PRESS)) {
                //Glib::ustring msg = (*iter)[m_columns.jid];

        } else if ((ev->type == GDK_BUTTON_PRESS)
                && (ev->button == 3)) {}

}
