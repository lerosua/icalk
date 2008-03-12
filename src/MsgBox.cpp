/*
 * =====================================================================================
 *
 *       Filename:  MsgBox.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年07月17日 22时47分13秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <gtkmm/textbuffer.h>
#include <gdkmm/pixbuf.h>
#include <time.h>
#include "MsgBox.h"

MsgBox::MsgBox()
{
	set_pixels_below_lines(2);
	set_pixels_above_lines(2);
	set_left_margin(2);
	set_right_margin(2);
      set_wrap_mode(Gtk::WRAP_WORD_CHAR);

	Glib::RefPtr < Gtk::TextBuffer > buffer =
	    Gtk::TextBuffer::create(taglist.getTagTable());

	set_buffer(buffer);
	set_name("icalk_msg_textview");
	show_all();
}
void MsgBox::showMessage(Glib::ustring::const_iterator first,Glib::ustring::const_iterator last)
{
	Glib::ustring text(first,last);
	showSimpleMessage(text);
}
void MsgBox::showMessage(const Glib::ustring& msg_,const std::string& time_)
{
	Glib::ustring::const_iterator first  = msg_.begin();
	Glib::ustring::const_iterator last   = msg_.end();
	Glib::ustring::const_iterator iter   = first;
	int key_len=0;
	Smile* smile = NULL;
	while(first<=last){
		smile = smileTree.find_smile(iter, last, key_len);
		if(NULL != smile) {
			if(iter!=first)
				showMessage(first,iter);
			for(int i=0;i<=key_len;i++)
				iter++;
			first=iter;
			insertImage(smile->getSmileImage());
			continue;
		}
	
		//没有找到，直接显示
		if(iter == last)
		{
			if(first != last)
			{
			showMessage(first,last);
			}
			//showTime(time_);
			showSimpleMessage("\n");
			return;
		}
		iter++;
	}
}
		
void MsgBox::showSimpleMessage(const Glib::ustring & msg_)
{
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();
	Gtk::TextBuffer::iterator end = buffer->end();
	Gdk::Rectangle rect;
	get_visible_rect(rect);

	int y = -1;
	int height = -1;
	get_line_yrange(end, y, height);

	buffer->place_cursor(buffer->insert(end, msg_));


	if (y < rect.get_y() + rect.get_height() + 16)	// 最后一行可见，也就是用户没有向上滚动
		scroll_to_mark(buffer->get_insert(), 0);	// 插入文本后也要向下滚动，使最后一行继续可见

//	showTime();
}

void MsgBox::showSystemMsg(const std::string& msg_)
{
	showGrayMsg("\t\t\t\t");
	showGrayMsg(msg_);
	showGrayMsg("\n");
}
void MsgBox::showGrayMsg(const std::string& msg_)
{
	RefTag titlecolor = taglist.getTitleColor(3);
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();

	Gtk::TextBuffer::iterator end = buffer->end();
	Glib::RefPtr < Gtk::TextBuffer::Mark > mark =
	    buffer->create_mark(end);

	Gtk::TextBuffer::iterator first = mark->get_iter();
	buffer->apply_tag(titlecolor, first, end);

	buffer->move_mark(mark, end);
	//end = buffer->insert_with_tag(end, "\t\t\t\t\t", taglist.getBoldFont());
	end = buffer->insert_with_tag(end, msg_, taglist.getBoldFont());
	//end = buffer->insert_with_tag(end, "\n", taglist.getBoldFont());

	first = mark->get_iter();
	buffer->apply_tag(titlecolor, first, end);

	// 插入文本后也要向下滚动，使最后一行继续可见
	scroll_to_mark(buffer->get_insert(), 0);	

	buffer->delete_mark(mark);
}
void MsgBox::showTime(const std::string& time_)
{
	char timetext[255];
	if(time_.empty())
	{

	/*设置时间戳*/
	struct tm* tmw;
	time_t now;
	time(&now);
	tmw = localtime(&now);
	snprintf(timetext,32,"%02d:%02d:%02d ",tmw->tm_hour,tmw->tm_min,tmw->tm_sec);
	}
	else{
		printf("Debug in time stamp\n");
		snprintf(timetext,time_.size(),time_.c_str());
	}
	RefTag titlecolor = taglist.getTimeFont();
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();

	Gtk::TextBuffer::iterator end = buffer->end();
	Glib::RefPtr < Gtk::TextBuffer::Mark > mark =
	    buffer->create_mark(end);

	Gtk::TextBuffer::iterator first = mark->get_iter();
	buffer->apply_tag(titlecolor, first, end);

	buffer->move_mark(mark, end);

	end = buffer->insert_with_tag(end, "\t\t\t\t\t\t", taglist.getBoldFont());
	end =
	    buffer->insert_with_tag(end, Glib::ustring(timetext), taglist.getBoldFont());

	end = buffer->insert_with_tag(end, "\n", taglist.getBoldFont());
	first = mark->get_iter();
	buffer->apply_tag(titlecolor, first, end);

	buffer->delete_mark(mark);
}

