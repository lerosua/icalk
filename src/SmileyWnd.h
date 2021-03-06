// generated 2005/8/22 19:17:02 CST by xihels@163.com
//


#ifndef _SMILEY_HH
#  define _SMILEY_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA
#include <gtkmm/accelgroup.h>

class GlademmData
{
        Glib::RefPtr<Gtk::AccelGroup> accgrp;

public:

        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {}

        Glib::RefPtr<Gtk::AccelGroup> getAccelGroup()
        {
                return accgrp;
        }
};

#endif //GLADEMM_DATA

#include <gtkmm/window.h>
#include <gtkmm/tooltips.h>

class MsgPage;
/**
 * @brief 笑脸表情窗口显示
 */

class SmileyWnd: public Gtk::Window
{

private:
        GlademmData *gmm_data;

public:
        SmileyWnd(MsgPage* f_page);
        ~SmileyWnd();
        bool on_key_press_event(GdkEventKey * ev);
        bool on_leave_event(GdkEventCrossing* ev);
	/** 显示表情窗口*/
        void showWindow();

private:
        void on_smiley_clicked(Glib::ustring code);

private:
        Gtk::Tooltips m_tooltips;
        MsgPage* page;
};

#endif
