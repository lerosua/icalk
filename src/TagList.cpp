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
#include "TagList.h"

namespace icalk {

TagList::Tags::Tags() {
        tagTable = Gtk::TextTagTable::create();

        timeFont = Gtk::TextTag::create();
        //timeFont->property_justification().set_value(Gtk::JUSTIFY_RIGHT);
        //timeFont->property_size().set_value(8 * Pango::SCALE);
        //timeFont->property_weight().set_value(Pango::WEIGHT_BOLD);
        timeFont->property_foreground().set_value("#AFB3B2");

        tagTable->add(timeFont);

        boldFont = Gtk::TextTag::create();

        boldFont->property_weight().set_value(Pango::WEIGHT_BOLD);

        tagTable->add(boldFont);

        urlFont = Gtk::TextTag::create();

        urlFont->property_underline().set_value(Pango::UNDERLINE_SINGLE);

        tagTable->add(urlFont);

        // #0D1CF2  深蓝 #自己消息的颜色改为 #16569E
        selfTitleColor = Gtk::TextTag::create();

        selfTitleColor->property_foreground().set_value("#16569E");

        tagTable->add(selfTitleColor);

        // #F31A17  深红 改为朱红#A82F2F
        titleColorList[0] = Gtk::TextTag::create();

        titleColorList[0]->property_foreground().set_value("#A82F2F");

        tagTable->add(titleColorList[0]);


        // #10ACD4  浅蓝
        titleColorList[1] = Gtk::TextTag::create();

        titleColorList[1]->property_foreground().set_value("#10ACD4");

        tagTable->add(titleColorList[1]);

        // #E911D1 鲜红
        titleColorList[2] = Gtk::TextTag::create();

        titleColorList[2]->property_foreground().set_value("#E911D1");

        tagTable->add(titleColorList[2]);


        // #AFB3B2  深灰
        titleColorList[3] = Gtk::TextTag::create();

        titleColorList[3]->property_foreground().set_value("#888888");

        tagTable->add(titleColorList[3]);

        // #338525  深绿
        titleColorList[4] = Gtk::TextTag::create();

        titleColorList[4]->property_foreground().set_value("#338525");

        tagTable->add(titleColorList[4]);

        // #17ED70  草绿
        titleColorList[5] = Gtk::TextTag::create();

        titleColorList[5]->property_foreground().set_value("#17ED70");

        tagTable->add(titleColorList[5]);

        // #D5711D 褐色
        titleColorList[6] = Gtk::TextTag::create();

        titleColorList[6]->property_foreground().set_value("#D5711D");

        tagTable->add(titleColorList[6]);

        // #C526F0 紫色
        titleColorList[7] = Gtk::TextTag::create();

        titleColorList[7]->property_foreground().set_value("#C526F0");

        tagTable->add(titleColorList[7]);

        // #F3E024 橙色
        titleColorList[8] = Gtk::TextTag::create();

        titleColorList[8]->property_foreground().set_value("#F3E024");

        tagTable->add(titleColorList[8]);

        // #26DDEB 青色
        titleColorList[9] = Gtk::TextTag::create();

        titleColorList[9]->property_foreground().set_value("#26DDEB");

        tagTable->add(titleColorList[9]);

        // #6F831D  深绿
        titleColorList[10] = Gtk::TextTag::create();

        titleColorList[10]->property_foreground().set_value("#6F831D");

        tagTable->add(titleColorList[10]);

        // #F6B7EE  粉红
        titleColorList[11] = Gtk::TextTag::create();

        titleColorList[11]->property_foreground().set_value("#F6B7EE");

        tagTable->add(titleColorList[11]);

        // #65C927 军绿
        titleColorList[12] = Gtk::TextTag::create();

        titleColorList[12]->property_foreground().set_value("#65C927");

        tagTable->add(titleColorList[12]);

        // #EA89B0  浅红
        titleColorList[13] = Gtk::TextTag::create();

        titleColorList[13]->property_foreground().set_value("#EA89B0");

        tagTable->add(titleColorList[13]);
}

TagList::Tags& TagList::getTags() {
        static Tags tags;
        return tags;
}

/*
const TagList::RefTag TagList::getTitleColor(uint32_t uid)
{
    TagMap::iterator iter = tagmap.find(uid);
    if (iter == tagmap.end()) {
 RefTag tag = getTags().titleColorList[colorid++];
 tagmap.insert(TagMap::value_type(uid, tag));
 colorid %= COLOR_LIST_SIZE;
 return tag;
    }
 
    return iter->second;
}
*/
const TagList::RefTag TagList::getTitleColor(int id) {
        return getTags().titleColorList[id];
}
}