void MsgBox::showTitle(const std::string & sender,bool self)
{
	/*是自己的发的消息，设置消息头颜色*/
	RefTag titlecolor;
	if(self)
	 titlecolor = taglist.getSelfTitleColor();
	else
	 titlecolor = taglist.getTitleColor(0);

	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();

	Gtk::TextBuffer::iterator end = buffer->end();
	Glib::RefPtr < Gtk::TextBuffer::Mark > mark =
	    buffer->create_mark(end);

	Gtk::TextBuffer::iterator first = mark->get_iter();
	buffer->apply_tag(titlecolor, first, end);

	buffer->move_mark(mark, end);

	end = buffer->insert_with_tag(end, sender, taglist.getBoldFont());
	end = buffer->insert_with_tag(end, ": ", taglist.getBoldFont());

	first = mark->get_iter();
	buffer->apply_tag(titlecolor, first, end);

	buffer->delete_mark(mark);

	showTime();
}

void MsgBox::readline(int type)
{
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();
	Gtk::TextBuffer::iterator first = buffer->begin();
	Gtk::TextBuffer::iterator last = buffer->end();
	switch(type){
		case DELETE_CHAR:
			last.backward_char();
			break;
		case DELETE_WORD:
			last.backward_word_start();
			break;
		case DELETE_LINE:
			backward_display_line_start(last);
			break;
		default:
			break;
	}
	Glib::ustring text = buffer->get_text(first,last);
	buffer->set_text(text);

}

void MsgBox::getText(Glib::ustring & text)
{
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();
	Gtk::TextBuffer::iterator first = buffer->begin();
	Gtk::TextBuffer::iterator last = buffer->end();
	std::string *data = NULL;
	Glib::RefPtr < Gtk::TextChildAnchor > anchor;
	gunichar c;
	for (; first != last; ++first) {
		c = first.get_char();
		if (0xFFFC == c) {
			anchor = first.get_child_anchor();
			if (anchor) {
				data =(std::string*) anchor->get_data("shortcut");
				anchor->set_data("shortcur", NULL);
				if (NULL != data) {
					text.append(*data);
					delete data;
				}
			}
		} else
			text.push_back(c);
	}
}

void MsgBox::insertImage(Gtk::Image* image)
{
	g_assert(image);
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();
	Glib::RefPtr < Gtk::TextChildAnchor > anchor = 
		buffer->create_child_anchor(buffer->end());
	add_child_at_anchor(*image,anchor);

	image->show();
}

void MsgBox::insertSmiley(const Glib::ustring& code)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = get_buffer();
	buffer->insert(buffer->end(), code);

}


void MsgBox::clear()
{
	Glib::RefPtr < Gtk::TextBuffer > buffer = get_buffer();
	Gtk::TextBuffer::iterator start,end;
	buffer->get_bounds(start,end);
	buffer->erase(start,end);
	//buffer->set_text("");
}
