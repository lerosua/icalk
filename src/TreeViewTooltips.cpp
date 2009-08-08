/*
* =====================================================================================
*
*       Filename:  TreeViewTooltips.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2007年10月09日 20时07分21秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#include "TreeViewTooltips.h"
#include "pixmaps.h"
#include "BuddyView.h"

TreeViewTooltips::TreeViewTooltips(BuddyView* view): m_buddyview(view)
                , Window(Gtk::WINDOW_POPUP)
{
        this->set_decorated(false);
        this->set_skip_pager_hint(true);
        this->set_skip_taskbar_hint(true);
        this->set_name("gtk-tooltips");
        this->set_position(Gtk::WIN_POS_MOUSE);
        this->set_resizable(false);
        this->set_border_width(4);
        this->set_app_paintable(true);
        this->property_can_focus() = false;

        m_label = Gtk::manage(new Gtk::Label());
        m_label->set_line_wrap(true);
        m_label->set_alignment(0.5, 0.5);
        m_label->set_use_markup(true);

        m_avatar = Gtk::manage(new Gtk::Image());
        Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox());
        Gtk::Image* logo = getImage("default.png");
        hbox->pack_start(*logo);
        hbox->pack_start(*m_label);
        hbox->pack_start(*m_avatar);
        hbox->show_all();

        add
                (*hbox);

        add_events(Gdk::POINTER_MOTION_MASK);

        this->signal_expose_event().connect(sigc::mem_fun(*this,
                                            &TreeViewTooltips::on_expose_event));

	/*
        this->signal_motion_notify_event().connect(sigc::mem_fun(
                                *this, &TreeViewTooltips::on_motion_event), false);

        this->signal_leave_notify_event().connect(sigc::mem_fun(
                                *this, &TreeViewTooltips::on_leave_event), false);
	*/

}

bool TreeViewTooltips::on_expose_event(GdkEventExpose* ev)
{
        int width, height;
        this->get_size(width, height);
        //Gtk::Requisition req = this->size_request();
        Glib::RefPtr<Gtk::Style> style = this->get_style();
        Gtk::Widget* widget = dynamic_cast<Gtk::Widget*>(this);
        const Glib::RefPtr<Gdk::Window> window = this->get_window();
        const Gdk::Rectangle* area = 0;
        //style->paint_flat_box(window, Gtk::STATE_NORMAL, Gtk::SHADOW_OUT,
        //                      *area, *widget, "tooltip", 0, 0, width, height);
        style->paint_flat_box(window, Gtk::STATE_NORMAL, Gtk::SHADOW_OUT,
                              *area, *widget, "tooltip", 0, 0, -1, 1 );
        style->paint_flat_box(window, Gtk::STATE_NORMAL, Gtk::SHADOW_OUT,
                              *area, *widget, "tooltip", 0, height - 1, -1, 1);
        style->paint_flat_box(window, Gtk::STATE_NORMAL, Gtk::SHADOW_OUT,
                              *area, *widget, "tooltip", 0, 0, 1, -1);
        style->paint_flat_box(window, Gtk::STATE_NORMAL, Gtk::SHADOW_OUT,
                              *area, *widget, "tooltip", width - 1, 0, 1, -1);
        return Gtk::Window::on_expose_event(ev);
}

bool TreeViewTooltips::on_motion_event(GdkEventMotion* ev)
{
        Gtk::TreeModel::Path path;
        Gtk::TreeViewColumn* column;
        int cell_x, cell_y;

        if ( m_buddyview->get_path_at_pos((int) ev->x, (int) ev->y, path, column, cell_x, cell_y) ) {
                hideTooltip();
                return 0;
        }
}


bool TreeViewTooltips::on_leave_event(GdkEventCrossing* ev)
{
        hideTooltip();
        return 0;
}

void TreeViewTooltips::hideTooltip()
{
        this->hide();
}

void TreeViewTooltips::showTooltip(GdkEventMotion* ev)
{
        int x_root, y_root, origY;
        int width, height;
        int x, y;
        int s_width, s_height;
        int currentX, currentY;
        m_buddyview->get_pointer(currentX, currentY);

        x_root = (int)ev->x_root;
        y_root = (int)ev->y_root;
        origY = (int)ev->y;
        this->get_size(width, height);
        s_width = this->get_screen()->get_width();
        s_height = this->get_screen()->get_height();

        x = x_root - width / 2;

        if (currentY >= origY)
                y = y_root + 24;
        else
                y = y_root + 6;

        //check if over the screen
        if (x + width > s_width)
                x = s_width - width;
        else if (x < 0)
                x = 0;

        if (y + height > s_height)
                y = y - height - 24;
        else if (y < 0)
                y = 0;

        this->move(x, y);

        this->show_all();

}

