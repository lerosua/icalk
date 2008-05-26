/*
* =====================================================================================
*
*       Filename:  htmltextview.h
*
*    Description:  XHTMLIM for textview support
*
*        Version:  1.0
*        Created:  2008年03月10日 14时53分52秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/
#ifndef HTLMTEXTVIEW_H_
#define HTLMTEXTVIEW_H_

#include <string>
#include <gtkmm/textview.h>
#include <gtkmm/image.h>


class IMHtml: public Gtk::TextView
{

public:
        IMHtml();
        ~IMHtml();

        void url_clicked();
        void toggle_format();
        void clear_format();

        void insert_link();
        void insert_smiley();
        void font_set_size(gint size);

        void showMessage(const Glib::ustring& f_msg, const std::string& f_time);

private:
        Glib::RefPtr < Gtk::TextBuffer > m_buffer;

        bool editable;
        bool edit_bold;
        bool edit_italic;
        bool edit_underline;
        bool edit_strike;
        std::string forecolor;
        std::string backcolor;
        std::string background;
        std::string fontface;
        int fontsize;










};

#endif

