#include <glib/gi18n.h>
#include <fstream>
#include <iostream>
#include "StatusMsgWidget.h"
#include "Unit.h"
#include "Bodies.h"


StatusMsgWidget::StatusMsgWidget(MainWindow * parent_): parent(parent_)
                , Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{
        GlademmXML vbox_xml =
                Gnome::Glade::Xml::create(statusmsg_ui, "statusMsgBox");
        Gtk::VBox * vBox =
                dynamic_cast <
                Gtk::VBox * > (vbox_xml->get_widget("statusMsgBox"));

        add
                (*vBox);

        set_transient_for(*parent);

        msgline = Gtk::manage(new class MsgLine);

        msgline->unset_flags(Gtk::HAS_FOCUS);

        msgline->init();

        Gtk::ScrolledWindow * scrolledwin =

                Gtk::manage(new class Gtk::ScrolledWindow());

        scrolledwin->set_policy(Gtk::POLICY_AUTOMATIC,
                                Gtk::POLICY_AUTOMATIC);

        scrolledwin->add
        (*msgline);

        //scrolledwin->set_size_request(120,-1);

        Gtk::Frame * frameline = Gtk::manage(new class Gtk::Frame());

        frameline->set_shadow_type(Gtk::SHADOW_IN);

        frameline->add
        (*scrolledwin);

        vBox->pack_start(*frameline);

        Gtk::Button * btok =
                dynamic_cast <
                Gtk::Button * > (vbox_xml->get_widget("button_ok"));

        Gtk::Button * btcancel =
                dynamic_cast <
                Gtk::Button * > (vbox_xml->get_widget("button_cancel"));

        Gtk::Button* btadd =
                dynamic_cast <
                Gtk::Button * > (vbox_xml->get_widget("button_add"));

        Gtk::Button* btdelete =
                dynamic_cast <
                Gtk::Button * > (vbox_xml->get_widget("button_del"));

        Gtk::Button* btedit =
                dynamic_cast <
                Gtk::Button * > (vbox_xml->get_widget("button_edit"));

        timeoutButton =
                dynamic_cast <
                Gtk::CheckButton* > (vbox_xml->get_widget("checkbutton_timeout"));

        btok->signal_clicked().
        connect(sigc::mem_fun(*this, &StatusMsgWidget::on_button_ok));

        btcancel->signal_clicked().
        connect(sigc::
                mem_fun(*this, &StatusMsgWidget::on_button_cancel));

        btadd->signal_clicked().
        connect(sigc::mem_fun(*this, &StatusMsgWidget::on_button_add));

        btdelete->signal_clicked().
        connect(sigc::mem_fun(*this, &StatusMsgWidget::on_button_del));

        btedit->signal_clicked().
        connect(sigc::mem_fun(*this, &StatusMsgWidget::on_button_edit));

        timeoutButton->signal_clicked().
        connect(sigc::mem_fun(*this, &StatusMsgWidget::on_checkbutton_clicked));

        if (parent->isMsgTimeout())
                timeoutButton->set_active(true);

        set_default_size(530, 400);

        set_title(_("Status Message Manager"));

        show_all();
}

StatusMsgWidget::~StatusMsgWidget()
{}

void StatusMsgWidget::on_button_ok()
{
        msgline->Save();
        parent->on_btstatusmsgmanager_close(this);
        //delete this;
}

void StatusMsgWidget::on_button_cancel()
{
        parent->on_btstatusmsgmanager_close(this);
        //delete this;
}

void StatusMsgWidget::on_button_add()
{

        Glib::RefPtr < Gnome::Glade::Xml >
        addDialog_xml =
                Gnome::Glade::Xml::create(statusmsg_ui, "dialog_add");

        auto_ptr < Gtk::Dialog > addMsgDialog(dynamic_cast <
                                              Gtk::Dialog *
                                              > (addDialog_xml->
                                                 get_widget
                                                 ("dialog_add")));
        addMsgDialog->raise();
        int result = addMsgDialog->run();

        switch (result) {
        case (Gtk::RESPONSE_OK): {
                        Gtk::TextView* textview = dynamic_cast <
                                                  Gtk::TextView* > (addDialog_xml->
                                                                    get_widget("textview_add"));
                        Glib::ustring text = textview->get_buffer()->get_text();

                        if (text.empty())
                                return ;

                        msgline->addLine(text);

                        msgline->Save();

                }

                break;

        case Gtk::RESPONSE_CANCEL:
                break;

        default:
                break;
        }

}



void StatusMsgWidget::on_button_del()
{
        Glib::RefPtr < Gtk::TreeSelection > selection =
                msgline->get_selection();

        if (!selection->count_selected_rows())
                return ;

        Gtk::TreeModel::iterator iter = selection->get_selected();

        msgline->delLine(iter);

}

