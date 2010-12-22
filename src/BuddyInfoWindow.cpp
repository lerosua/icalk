#include <glib/gi18n.h>
#include <sigc++/connection.h>
#include "BuddyInfoWindow.h"

BuddyInfoWindow::BuddyInfoWindow(Buddy* buddy_): buddy(buddy_)
{
		GBuilderXML  info_xml = Gtk::Builder::create_from_file(main_ui, "vbox_info");
        Gtk::Window* infowin = this;
        infowin->set_default_size(530, 400);
        Gtk::VBox* vBox = 0;
		info_xml->get_widget("vbox_info", vBox);

        infowin->add(*vBox);

        Gtk::Label * label = 0;
		info_xml->get_widget("label_vcard", label);

        label->set_use_markup(true);

        std::string name = buddy->get_jid();

        char *marktext =
                g_markup_printf_escaped
                (_("<span weight='heavy' color='black'>%s 's information</span>"),
                 name.c_str());

        label->set_label(marktext);

        g_free(marktext);

        infowin->set_title(_("Friend's information"));


        Glib::RefPtr < Gdk::Pixbuf > logo;

        buddy->refreshVcard();

        logo = buddy->getLogo();

        Gtk::VBox * vbox = 0;
		info_xml->get_widget("vBox", vbox);

        infoBox = new MsgBox();

        vbox->pack_end(*infoBox);

        Gtk::Button* btclose = 0;
		info_xml->get_widget("button_close", btclose);

        btclose->signal_clicked().connect(sigc::mem_fun(*this,
                                          &BuddyInfoWindow::on_btclose_clicked));

        const JID & jid = buddy->getJID();

        std::string msg_;

        /**get the resources*/
        std::string resources_ = _("resources: ") + buddy->getResource() + "\n";

        const std::string & server_ = jid.server();

        const std::string & name_ = jid.username();

        int type_ = buddy->getType();

        switch (type_) {

        case TYPE_FRIEND:
                msg_ = resources_ + "\n" + _("server: ") + server_ +
                       "\n" + _("full name: ") + name_ + "\n";
                break;

        case TYPE_TRANPORT:
                msg_ = _(" Type: Transport\n") +
                       resources_ + "\n" + _("server: ") + server_ +
                       "\n";

                break;

        case TYPE_GROUPCHAT:
                msg_ = _("Type: GroupChat\n") +
                       resources_ + "\n" + _("server: ") + server_ +
                       "\n" + _("full name: ") + name_ + "\n";
                break;

        case TYPE_BOT:
                msg_ = _("Type: Bot\n") +
                       resources_ + "\n" + _("server: ") + server_ +
                       "\n" + _("full name: ") + name_ + "\n";
                break;

        case TYPE_MSN:
                msg_ = _("Type: MSN\n") +
                       resources_ + "\n" + _("server: ") + server_ +
                       "\n" + _("full name: ") + name_ + "\n";
                break;

        case TYPE_OTHER:
                msg_ = _("Type: Other\n") +
                       resources_ + "\n" + _("server: ") + server_ +
                       "\n" + _("full name: ") + name_ + "\n";
                break;

        default:
                break;
        }

        infoBox->showGrayMsg(msg_);

        std::string nickname = buddy->get_nickname();
        msg_ = _("nick name: ") + nickname + "\n";
        infoBox->showGrayMsg(msg_);

        const std::string & sign_msg_ = buddy->get_sign_msg();
        msg_ = _("Status Message: ") + sign_msg_ + "\n";
        infoBox->showGrayMsg(msg_);

        Gtk::Image * image = Gtk::manage(new Gtk::Image(logo));
        infoBox->showGrayMsg(_("picture ："));
        infoBox->insertImage(image);
        infowin->show_all();

}

BuddyInfoWindow::~BuddyInfoWindow()
{
        delete infoBox;
}

void BuddyInfoWindow::on_btclose_clicked()
{
        delete this;
}

bool BuddyInfoWindow::on_key_press_event(GdkEventKey* ev)
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
