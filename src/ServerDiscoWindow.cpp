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

const char* agent_type_info[19][3] = {
                                              { "aim", _("AIM Transports"), "aim.png"}
                                              , { "icq", _("ICQ Transports"), "icq.png"}
                                              , { "msn", _("MSN Transports"), "msn.png" }
                                              , { "yahoo", _("Yahoo Transports"), "yahoo.png" }
                                              , { "qq", _("QQ Transports"), "qq.png" }
                                              , { "gadu-gadu", _("Gadu-Gadu Transports"), "gadu.png"}
                                              , { "irc", _("IRC Transports"), "irc.png" }
                                              , { "conference", _("Conference"), "groupchat.png" }
                                              , { "rss", _("RSS Transports"), "rss.png" }
                                              , { "weather", _("Weather"), "other.png" }
                                              , { "sip", _("SIP"), "other.png" }
                                              , { "bytestreams", _("Socks5 proxy"), "bytestreams.png" }
                                              , { "pubsub", _("PubSub"), "pubsub.png" }
                                              , { "http-ws" , _("Http-ws"), "other.png" }
                                              , { "sms", _("SMS Transports"), "sms.png" }
                                              , { "smtp", _("SMTP Transports"), "mail.png" }
                                              , { "user", _("User"), "jud.png" }
                                              , { "other", "", "other.png" }
};

ServerDiscoWindow::ServerDiscoWindow(MainWindow * parent_): m_parent(parent_)
                , Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{
        server_discovery_xml = Gnome::Glade::Xml::create(server_discovery_ui, "vBox");
        Gtk::VBox* vBox = dynamic_cast<Gtk::VBox*>(server_discovery_xml->get_widget("vBox"));

        add
                (*vBox);

        set_transient_for(*m_parent);

        m_label = dynamic_cast<Gtk::Label*>(server_discovery_xml->get_widget("banner_agent_label"));

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
        int f_type;

        if (0 == f_jid.find("msn."))
                f_type = AGENT_MSN;
        else if (0 == f_jid.find("aim."))
                f_type = AGENT_AIM;
        else if (0 == f_jid.find("icq."))
                f_type = AGENT_ICQ;
        else if (0 == f_jid.find("yahoo."))
                f_type = AGENT_YAHOO;
        else if (0 == f_jid.find("qq."))
                f_type = AGENT_QQ;
        else if (0 == f_jid.find("gadu-gadu."))
                f_type = AGENT_GADU;
        else if (0 == f_jid.find("irc."))
                f_type = AGENT_IRC;
        else if (0 == f_jid.find("conference.") || 0 == f_jid.find("chat."))
                f_type = AGENT_CONFERENCE;
        else if (0 == f_jid.find("rss."))
                f_type = AGENT_RSS;
        else if (0 == f_jid.find("weather."))
                f_type = AGENT_WEATHER;
        else if (0 == f_jid.find("sip."))
                f_type = AGENT_SIP;
        else if (0 == f_jid.find("proxy65."))
                f_type = AGENT_BYTESTREAMS;
        else if (0 == f_jid.find("pubsub."))
                f_type = AGENT_PUBSUB;
        else if (0 == f_jid.find("http-ws."))
                f_type = AGENT_HTTP_WS;
        else if (0 == f_jid.find("sms."))
                f_type = AGENT_SMS;
        else if (0 == f_jid.find("smtp."))
                f_type = AGENT_SMTP;
        else if (0 == f_jid.find("users.") || 0 == f_jid.find("vjud."))
                f_type = AGENT_JUD;
        else
                f_type = AGENT_OTHER;

        agentline->addLine(f_jid, f_type);
}

void ServerDiscoWindow::setLabel(const std::string& f_node, const std::string& f_server)
{
        char* marktext = g_markup_printf_escaped("<span weight=\"heavy\" size=\"large\"> %s </span>\n %s", f_node.c_str(), f_server.c_str());
        m_label->set_label(marktext);
        g_free(marktext);
}

void ServerDiscoWindow::clear()
{
        agentline->clear();
}

void ServerDiscoWindow::final_progress()
{
        if (m_timeout.connected())
                m_timeout.disconnect();

        m_progressbar->set_fraction(1.0);
}

bool ServerDiscoWindow::on_progress()
{
        double new_val = m_progressbar->get_fraction() + 0.01;

        if (new_val > 1.0)
                new_val = 0.0;

        m_progressbar->set_fraction(new_val);

        return true;
}

void ServerDiscoWindow::showError()
{
        if (m_timeout.connected())
                m_timeout.disconnect();

        Glib::ustring msg(_("Server Discover happen error"));

        Gtk::MessageDialog dialog("Error", false,
                                  Gtk::MESSAGE_INFO);

        dialog.set_secondary_text(msg);

        dialog.run();

        return ;
}

void ServerDiscoWindow::on_btGo_clicked()
{
        clear();

        if (m_timeout.connected())
                m_timeout.disconnect();

        std::string node = m_nodeEntry->get_entry()->get_text();

        if (node.empty())
                return ;

        setLabel(node);

        Bodies::Get_Bodies().disco_node(node);

        m_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
                        &ServerDiscoWindow::on_progress), 50);

}

bool ServerDiscoWindow::on_delete_event(GdkEventAny*)
{
	on_btclose_clicked();
}
void ServerDiscoWindow::on_btclose_clicked()
{
        if (m_timeout.connected())
                m_timeout.disconnect();

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

        case GDK_Return:

        case GDK_KP_Enter:
                on_btGo_clicked();
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

        m_refTreeModel = Gtk::ListStore::create(m_columns);
        agentline->set_model(m_refTreeModel);
        agentline->append_column("icons", m_columns.m_icons);
        agentline->append_column("name", m_columns.m_name);
        agentline->append_column("jid", m_columns.m_jid);
        agentline->show();


}

void AgentLine::addLine(const std::string& f_jid, const int f_type)
{
        Gtk::TreeModel::iterator iter = m_refTreeModel->append();

        if (f_type != AGENT_OTHER)
                (*iter)[m_columns.m_name] = std::string(agent_type_info[f_type][agent_info]);
        else
                (*iter)[m_columns.m_name] = f_jid;

        (*iter)[m_columns.m_jid] = f_jid;

        Glib::RefPtr < Gdk::Pixbuf > pix = getPix(agent_type_info[f_type][agent_icon]);

        (*iter)[m_columns.m_icons] = pix;
}

void AgentLine::clear()
{
        m_refTreeModel->clear();
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
                //Glib::ustring msg = (*iter)[m_columns.m_jid];

        } else if ((ev->type == GDK_BUTTON_PRESS)
                && (ev->button == 3)) {}

}
