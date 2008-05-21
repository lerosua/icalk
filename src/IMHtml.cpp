/*
 * =====================================================================================
 *
 *       Filename:  IMHtml.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月20日 13时17分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include "IMHtml.h"

IMHtml::IMHtml():edit_bold(false)
		,edit_italic(false)
		,edit_underline(false)
		,edit_strike(false)
		,fontsize(0)
		,editable(false);
{
        set_pixels_below_lines(5);
        set_pixels_above_lines(2);
        set_left_margin(2);
        set_right_margin(2);
        set_wrap_mode(Gtk::WRAP_WORD_CHAR);

	m_buffer =
                Gtk::TextBuffer::create(taglist.getTagTable());
        set_buffer(m_buffer);


	m_butter->signal_insert().connect(
                sigc::mem_fun(*this, &IMHtml::on_preinsert_text));
	m_butter->signal_insert_child_anchor().connect(
                sigc::mem_fun(*this, &IMHtml::on_insert_anchor));

        set_name("icalk_msg_textview");
        show_all();
}

IMHtml::clear_format()
{
	if(!editable)
		return;
        Gtk::TextBuffer::const_iterator start, end;
        m_buffer->get_bounds(start, end);
	m_buffer->remove_all_tags(start,end);

		edit_bold=false;
		edit_italic=false;
		edit_underline=false;
		edit_strike=false;
		forecolor="";
		backcolor="";
		background="";
		fontface="";
		fontsize=0;
}

#define VALID_TAG(x) if(( len=sub.uppercase().find( x ">")!=Glib::ustring::npos)) { \
	f_len = len;								\
	f_tag=sub;								\
	return true;								\
}   \  
f_type++

#define VALID_OPT_TAG(x) if(( len=sub.uppercase().find( x " ")!=Glib::ustring::npos)) { \
		if(( len=sub.uppercase().find( ">")!=Glib::ustring::npos))  \
		f_tag=sub;								\
	return true;								\
}   \
		f_type++

bool IMHtml::what_is_tag(Glib::ustring::const_iterator f_iter,Glib::ustring::const_iterator f_last,Glib::ustring& f_tag, gint& f_len,Glib::ustring& f_type)
{

	int len;
	Glib::ustring tmp(f_iter,f_last);
	size_t pos = tmp.find_first_of(">");
	if(pos == Glib::ustring::npos)
		return false;
	Glib::ustring sub = tmp.substr(0,pos);
	VALID_TAG ("B");
	VALID_TAG ("BOLD");
	VALID_TAG ("/B");
	VALID_TAG ("/BOLD");
	VALID_TAG ("I");
	VALID_TAG ("ITALIC");
	VALID_TAG ("/I");
	VALID_TAG ("/ITALIC");
	VALID_TAG ("U");
	VALID_TAG ("UNDERLINE");
	VALID_TAG ("/U");
	VALID_TAG ("/UNDERLINE");
	VALID_TAG ("S");
	VALID_TAG ("STRIKE");
	VALID_TAG ("/S");
	VALID_TAG ("/STRIKE");
	VALID_TAG ("SUB");
	VALID_TAG ("/SUB");
	VALID_TAG ("SUP");
	VALID_TAG ("/SUP");
	VALID_TAG ("PRE");
	VALID_TAG ("/PRE");
	VALID_TAG ("TITLE");
	VALID_TAG ("/TITLE");
	VALID_TAG ("BR");
	VALID_TAG ("HR");
	VALID_TAG ("/FONT");
	VALID_TAG ("/A");
	VALID_TAG ("P");
	VALID_TAG ("/P");
	VALID_TAG ("H3");
	VALID_TAG ("/H3");
	VALID_TAG ("HTML");
	VALID_TAG ("/HTML");
	VALID_TAG ("BODY");
	VALID_TAG ("/BODY");
	VALID_TAG ("FONT");
	VALID_TAG ("HEAD");
	VALID_TAG ("/HEAD");
	VALID_TAG ("BINARY");
	VALID_TAG ("/BINARY");

	VALID_OPT_TAG ("HR");
	VALID_OPT_TAG ("FONT");
	VALID_OPT_TAG ("BODY");
	VALID_OPT_TAG ("A");
	VALID_OPT_TAG ("IMG");
	VALID_OPT_TAG ("P");
	VALID_OPT_TAG ("H3");
	VALID_OPT_TAG ("HTML");

	VALID_TAG ("CITE");
	VALID_TAG ("/CITE");
	VALID_TAG ("EM");
	VALID_TAG ("/EM");
	VALID_TAG ("STRONG");
	VALID_TAG ("/STRONG");

	VALID_OPT_TAG ("SPAN");
	VALID_TAG ("/SPAN");
	VALID_TAG ("BR/"); /* hack until gtkimhtml handles things better */
	VALID_TAG ("IMG");
	VALID_TAG("SPAN");
	VALID_OPT_TAG("BR");

	f_type = -1;
	f_len = pos;
	f_tag = sub;

}

