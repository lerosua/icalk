// generated 2005/8/22 19:17:02 CST by xihels@163.com
//


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x 
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/connection.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "SmileyWnd.h"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gdkmm/pixbufloader.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#include <gtkmm/table.h>

#include "MsgPage.h"
#include "MsgBox.h"

#include "pixmaps.h"

SmileyWnd::SmileyWnd(MsgPage* page_) :
                page(page_)
{
        Gtk::Window* smiley = this;
        gmm_data = new GlademmData(get_accel_group());

        Gtk::Table* table = Gtk::manage(new class Gtk::Table(10, 10, true));

        table->set_row_spacings(0);
        table->set_col_spacings(0);

        Gtk::Image* image = NULL;
        Gtk::Button* button = NULL;

        int x;
        int y;
        int index;
        SMILELIST& smilelist = page->getSmileMap();
        SMILELIST::const_iterator iter = smilelist.begin();
        Glib::ustring tip;

        for (int i = 0; iter != smilelist.end(); iter++, i++) {
                image = (*iter).second->getSmileImage();
                image->set_alignment(0.5, 0.5);
                image->set_padding(0, 0);

                button = Gtk::manage(new class Gtk::Button());
                button->set_relief(Gtk::RELIEF_NONE);

                button->add
                (*image);

                x = i % 10;

                y = i / 10;

                table->attach(*button, x, x + 1, y, y + 1,
                              Gtk::AttachOptions(), Gtk::AttachOptions(), 0, 0);

                tip = (*iter).first;

                _tooltips.set_tip(*button, tip, "");

                button->signal_clicked().connect(
                        sigc::bind<Glib::ustring>(
                                sigc::mem_fun(*this, &SmileyWnd::on_smiley_clicked), tip), false);

        }

        /*
        add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_MOTION_MASK |
         Gdk::BUTTON_PRESS_MASK | Gdk::
         BUTTON_RELEASE_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::
         LEAVE_NOTIFY_MASK);
        this->signal_leave_notify_event().
         connect(sigc::mem_fun(*this, &SmileyWnd::on_leave_event),
          false);
          */

        smiley->set_title("表情");

        smiley->property_window_position().set_value(Gtk::WIN_POS_MOUSE);

        smiley->set_resizable(false);

        smiley->set_type_hint(Gdk::WINDOW_TYPE_HINT_TOOLBAR);

        smiley->add
        (*table);

        table->show_all();
}

void SmileyWnd::on_smiley_clicked(Glib::ustring code)
{
        hide();

        if (NULL != page) {
                printf("insertSmiley code ? \n");
                page->insertSmiley(code);
        }
}

bool SmileyWnd::on_leave_event(GdkEventCrossing* ev)
{
        hide();
}

SmileyWnd::~SmileyWnd()
{
        delete gmm_data;
}


bool SmileyWnd::on_key_press_event(GdkEventKey * ev)
{
        if (ev->type != GDK_KEY_PRESS)
                return Gtk::Window::on_key_press_event(ev);

        switch (ev->keyval) {

        case GDK_Escape:
                hide();

                if (NULL != page) {
                        page = NULL;
                }

        default:

                return Gtk::Window::on_key_press_event(ev);
        }

        return 0;
}
