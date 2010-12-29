/*
* =====================================================================================
*
*       Filename:  MsgWindow.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2007年05月24日 19时43分28秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/



#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sigc++/connection.h>
#include <assert.h>
#include <glib/gi18n.h>
#include "MsgWindow.h"
#include "Bodies.h"
#include "pixmaps.h"




MsgWindow::MsgWindow()
{
	//gmm_data = new GlademmData(get_accel_group());
        msg_xml = Gtk::Builder::create_from_file(msg_ui, "msg_vbox");
		Gtk::Widget * widget = 0;
		msg_xml->get_widget("msg_vbox",widget);
		msg_xml->get_widget("nbMsg", notebook);

        notebook->signal_switch_page().
        connect(sigc::mem_fun(*this, &MsgWindow::on_switch_page));

		msg_xml->get_widget("statusbar", statusbar);

		pack_start(*widget);
        //add(*widget);

		//msg_xml->get_widget("bt_send_file", btsendfile);

		//btsendfile->signal_clicked().connect(sigc::mem_fun(*this, &MsgWindow::on_send_file));

        showTypeImage(false);
        showEncryptImage(false);
        //set_default_size(600, 400);
		//Glib::RefPtr < Gdk::Pixbuf > pix = getPix("default.png");
		//set_icon(pix);

        //hide();

}

void MsgWindow::setNewMsgTitle(bool mode)
{
        char *marktext;

        if (mode)
                marktext = _("*New Message");
        else
                marktext = _(" iCalk");

        //set_title(marktext);
}


void MsgWindow::add_page(MsgPage & page)
{
        /*如果标签页已经在NoteBook中，返回 */

        if ( -1 != notebook->page_num(page))
                return ;

        //关闭按钮
        Gtk::Button * button = Gtk::manage(new Gtk::Button());

        Gtk::Image * bimage =
                Gtk::manage(new Gtk::
                            Image(Gtk::Stock::CLOSE, Gtk::ICON_SIZE_MENU));

        button->set_image(*bimage);

        button->set_relief(Gtk::RELIEF_NONE);

        button->signal_clicked().
        connect(sigc::mem_fun(*this, &MsgWindow::on_page_close_click));

        Gtk::Image * logo = getImage("online.png");

        //     Gtk::manage(new Gtk::
        //   Image(page.get_logo().get_pixbuf()->
        //         scale_simple(16, 16, Gdk::INTERP_NEAREST)));

        Gtk::HBox * box = Gtk::manage(new Gtk::HBox());

        box->pack_start(*logo);

        box->pack_start(page.get_lable());

        box->pack_start(*button);

        box->set_size_request( -1, 20);

        notebook->append_page(page, *box);

        setCurrentPage(&page);

		//set_focus(*(page.getFocusWidge()));

        box->show_all();

        showTypeImage(false);

		//show_all();
}


MsgWindow::~MsgWindow()
{
	//delete gmm_data;
}

#if 0
bool MsgWindow::on_delete_event(GdkEventAny *)
{
        int num;

        do {
                num = notebook->get_current_page();
                MsgPage *page =
                        dynamic_cast <
                        MsgPage * > (notebook->get_nth_page(num));
                page->close();
        } while (num > 0);

        hide();
}
#endif

void MsgWindow::del_page(MsgPage & page)
{
        int num;
        num = notebook->page_num(page);
        notebook->remove_page(num);
        num = notebook->get_n_pages();

		//if (0 == num)
		//hide();

        showTypeImage(false);

        //page=NULL;
}

void MsgWindow::on_page_close_click()
{
        int num = notebook->get_current_page();
        MsgPage *page =
                dynamic_cast < MsgPage * >(notebook->get_nth_page(num));
        showTypeImage(false);
        page->close();
}

void MsgWindow::on_switch_page(GtkNotebookPage * page, guint index)
{
        showTypeImage(false);
}

void MsgWindow::showTypeImage(bool isShow)
{
        Gtk::Image * typeImage = 0;
		msg_xml->get_widget("image_type", typeImage);

        if (isShow) {
                Glib::RefPtr < Gdk::Pixbuf > pix = getPix("typed.png");

                typeImage->set
                (pix);
        } else
                typeImage->clear();
}

void MsgWindow::showEncryptImage(bool isShow)
{
        Gtk::Image * encryptImage = 0;
		msg_xml->get_widget("image_encrypt", encryptImage);

        if (isShow) {
                Glib::RefPtr < Gdk::Pixbuf > pix = getPix("crypto.png");

                encryptImage->set
                (pix);
        } else
                encryptImage->clear();
}

#if 0
bool MsgWindow::on_key_press_event(GdkEventKey * ev)
{
        MsgPage *page;

        if (ev->type != GDK_KEY_PRESS)
                return Gtk::Window::on_key_press_event(ev);

        if (ev->state & (GDK_MOD1_MASK)) {
                switch (ev->keyval) {

                case GDK_1:

                case GDK_2:

                case GDK_3:

                case GDK_4:

                case GDK_5:

                case GDK_6:

                case GDK_7:

                case GDK_8:

                case GDK_9:
                        notebook->set_current_page(ev->keyval - GDK_1);
                        break;

                default:
                        return Gtk::Window::on_key_press_event(ev);
                }
        }

        if (ev->state & (GDK_CONTROL_MASK)) {
                switch (ev->keyval) {

                case GDK_Tab:

                        if (notebook->get_current_page() + 1 ==
                                        notebook->get_n_pages())
                                notebook->set_current_page(0);
                        else
                                notebook->next_page();

                        break;

                case GDK_Left:
                        notebook->prev_page();

                        break;

                case GDK_Right:
                        notebook->next_page();

                        break;

                case GDK_L:

                case GDK_l:
                        getCurrentPage()->clear();

                        break;

                case GDK_w:
                        getCurrentPage()->deleteWord(DELETE_WORD);

                        break;

                case GDK_u:
                        getCurrentPage()->deleteWord(DELETE_LINE);

                        break;

                case GDK_h:
                        getCurrentPage()->deleteWord(DELETE_CHAR);

                        break;

                case GDK_j:
                        /*
                        case GDK_Return:
                        case GDK_KP_Enter:
                        */
                        page = getCurrentPage();

                        if (page)
                                page->sendMessage();

                        break;

                default:
                        return Gtk::Window::on_key_press_event(ev);
                }

                return true;
        } else {
                switch (ev->keyval) {

                case GDK_Escape:

                        if (!Gtk::Window::on_key_press_event(ev)) {
                                on_page_close_click();
                        }

                        break;

                case GDK_Return:

                case GDK_KP_Enter:
                        page = getCurrentPage();

                        if (page)
                                page->sendMessage();

                        break;

                default:
                        return Gtk::Window::on_key_press_event(ev);
                }
        }

        return true;

        //return Gtk::Window::on_key_press_event(ev);

}
#endif


void MsgWindow::showStatusBarMsg(const std::string& msg, unsigned int id)
{
        statusbar->pop(id);
        statusbar->push(msg, id);
}

void MsgWindow::on_send_file()
{
	MsgPage *page = getCurrentPage();
	page->on_send_file();


}
void MsgWindow::show()
{


}