void IMHtml::showMessage(const XHtmlIM* x_msg, const std::string& f_time)
{

	Tag* body = x_msg->tag()->findChild("body");
	DLOG("body normal text = %s\n",body->xml().c_str());
	const std::string c_body = body->xml();


}
void IMHtml::showMessage(const Glib::ustring& f_msg, const std::string& f_time)
{
        Glib::ustring::const_iterator first = f_msg.begin();
        Glib::ustring::const_iterator last = f_msg.end();
        Glib::ustring::const_iterator iter = first;
        int key_len = 0;
	int tlen=0;
	int type=0;

        while (first <= last) {
		if(*iter == '<' && what_is_tag(iter,last,tag, tlen,type))
		{
			for(int i = 0;i<tlen;i++)
				iter++;
			switch(type)
			{
				case 1: /*B */
				case 2: /*BOLD*/
				case 54: /*STRONG*/

					break;
				case 3:		/* /B */
				case 4:		/* /BOLD */
				case 55:	/* /STRONG */

					break;

				case 5:		/* I */
				case 6:		/* ITALIC */
				case 52:	/* EM */

					break;
				case 7:		/* /I */
				case 8:		/* /ITALIC */
				case 53:	/* /EM */
					break;
				case 9:		/* U */
				case 10:	/* UNDERLINE */

					break;
				case 11:	/* /U */
				case 12:	/* /UNDERLINE */
					break;

				case 13:	/* S */
				case 14:	/* STRIKE */
					break;
				case 15:	/* /S */
				case 16:	/* /STRIKE */
					break;
				case 17:	/* SUB */
					break;
				case 18:	/* /SUB */
					break;
				case 19:	/* SUP */
					break;
				case 20:	/* /SUP */
					break;
				case 21:	/* PRE */
					break;
				case 22:	/* /PRE */
					break;
				case 23:	/* TITLE */
					break;
				case 24:	/* /TITLE */
					break;

				case 25:	/* BR */
				case 58:	/* BR/ */
				case 61:	/* BR (opt) */
					break;
				case 26:        /* HR */
				case 42:        /* HR (opt) */
					break;
				case 27:	/* /FONT */
					break;
				case 28:        /* /A    */
					break;

				case 29:	/* P */
				case 30:	/* /P */
				case 31:	/* H3 */
				case 32:	/* /H3 */
				case 33:	/* HTML */
				case 34:	/* /HTML */
				case 35:	/* BODY */
					break;
				case 36:	/* /BODY */
					break;
				case 37:	/* FONT */
				case 38:	/* HEAD */
				case 39:	/* /HEAD */
				case 40:	/* BINARY */
				case 41:	/* /BINARY */
					break;
				case 43:	/* FONT (opt) */
					break;
				case 44:	/* BODY (opt) */
					break;
				case 45:	/* A (opt) */
					break;
				case 46:	/* IMG (opt) */
				case 59:	/* IMG */
					break;
				case 47:	/* P (opt) */
				case 48:	/* H3 (opt) */
				case 49:	/* HTML (opt) */
				case 50:	/* CITE */
				case 51:	/* /CITE */
				case 56:	/* SPAN (opt) */
					/* Inline CSS Support - Douglas Thrift
					 *
					 * color
					 * background
					 * font-family
					 * font-size
					 * text-decoration: underline
					 * font-weight: bold
					 * direction: rtl
					 * text-align: right
					 *
					 * TODO:
					 * background-color
					 * font-style
					 */
				case 57:	/* /SPAN */
					break;
				case 60:    /* SPAN */
					break;
				case 62:	/* comment */
					break;
				default:
					break;

		}



	}
		else if


}
