/***************************************************************************
 *   Copyright (C) 2005 by xihe                                            *
 *   xihels@163.com                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _TAG_LIST_H_
#define _TAG_LIST_H_
#include <gtkmm/texttag.h>
#include <gtkmm/texttagtable.h>
#include <ext/hash_map>
#include <stdint.h>
//using __gnu_cxx::hash_map;

#define COLOR_LIST_SIZE 16
class TagList {
    public:
	typedef Glib::RefPtr<Gtk::TextTagTable> RefTagTable;
	typedef Glib::RefPtr<Gtk::TextTag> RefTag;	

	TagList(): colorid(0){}

	const RefTagTable getTagTable() const { return getTags().tagTable; }
	const RefTag getTitleColor(int  id);
	const RefTag getSelfTitleColor() const { return getTags().selfTitleColor; }
	const RefTag getTimeFont() const { return getTags().timeFont; }
	const RefTag getBoldFont() const { return getTags().boldFont; }
    private:
	struct Tags {
	    Tags();
	    RefTag	titleColorList[COLOR_LIST_SIZE];
	    RefTagTable tagTable;
	    RefTag	timeFont;
	    RefTag	boldFont;
	    RefTag	urlFont;
	    RefTag      selfTitleColor;
	};

	static Tags& getTags();
	int	colorid;
};
#endif // _TAG_LIST_H_

