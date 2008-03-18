// generated 2005/8/8 17:10:19 CST by xihels@163.com
//

#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
//#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/connection.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gdkmm/pixbufloader.h>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/buttonbox.h>
#include "pixmaps.h"
#include "CommandBar.h"
#include "MsgPage.h"


CommandBar::CommandBar(MsgPage* _parent) :
                parent(reinterpret_cast<MsgPage*>(_parent)),
                smileyWnd(NULL)
{
        CommandBar* commandBar = this;

        Gtk::Image* image_ = getImage("insert-fonts.png");
        Gtk::ToolButton* item = Gtk::manage(new Gtk::ToolButton(*image_));
        item->signal_clicked().connect(sigc::mem_fun(*this, &CommandBar::on_fonts_clicked));
        item->set_visible_horizontal(true);
        item->set_visible_vertical(true);
        item->set_is_important(true);
        item->set_homogeneous(true);
        item->set_expand(false);
        this->pack_start(*item, Gtk::PACK_SHRINK);

        image_ = getImage("insert-link.png");
        item = Gtk::manage(new Gtk::ToolButton(*image_));
        item->signal_clicked().connect(sigc::mem_fun(*this, &CommandBar::on_link_clicked));
        item->set_visible_horizontal(true);
        item->set_visible_vertical(true);
        item->set_is_important(true);
        item->set_homogeneous(true);
        item->set_expand(false);
        this->pack_start(*item, Gtk::PACK_SHRINK);

        image_ = getImage("insert-image.png");
        item = Gtk::manage(new Gtk::ToolButton(*image_));
        item->signal_clicked().connect(sigc::mem_fun(*this, &CommandBar::on_image_clicked));
        item->set_visible_horizontal(true);
        item->set_visible_vertical(true);
        item->set_is_important(true);
        item->set_homogeneous(true);
        item->set_expand(false);
        this->pack_start(*item, Gtk::PACK_SHRINK);

        image_ = getImage("insert-smiley.png");
        item = Gtk::manage(new Gtk::ToolButton(*image_));
        item->signal_clicked().connect(sigc::mem_fun(*this, &CommandBar::on_smiley_clicked));
        item->set_visible_horizontal(true);
        item->set_visible_vertical(true);
        item->set_is_important(true);
        item->set_homogeneous(true);
        item->set_expand(false);
        this->pack_start(*item, Gtk::PACK_SHRINK);

}



CommandBar::~CommandBar()
{
        delete smileyWnd;
}

void CommandBar::on_fonts_clicked()
{
        parent->on_toolbar_fonts();
}

void CommandBar::on_link_clicked()
{
        parent->on_toolbar_link();
}

void CommandBar::on_image_clicked()
{
        parent->on_toolbar_image();
}

void CommandBar::on_smiley_clicked()
{
        if (NULL == smileyWnd)
                smileyWnd = new SmileyWnd(parent);

        smileyWnd->showWindow();
}