void StatusMsgWidget::on_button_edit()
{
        Glib::RefPtr < Gtk::TreeSelection > selection =
                msgline->get_selection();

        if (!selection->count_selected_rows())
                return ;

        Gtk::TreeModel::iterator iter = selection->get_selected();

        Glib::RefPtr < Gnome::Glade::Xml >
        addDialog_xml =
                Gnome::Glade::Xml::create(statusmsg_ui, "dialog_add");


        auto_ptr < Gtk::Dialog > addMsgDialog(dynamic_cast <
                                              Gtk::Dialog *
                                              > (addDialog_xml->
                                                 get_widget
                                                 ("dialog_add")));

        addMsgDialog->set_title(_("Edit Message"));

        Gtk::Label* label = dynamic_cast<Gtk::Label*>(addDialog_xml->
                            get_widget("label_addMsg"));

        label->set_text(_("\n  Edit the Status Message by here   \n"));

        Gtk::TextView* textview = dynamic_cast <
                                  Gtk::TextView* > (addDialog_xml->
                                                    get_widget("textview_add"));

        Glib::ustring text;

        text = msgline->getLineMsg(iter);

        textview->get_buffer()->set_text(text);

        addMsgDialog->raise();

        int result = addMsgDialog->run();

        switch (result) {
        case (Gtk::RESPONSE_OK): {

                        text = textview->get_buffer()->get_text();
                        int num = msgline->getLineNumber(iter);
                        msgline->editLine(num, text);

                }

                break;

        case Gtk::RESPONSE_CANCEL:
                break;

        default:
                break;
        }

}

void StatusMsgWidget::on_checkbutton_clicked()
{
        if (timeoutButton->get_active())
                parent->addStatusMsgTimeout();
        else
                parent->delStatusMsgTimeout();
}

bool StatusMsgWidget::on_key_press_event(GdkEventKey * ev)
{
        if (ev->type != GDK_KEY_PRESS)
                return Gtk::Window::on_key_press_event(ev);

        switch (ev->keyval) {

        case GDK_Escape:
                on_button_cancel();
                break;

        default:
                return Gtk::Window::on_key_press_event(ev);
        }

        return true;
}

MsgLine::MsgLine(): number(0)
{
        MsgLine *msgline = this;
        msgline->set_name("icalk_statusmsg_listview");
        msgline->set_flags(Gtk::CAN_FOCUS);
        msgline->set_headers_visible(false);
        msgline->set_rules_hint(false);

        m_liststore = Gtk::ListStore::create(columns);
        msgline->set_model(m_liststore);
        msgline->append_column("number", columns.num);
        msgline->append_column("message", columns.message);
        msgline->show();
}

void MsgLine::init()
{
        char buf[512];
        snprintf(buf, 512, "%s/StatusMsgFile", GUnit::getUserPath());
        std::ifstream msgfile(buf);

        if (!msgfile) {
                std::ofstream outfile(buf);
                outfile.close();
                msgfile.open(buf);
        }

        std::string line;

        if (msgfile) {
                while (getline(msgfile, line)) {
                        number++;
                        addLine(number, line);
                }
        }

        msgfile.close();
}

void MsgLine::Save()
{
        char buf[512];
        snprintf(buf, 512, "%s/StatusMsgFile", GUnit::getUserPath());
        std::ofstream msgfile(buf);

        if (!msgfile) {
                std::ofstream outfile(buf);
                outfile.close();
                msgfile.open(buf);
        }

        Glib::ustring strline;

        if (msgfile) {
                Gtk::TreeModel::Children children = m_liststore->children();
                Gtk::TreeModel::iterator iter = children.begin();

                for (; iter != children.end(); iter++) {
                        strline = (*iter)[columns.message];
                        msgfile << strline;
                        msgfile << std::endl;

                }

        }

        msgfile.close();
}

Gtk::TreeModel::iterator MsgLine::getListIter(Gtk::TreeModel::
                Children children,
                const int id)
{
        /*查找好友列表项 */
        return find_if(children.begin(),
                       children.end(),
                       bind2nd(CompareMsg(columns), id));
}

Glib::ustring MsgLine::getLineMsg(Gtk::TreeModel::iterator iter)
{
        return (*iter)[columns.message];
}

int MsgLine::getLineNumber(Gtk::TreeModel::iterator iter)
{
        return (*iter)[columns.num];
}

void MsgLine::addLine(int num, const Glib::ustring & msg)
{
        Gtk::TreeModel::iterator iter = m_liststore->append();
        (*iter)[columns.num] = num;
        (*iter)[columns.message] = msg;
}

void MsgLine::addLine(const Glib::ustring & msg)
{
        int num = number + 1;
        Gtk::TreeModel::iterator iter = m_liststore->append();
        (*iter)[columns.num] = num;
        (*iter)[columns.message] = msg;
}

void MsgLine::editLine(const int num, const Glib::ustring& msg)
{
        Gtk::TreeModel::Children children = m_liststore->children();
        Gtk::TreeModel::iterator listiter;
        listiter = getListIter(children, num);

        if (listiter == children.end())
                return ;

        (*listiter)[columns.message] = msg;
}

void MsgLine::delLine(const int num)
{
        Gtk::TreeModel::Children children = m_liststore->children();
        Gtk::TreeModel::iterator listiter;
        listiter = getListIter(children, num);

        if (listiter == children.end())
                return ;

        m_liststore->erase(listiter);
}

void MsgLine::delLine(Gtk::TreeModel::iterator iter)
{
        m_liststore->erase(iter);
}

bool MsgLine::on_button_press_event(GdkEventButton * ev)
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
                Glib::ustring msg = (*iter)[columns.message];
                Bodies::Get_Bodies().get_main_window().setStatusMsg(msg);

        } else if ((ev->type == GDK_BUTTON_PRESS)
                && (ev->button == 3)) {}

}
