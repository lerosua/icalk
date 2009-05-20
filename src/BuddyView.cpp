/*
* =====================================================================================
*
*       Filename:  BuddyView.cpp
*
*    Description:  显示好友列表的控件
*
*        Version:  1.0
*        Created:  2007年06月28日 19时26分39秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:  cyclone
*
*  纪念5.12汶川地震中消逝的那些生命
* =====================================================================================
*/

#include <glib/gi18n.h>
#include <fstream>
#include <unistd.h>
#include "BuddyView.h"
#include "BuddyList.h"
#include "Bodies.h"
#include "Unit.h"
#include "pixmaps.h"
#include "sounds.h"
#include "Unit.h"
#include "TreeViewTooltips.h"
#include "RoomItem.h"
#include "Buddy.h"

BuddyView::BuddyView(MainWindow & f_parent):
                m_parent(f_parent)
                , SHOWALL(false)
                , EXPAND(false)
                , m_filterText("")
{
        set_headers_visible(false);
        set_border_width(5);
        set_name("icalk_blist_treeview");


        add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_MOTION_MASK |
                   Gdk::BUTTON_PRESS_MASK | Gdk::
                   BUTTON_RELEASE_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::
                   LEAVE_NOTIFY_MASK);
        m_tooltips = new TreeViewTooltips(this);

        m_treestore = TreeModelDnd::create(buddyColumns);
        //set_model(m_treestore);

        m_treemodelfilter = Gtk::TreeModelFilter::create(m_treestore);
        m_treemodelfilter->set_visible_func(sigc::mem_fun(*this, &BuddyView::
                                            list_visible_func));

        set_model(m_treemodelfilter);

        append_column("ICON", buddyColumns.icon);
        //set_show_expanders(false); //gtkmm 2.12

        /*
           Gtk::TreeView::Column * col =
           Gtk::manage(new Gtk::TreeView::Column("iCalk"));
           col->pack_start(buddyColumns.nickname, false);       //false = don't expand
           vector < Gtk::CellRenderer * >rends = col->get_cell_renderers();
           col->clear_attributes(*rends[0]);
           col->add_attribute(*rends[0], "markup", 1);
         */

        Gtk::TreeView::Column * col =
                Gtk::manage(new Gtk::TreeView::Column("iCalk"));
        col->pack_start(m_rendtext);
        //Tell the view column how to render the model values:
        //将ViewColumn控件和Model values关联起来
        col->set_cell_data_func(m_rendtext,
                                sigc::mem_fun(*this,
                                              &BuddyView::
                                              tvc_connect_cell_data));
        //make the cellrenderer ellipsize
#ifdef GLIBMM_PROPERTIES_ENABLED
        //m_rendtext.property_editable()=true;
        m_rendtext.property_ellipsize() = Pango::ELLIPSIZE_END;
#else

        m_rendtext.set_property("editable", ture);
#endif

        m_rendtext.signal_editing_started().
        connect(sigc::
                mem_fun(*this,
                        &BuddyView::cellrender_on_editing_start));
        m_rendtext.signal_edited().
        connect(sigc::
                mem_fun(*this, &BuddyView::cellrender_on_edited));

        col->add_attribute(m_rendtext.property_markup(),
                           buddyColumns.nickname);
        col->set_resizable(true);
        col->set_expand();


        this->append_column(*col);
        this->append_column("Voip", buddyColumns.audioicon);

        m_treestore->
        set_default_sort_func(sigc::
                              mem_fun(*this,
                                      &BuddyView::on_sort_compare));
        m_treestore->
        set_sort_column_id(Gtk::TreeSortable::DEFAULT_SORT_COLUMN_ID,
                           Gtk::SORT_ASCENDING);

        //设置可托拽
        this->enable_model_drag_source();
        this->enable_model_drag_dest();

	this->set_tooltip_window( *m_tooltips);
	this->set_has_tooltip();
	this->signal_query_tooltip().connect(sigc::mem_fun(*this,
				&BuddyView::on_tooltip_show));
	/*
        this->signal_motion_notify_event().
        connect(sigc::mem_fun(*this, &BuddyView::on_motion_event),
                false);
        this->signal_leave_notify_event().
        connect(sigc::mem_fun(*this, &BuddyView::on_leave_event),
                false);
	*/
        /*
           this->signal_enter_notify_event().connect(sigc::mem_fun(
           *this,&BuddyView::on_enter_event));
         */
        show_all_children();
}

BuddyView::~BuddyView()
{
        delete m_tooltips;

}

int BuddyView::on_sort_compare(const Gtk::TreeModel::iterator & a,
                               const Gtk::TreeModel::iterator & b)
{
        int result;

        if ((result =
                                (*a)[buddyColumns.status] - (*b)[buddyColumns.status]) == 0) {
                Glib::ustring an = (*a)[buddyColumns.nickname];
                Glib::ustring bn = (*b)[buddyColumns.nickname];
                result = an.lowercase().compare(bn.lowercase());
        }

        return result;
}

bool BuddyView::on_motion_event(GdkEventMotion * ev)
{
        Gtk::TreeModel::Path path;
        Gtk::TreeViewColumn * column;
        int cell_x, cell_y;
        int delay = 600;

        if (m_tipTimeout.connected()) {

                m_tipTimeout.disconnect();
                m_tooltips->hideTooltip();
        }

        if (this->
                        get_path_at_pos((int) ev->x, (int) ev->y, path, column, cell_x,
                                        cell_y)) {
                Gtk::TreeModel::iterator iter =
                        this->get_model()->get_iter(path);
                int type = (*iter)[buddyColumns.status];

                if (STATUS_GROUP != type)
                        m_tipTimeout =
                                Glib::signal_timeout().connect(sigc::bind <
                                                               GdkEventMotion *
                                                               > (sigc::
                                                                  mem_fun(*this,
                                                                          &BuddyView::
                                                                          tooltip_timeout),
                                                                  ev), delay);
                else
                        m_tooltips->hideTooltip();
        } else
                m_tooltips->hideTooltip();

        return true;
}

bool BuddyView::on_tooltip_show(int x,int y, bool key_mode,const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
        Gtk::TreeModel::Path path;
        Gtk::TreeViewColumn * column;
        int cell_x, cell_y;

        if (this->get_path_at_pos(x, y, path, column, cell_x,cell_y)){
                Gtk::TreeModel::iterator iter =
                        this->get_model()->get_iter(path);

                if (!iter)
                        return false;

                int type = (*iter)[buddyColumns.status];

                Glib::ustring jid = (*iter)[buddyColumns.id];

                Glib::ustring text_, status_, status_sub, msg_;

                if (STATUS_ROOM != type) {
                        Buddy *buddy =
                                Bodies::Get_Bodies().get_buddy_list().
                                find_buddy(jid);

                        if (NULL == buddy)
                                return false;

                        SubscriptionType substrEnum =
                                buddy->getSubscription();

                        status_sub = "<span weight='bold'>";

                        if (S10nBoth == substrEnum)
                                status_sub =
                                        status_sub +
                                        _("subscribed: </span>Both \n");
                        else
                                status_sub =
                                        status_sub +
                                        _("subscribed: </span>Delay \n");

                        text_ =
                                "<span weight='bold'>" + jid + "\n" +
                                status_sub + _("Status (");

                        status_ =
                                buddy->getResource() + "):</span> " +
                                buddy->get_sign_msg() + "\n";

                        msg_ = text_ + status_;

                        m_tooltips->setLabel(msg_);

                        Glib::RefPtr < Gdk::Pixbuf > logo =
                                buddy->getLogo();

                        m_tooltips->setImage(logo);

                        //m_tooltips->showTooltip(ev);
                } else {
                        text_ =
                                "<span weight='bold'>" + jid + "\n" +
                                "type:</span> Room\n";
                        Glib::RefPtr < Gdk::Pixbuf > logo_ =
                                getPix("room.png");
                        m_tooltips->setLabel(text_);
                        m_tooltips->setImage(logo_);
                        //m_tooltips->showTooltip(ev);
                }
		return true;
        }
	return false;
}


bool BuddyView::tooltip_timeout(GdkEventMotion * ev)
{
        Gtk::TreeModel::Path path;
        Gtk::TreeViewColumn * column;
        int cell_x, cell_y;

        if (this->
                        get_path_at_pos((int) ev->x, (int) ev->y, path, column, cell_x,
                                        cell_y)) {
                Gtk::TreeModel::iterator iter =
                        this->get_model()->get_iter(path);

                if (!iter)
                        return false;

                int type = (*iter)[buddyColumns.status];

                Glib::ustring jid = (*iter)[buddyColumns.id];

                Glib::ustring text_, status_, status_sub, msg_;

                if (STATUS_ROOM != type) {
                        Buddy *buddy =
                                Bodies::Get_Bodies().get_buddy_list().
                                find_buddy(jid);

                        if (NULL == buddy)
                                return false;

                        SubscriptionType substrEnum =
                                buddy->getSubscription();

                        status_sub = "<span weight='bold'>";

                        if (S10nBoth == substrEnum)
                                status_sub =
                                        status_sub +
                                        _("subscribed: </span>Both \n");
                        else
                                status_sub =
                                        status_sub +
                                        _("subscribed: </span>Delay \n");

                        text_ =
                                "<span weight='bold'>" + jid + "\n" +
                                status_sub + _("Status (");

                        status_ =
                                buddy->getResource() + "):</span> " +
                                buddy->get_sign_msg() + "\n";

                        msg_ = text_ + status_;

                        m_tooltips->setLabel(msg_);

                        Glib::RefPtr < Gdk::Pixbuf > logo =
                                buddy->getLogo();

                        m_tooltips->setImage(logo);

                        m_tooltips->showTooltip(ev);
                } else {
                        text_ =
                                "<span weight='bold'>" + jid + "\n" +
                                "type:</span> Room\n";
                        Glib::RefPtr < Gdk::Pixbuf > logo_ =
                                getPix("room.png");
                        m_tooltips->setLabel(text_);
                        m_tooltips->setImage(logo_);
                        m_tooltips->showTooltip(ev);
                }

        }

        return false;
}

bool BuddyView::on_leave_event(GdkEventCrossing * ev)
{
        if (m_tipTimeout.connected()) {
                m_tipTimeout.disconnect();
        }

        return false;
}

bool BuddyView::on_enter_event(GdkEventCrossing * ev)
{
        std::cout << "enter event" << std::endl;
        return true;
}

void BuddyView::loadBlistTag()
{
        char buf[512];
        snprintf(buf, 512, "%s/blist.xml", GUnit::getUserPath());

        ConfXml blistxml;
        blistTag = blistxml.xml_from_file(buf);
}

void BuddyView::saveBlistTag()
{
        char buf[512];
        snprintf(buf, 512, "%s/blist.xml", GUnit::getUserPath());

        ConfXml blistxml;
        blistxml.setTagXml(blistTag);
        blistxml.xml_saveto_file(buf);
}


const std::string BuddyView::getBlistTag(const std::string & tagname,
                const std::string & name,
                const std::string & attr)
{
        Tag *tmpTag = blistTag->findChild(tagname, "name", name);

        if (tmpTag == NULL) {
                return std::string();
        }

        Tag *sontag = tmpTag->findChild(attr);

        if (sontag == NULL)
                return std::string();

        return sontag->cdata();
}

void BuddyView::setBlistTag(const std::string & tagname,
                            const std::string & name,
                            const std::string & attr,
                            const std::string & value)
{
        Tag *tmpTag = blistTag->findChild(tagname, "name", name);

        if (tmpTag == NULL) {
                tmpTag = new Tag(tagname, "name", name);
                blistTag->addChild(tmpTag);
        }

        Tag *sontag = tmpTag->findChild(attr);

        if (sontag == NULL) {
                sontag = new Tag(attr, value);
                tmpTag->addChild(sontag);
        } else
                sontag->setCData(value);

        saveBlistTag();
}

void BuddyView::removeBlistTag(const std::string& tagname, const std::string& name)
{
        Tag *tmpTag = blistTag->findChild(tagname, "name", name);
        blistTag->removeChild(tmpTag);

        saveBlistTag();
}



void BuddyView::tvc_connect_cell_data(Gtk::CellRenderer * renderer,
                                      const Gtk::TreeModel::
                                      iterator & iter)
{
        if (iter) {
#ifdef GLIBMM_PROPERTIES_ENABLED
                //m_rendtext.property_text() = (*iter)[buddyColumns.nickname];
                m_rendtext.property_markup() =
                        (*iter)[buddyColumns.nickname];
#else

                m_rendtext.set_property("text",
                                        (*iter)[buddyColumns.nickname]);
#endif

        }
}

bool BuddyView::list_visible_func(const Gtk::TreeIter& iter)
{

        Glib::ustring email = (*iter)[buddyColumns.id];
        Glib::ustring name = (*iter)[buddyColumns.nickname];
        int type = (*iter)[buddyColumns.status];

        if (m_filterText.empty())
                return true;
        else if (email.lowercase().find(m_filterText.lowercase()) != Glib::ustring::npos)
                return true;
        else if (name.lowercase().find(m_filterText.lowercase()) != Glib::ustring::npos)
                return true;
        else if (type == STATUS_GROUP )
                return true;

        return false;

}

void BuddyView::setFilterText(const Glib::ustring& text)
{
        m_filterText = text;
        m_treemodelfilter->refilter();
}

void BuddyView::cellrender_edited(bool mode)
{
        if (mode) {
                m_rendtext.property_editable() = true;
        } else {

                m_rendtext.property_editable() = false;
        }
}

void BuddyView::cellrender_on_editing_start(Gtk::CellEditable *
                cell_editable,
                const Glib::ustring & path)
{
        Glib::RefPtr < Gtk::TreeSelection > selection =
                this->get_selection();
        Gtk::TreeModel::iterator iter = selection->get_selected();

        if (!selection->count_selected_rows())
                return ;

        Glib::ustring jid = (*iter)[buddyColumns.id];

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(jid);

        const Glib::ustring & nickname = buddy->get_nickname();

        Gtk::CellEditable * cell = cell_editable;

        Gtk::Entry * pEntry = dynamic_cast < Gtk::Entry * >(cell);

        if (pEntry) {
                if (!nickname.empty())
                        pEntry->set_text(nickname);
                else
                        pEntry->set_text(jid);
        }
}

void BuddyView::cellrender_on_edited(const Glib::ustring & path_string,
                                     const Glib::ustring & new_text)
{
        Gtk::TreePath path(path_string);

        if (new_text.empty())
                return ;

        //Get the row from the path;
        Gtk::TreeModel::iterator iter = m_treestore->get_iter(path);

        if (iter) {
                Glib::ustring jid = (*iter)[buddyColumns.id];
                Buddy *buddy =
                        Bodies::Get_Bodies().get_buddy_list().find_buddy(jid);
                buddy->set_nickname(new_text);
                Bodies::Get_Bodies().get_client().rosterManager()->
                synchronize();
                refreshBuddyStatus(jid);
        }

        cellrender_edited(false);
}

Gtk::TreeModel::iterator BuddyView::getListIter(Gtk::TreeModel::
                Children children,
                const Glib::ustring & id)
{
        /*查找好友列表项 */
        return find_if(children.begin(),
                       children.end(),
                       bind2nd(CompareBuddy(buddyColumns), id));
}

int BuddyView::iter_n_children(Gtk::TreeModel::iterator listiter)
{
        if (listiter->children().empty())
                return 0;

        int num = 0;

        Gtk::TreeModel::iterator iter = listiter->children().begin();

        for (; iter != listiter->children().end(); iter++)
                num++;

        return num;

}

void BuddyView::expanded_list()
{
        if (EXPAND) {
                this->collapse_all();
                EXPAND = false;
        } else {
                this->expand_all();
                EXPAND = true;
        }

}

bool BuddyView::remove
        (const Glib::ustring & id)
{
        /*查找要删除的好友在列表中是否已经显示 */

        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(id);

        if (NULL == buddy) {
                DLOG("buddy of %s is NULL\n", id.c_str());
                return false;
        }

        StringList g = buddy->getGroups();

        if (g.empty()) {
                std::string mainGroup = "iCalk";
                g.push_back(mainGroup);
        }

        StringList::const_iterator it_g = g.begin();

        for (; it_g != g.end(); ++it_g) {
                listiter = getListIter(children, *it_g);

                if (listiter == children.end())
                        return false;

                Gtk::TreeModel::Children grandson = (*listiter).children();

                Gtk::TreeModel::iterator treeiter =
                        getListIter(grandson, id);

                if (treeiter == grandson.end())
                        return false;

                if (!SHOWALL)
                        m_treestore->erase(treeiter);


                int num = iter_n_children(listiter);

                Glib::ustring groupName = (*listiter)[buddyColumns.id];

                char *marktext = g_markup_printf_escaped("<b>%s</b>(%d)",
                                 groupName.c_str(),
                                 num);

                (*listiter)[buddyColumns.nickname] = marktext;

                g_free(marktext);

                /**如果组为空，则也删除组*/
                if (listiter->children().empty())
                        m_treestore->erase(*listiter);
        }

        return false;
}

void BuddyView::initial()
{
        const BuddyList::BUDDY_MAP & buddyMap =
                Bodies::Get_Bodies().get_buddy_list().get_buddy_map();
        BuddyList::BUDDY_MAP::const_iterator iter;

        for (iter = buddyMap.begin(); iter != buddyMap.end(); iter++)
                initBuddy((*iter).second);

        showOffline(false);

        showGroup(false);
}

void BuddyView::initBuddyType(Buddy* value)
{
        const std::string & buddyname = value->get_jid();
        /** 根据服务器名猜测好友类型*/
        BuddyType buddyType = value->guessType();
        value->setType(buddyType);

        /** 根据配置设置Buddy的类型*/
        const std::string & mType =
                getBlistTag("buddy", buddyname, "type");

        if (mType.empty())
                return ;

        int iType = atoi(mType.c_str());

        value->setType(iType);
}

void BuddyView::initBuddy(Buddy * value)
{
        StringList g = value->getGroups();
        StringList::const_iterator it_g;
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;

        if (g.empty()) {
                std::string mainGroup = "iCalk";
                g.push_back(mainGroup);
        }

        for (it_g = g.begin(); it_g != g.end(); ++it_g) {
                listiter = getListIter(children, *it_g);

                if (listiter == children.end()) {
                        listiter = addBuddyGroup(*it_g);
                        groupList.push_back(*it_g);
                }


                /* 添加好友 */
                Gtk::TreeModel::iterator treeiter =
                        m_treestore->append(listiter->children());

                const std::string & buddyname = value->get_jid();

                /** 根据用户信息猜测用户类型*/
                const int buddyType = value->guessType();

                switch (buddyType) {

                case TYPE_TRANPORT:
                        (*treeiter)[buddyColumns.type] = TYPE_TRANPORT;
                        value->setType(TYPE_TRANPORT);
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("transport.png");
                        break;

                case TYPE_MSN:
                        (*treeiter)[buddyColumns.type] = TYPE_MSN;
                        value->setType(TYPE_MSN);
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("msn.png");
                        break;

                default:
                        (*treeiter)[buddyColumns.type] = TYPE_FRIEND;
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("CallDown.png");
                        break;
                }

                /** 根据配置设置Buddy的类型*/
                const std::string & mType =
                        getBlistTag("buddy", buddyname, "type");

                int iType = atoi(mType.c_str());

                value->setType(iType);


                Glib::RefPtr < Gdk::Pixbuf > officon;

                const std::string & filename_ =
                        getBlistTag("buddy", buddyname, "icon");

                if ((!filename_.empty())
                                && (!access(filename_.c_str(), F_OK))) {
                        try {
                                officon =
                                        Gdk::Pixbuf::create_from_file(filename_.
                                                                      c_str(), 30, 30);
                        } catch (Glib::FileError e) {
                                g_message("caught Glib::FileError in initBuddy create from file");
                        } catch (Gdk::PixbufError e) {
                                g_message("Gdk::PixbufError in create_from_file");
                        }

                        if (0 == officon)
                                officon = getPix30("offline.png");
                } else
                        officon = getPix30("offline.png");

                Glib::RefPtr < Gdk::Pixbuf > emblem =
                        getPix("unavailable.png");

                emblem->composite(officon, 15 /*目标x */ ,
                                  15 /*目标y */ , 15, 15, 15, 15, 1, 1,
                                  Gdk::INTERP_BILINEAR, 255);

                (*treeiter)[buddyColumns.icon] = officon;

                char *marktext;

                const std::string & nickname = value->get_nickname();

                if (!nickname.empty())
                        marktext =
                                g_markup_printf_escaped
                                (_
                                 ("<span color='dim grey'>%s\noffline</span>"),
                                 nickname.c_str());
                else
                        marktext =
                                g_markup_printf_escaped
                                (_
                                 ("<span color='dim grey'>%s\noffline</span>"),
                                 buddyname.c_str());

                (*treeiter)[buddyColumns.nickname] = marktext;

                g_free(marktext);

                (*treeiter)[buddyColumns.status] =
                        (int) value->get_status();

                (*treeiter)[buddyColumns.id] = value->get_jid();


        }

}

void BuddyView::initRoomList()
{
        gloox::TagList rlist = blistTag->findChildren("room");
        gloox::TagList::const_iterator it_g;

        for (it_g = rlist.begin(); it_g != rlist.end(); ++it_g) {
                ConferenceListItem ci;
                ci.jid = (*it_g)->findAttribute("name");
                Tag *tmptag = (*it_g)->findChild("roomname");
                ci.name = tmptag->cdata();
                tmptag = (*it_g)->findChild("nickname");
                ci.nick = tmptag->cdata();
                tmptag = (*it_g)->findChild("passwd");
                ci.password = tmptag->cdata();
                tmptag = (*it_g)->findChild("autojoin");

                if (tmptag && ("false" == tmptag->cdata()))
                        ci.autojoin = 0;
                else
                        ci.autojoin = 1;

                addRoom(ci);

        }

}


Gtk::TreeModel::iterator BuddyView::addBuddyGroup(const Glib::
                ustring & groupName)
{
        Gtk::TreeModel::iterator listiter = m_treestore->append();
        char *marktext;
        (*listiter)[buddyColumns.icon] = getPix16("group.png");
        (*listiter)[buddyColumns.id] = groupName;
        marktext = g_markup_printf_escaped("<b>%s</b>", groupName.c_str());
        (*listiter)[buddyColumns.nickname] = marktext;
        (*listiter)[buddyColumns.status] = STATUS_GROUP;
        g_free(marktext);

        groupList.push_back(groupName);
        return listiter;
}

void BuddyView::addRoom(const ConferenceListItem & ci)
{
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;
        std::string roomgroup = "room";
        listiter = getListIter(children, roomgroup);

        if (listiter == children.end())
                listiter = addBuddyGroup(roomgroup);

        Gtk::TreeModel::Children grandson = (*listiter).children();

        Gtk::TreeModel::iterator treeiter =
                getListIter(grandson, ci.jid);

        //如果在列表中，则返回。防止加载相同的聊天室
        if (!(treeiter == grandson.end()))
                return ;

        treeiter =
                m_treestore->append(listiter->children());

        (*treeiter)[buddyColumns.id] = ci.jid;

        (*treeiter)[buddyColumns.nickname] = ci.name;

        (*treeiter)[buddyColumns.status] = STATUS_ROOM;

        (*treeiter)[buddyColumns.icon] = getPix30("room.png");

        (*treeiter)[buddyColumns.audioicon] = getPix("CallOver.png");

        int num = iter_n_children(listiter);

        Glib::ustring groupName = (*listiter)[buddyColumns.id];

        char *marktext =
                g_markup_printf_escaped("<b>%s</b>(%d)", groupName.c_str(),
                                        num);

        (*listiter)[buddyColumns.nickname] = marktext;

        g_free(marktext);

        /*将房间初始化 -- initial room */
        RoomItem *item = new RoomItem(ci);

        Bodies::Get_Bodies().getRoomHandler().addRoomItem(ci.jid,
                        item);

        if (ci.autojoin) {
                item->join();
        }
}


void BuddyView::delRoom(const Glib::ustring & jid)
{

        std::cout << "del room " << jid << std::endl;
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;
        std::string roomgroup = "room";
        listiter = getListIter(children, roomgroup);

        if (listiter == children.end())
                return ;

        Gtk::TreeModel::Children grandson = (*listiter).children();

        Gtk::TreeModel::iterator treeiter = getListIter(grandson, jid);

        if (treeiter == grandson.end())
                return ;

        m_treestore->erase(treeiter);

        /** delete room file in blist.xml*/
        /** 删除掉blist.xml里的房间信息*/
        removeBlistTag("room", jid);

        /**更新组名里的成员数*/
        int num = iter_n_children(listiter);

        Glib::ustring groupName = (*listiter)[buddyColumns.id];

        char *marktext =
                g_markup_printf_escaped("<b>%s</b>(%d)", groupName.c_str(),
                                        num);

        (*listiter)[buddyColumns.nickname] = marktext;

        g_free(marktext);
}

void BuddyView::add
        (const Glib::ustring & jid_str)
{

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(jid_str);

        if (NULL == buddy) {
                DLOG("buddy of %s is NULL\n", jid_str.c_str());
                return ;
        }

        StringList g = buddy->getGroups();
        StringList::const_iterator it_g;
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;



        if (g.empty()) {
                //printf("group empty\n");
                std::string mainGroup = "iCalk";
                g.push_back(mainGroup);
        }

        for (it_g = g.begin(); it_g != g.end(); ++it_g) {
                listiter = getListIter(children, *it_g);

                if (listiter == children.end())
                        listiter = addBuddyGroup(*it_g);

                /* 添加好友 */
                Gtk::TreeModel::iterator treeiter =
                        m_treestore->append(listiter->children());

                (*treeiter)[buddyColumns.icon] = getPix30("offline.png");

                char *marktext;

                const std::string & nickname = buddy->get_nickname();

                if (buddy->getSubscription() == S10nNone)
                        marktext =
                                g_markup_printf_escaped(_
                                                        ("<span color='dim grey'>%s\nNot auth</span>"),
                                                        buddy->get_jid().
                                                        c_str());
                else if (!nickname.empty())
                        marktext =
                                g_markup_printf_escaped
                                (_
                                 ("<span color='dim grey'>%s\noffline</span>"),
                                 nickname.c_str());
                else
                        marktext =
                                g_markup_printf_escaped
                                (_
                                 ("<span color='dim grey'>%s\noffline</span>"),
                                 buddy->get_jid().c_str());

                (*treeiter)[buddyColumns.nickname] = marktext;

                g_free(marktext);

                (*treeiter)[buddyColumns.status] =
                        (int) buddy->get_status();

                (*treeiter)[buddyColumns.id] = jid_str;

                (*treeiter)[buddyColumns.audioicon] =
                        getPix("CallDown.png");

                int num = iter_n_children(listiter);

                Glib::ustring groupName = (*listiter)[buddyColumns.id];

                marktext =
                        g_markup_printf_escaped("<b>%s</b>(%d)",
                                                groupName.c_str(), num);

                (*listiter)[buddyColumns.nickname] = marktext;

                g_free(marktext);
        }

        this->expand_all();
}

void BuddyView::showGroup(bool mode)
{
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator it_g;
        it_g = children.begin();

        while (it_g != children.end()) {
                Gtk::TreeModel::Children grandson = it_g->children();

                if (grandson.empty())
                        it_g = m_treestore->erase(it_g);
                else
                        it_g++;
        }
}

void BuddyView::showOffline(bool mode)
{
        SHOWALL = mode;
        const BuddyList::BUDDY_MAP & buddyMap =
                Bodies::Get_Bodies().get_buddy_list().get_buddy_map();
        BuddyList::BUDDY_MAP::const_iterator iter;

        for (iter = buddyMap.begin(); iter != buddyMap.end(); iter++) {
                Presence::PresenceType type = (*iter).second->get_status();

                if (Presence::Unavailable == type) {
                        if (mode)
                                add
                                        ((*iter).second->get_jid());
                        else {
                                remove
                                        ((*iter).second->get_jid());
                        }
                }

        }
}

bool BuddyView::on_buddy_login(const Glib::ustring & jid_ctr)
{
        refreshBuddyStatus(jid_ctr);
        return false;
}

void BuddyView::refreshList()
{
        const BuddyList::BUDDY_MAP & buddyMap =
                Bodies::Get_Bodies().get_buddy_list().get_buddy_map();
        BuddyList::BUDDY_MAP::const_iterator iter;

        for (iter = buddyMap.begin(); iter != buddyMap.end(); iter++) {
                refreshBuddyStatus((*iter).second->get_jid());
        }
}

void BuddyView::refreshBuddyStatus(const Glib::ustring & jid_ctr)
{
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;
        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(jid_ctr);
        const std::string & msg = buddy->get_sign_msg();
        const std::string & nickname = buddy->get_nickname();
        const std::string & buddyname = buddy->get_jid();
        Presence::PresenceType status_ = buddy->get_status();
        char *marktext;
        Glib::RefPtr < Gdk::Pixbuf > emblem;
        StringList g = buddy->getGroups();

        if (g.empty()) {
                std::string mainGroup = "iCalk";
                g.push_back(mainGroup);
        }

        StringList::const_iterator it_g = g.begin();

        for (; it_g != g.end(); ++it_g) {
                listiter = getListIter(children, *it_g);

                if (listiter == children.end())
                        listiter = addBuddyGroup(*it_g);

                //return;

                Gtk::TreeModel::Children grandson = (*listiter).children();

                Gtk::TreeModel::iterator treeiter =
                        getListIter(grandson, jid_ctr);


                /*在列表中找不到则添加,这种情况应该为刚上线时 */
                if ((Presence::Unavailable != status_) && (treeiter == grandson.end())) {
                        treeiter =
                                m_treestore->append(listiter->children());

                        int delay = 9000;
                        Glib::RefPtr < Gdk::Pixbuf > signoff =
                                getPix("log-in.png");
                        (*treeiter)[buddyColumns.icon] = signoff;
                        (*treeiter)[buddyColumns.status] = (int) status_;
                        (*treeiter)[buddyColumns.id] = jid_ctr;
                        (*treeiter)[buddyColumns.nickname] = nickname;

                        if ((*buddy).timer.connected())
                                (*buddy).timer.disconnect();

                        (*buddy).timer =
                                Glib::signal_timeout().connect(sigc::bind <
                                                               std::string >
                                                               (sigc::
                                                                mem_fun(*this,
                                                                        &BuddyView::
                                                                        on_buddy_login),
                                                                jid_ctr),
                                                               delay);

                        sounds::play(sounds::ARRIVE_SOUND);

                        return ;
                } else if ((Presence::Unavailable == status_) && (treeiter == grandson.end())) {
                        /**好友状态为离线，并且不在列表中的状况*/
                        return ;
                }

                (*treeiter)[buddyColumns.status] = (int) status_;
                (*treeiter)[buddyColumns.id] = jid_ctr;


                /*下线时的处理情况 */

                if (Presence::Unavailable == status_) {
                        int delay = 9000;
                        Glib::RefPtr < Gdk::Pixbuf > signoff =
                                getPix("log-out.png");
                        (*treeiter)[buddyColumns.icon] = signoff;

                        if ((*buddy).timer.connected())
                                (*buddy).timer.disconnect();

                        (*buddy).timer =
                                Glib::signal_timeout().connect(sigc::bind <
                                                               std::string >
                                                               (sigc::
                                                                mem_fun(*this,
                                                                        &BuddyView::
                                                                        remove
                                                                               ),
                                                                        jid_ctr),
                                                                       delay);

                        sounds::play(sounds::LEAVE_SOUND);

                        return ;
                }

                /*如果签名不为空的情况 */
                if (!msg.empty()) {
                        /*如果有昵称的情况 */

                        if (!nickname.empty()) {
                                marktext =
                                        g_markup_printf_escaped
                                        ("%s\n<span color='dim grey'><small>%s</small></span>",
                                         nickname.c_str(), msg.c_str());
                        } else {
                                marktext =
                                        g_markup_printf_escaped
                                        ("%s\n<span color='dim grey'><small>%s</small></span>",
                                         buddyname.c_str(), msg.c_str());
                        }
                } else {
                        char buf[512];

                        if (!nickname.empty())
                                sprintf(buf, "%s", nickname.c_str());
                        else
                                sprintf(buf, "%s", buddyname.c_str());

                        switch (status_) {

                        case Presence::XA:
                                marktext =
                                        g_markup_printf_escaped
                                        (_
                                         ("%s\n<span color='dim grey'><small>extend leave</small></span>"),
                                         buf);
                                break;

                        case Presence::DND:
                                marktext =
                                        g_markup_printf_escaped
                                        (_
                                         ("%s\n<span color='dim grey'><small>don't distrub</small></span>"),
                                         buf);
                                break;

                        case Presence::Away:
                                marktext =
                                        g_markup_printf_escaped
                                        (_
                                         ("%s\n<span color='dim grey'><small>leave</small></span>"),
                                         buf);
                                break;

                        default:
                                marktext =
                                        g_markup_printf_escaped("%s", buf);
                                break;
                        }

                }

                (*treeiter)[buddyColumns.nickname] = marktext;
                g_free(marktext);

                (*treeiter)[buddyColumns.status] = (int) status_;
                initBuddyType(buddy);
                BuddyType mType = buddy->getType();

                switch (mType) {

                case TYPE_TRANPORT:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("transport.png");
                        break;

                case TYPE_GROUPCHAT:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("groupchat.png");
                        break;

                case TYPE_BOT:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("bot.png");
                        break;

                case TYPE_ICQ:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("icq.png");
                        break;

                case TYPE_MSN:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("msn.png");
                        break;

                case TYPE_YAHOO:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("yahoo.png");
                        break;

                case TYPE_OTHER:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("other.png");
                        break;

                default:
                        (*treeiter)[buddyColumns.audioicon] =
                                getPix("CallOver.png");

                        break;
                }

                //const VCard *vcard = buddy->get_vcard();

                {

                        /*
                                             char *random =
                                                     g_strdup_printf("%x", g_random_int());
                                             const char *dirname = GUnit::getIconPath();
                                             char *filename =
                                                     g_build_filename(dirname, random, NULL);
                         */

                        Glib::RefPtr < Gdk::Pixbuf > faceicon;
                        Glib::RefPtr < Gdk::Pixbuf > faceicon_orgin;

                        const std::string & filename_ =
                                getBlistTag("buddy", buddyname, "icon");
                        /*文件名存在且能打开文件 */

                        if ((!filename_.empty())
                                        && (!access(filename_.c_str(), F_OK)))
                        {
                                try {
                                        faceicon_orgin =
                                                Gdk::Pixbuf::
                                                create_from_file(filename_.c_str());
                                } catch (Glib::FileError e) {
                                        g_message("caught Glib::FileError in refreshBuddyStatus create from file");
                                } catch (Gdk::PixbufError e) {
                                        g_message("Gdk::PixbufError in create_from_file");
                                }

                                if (faceicon_orgin == 0) {
                                        faceicon = getPix30("default.png");
                                        printf
                                        ("pixbuf load file %s error\n",
                                         filename_.c_str());
                                        buddy->setLogo(faceicon);
                                } else {
                                        faceicon =
                                                faceicon_orgin->scale_simple(30, 30,
                                                                             Gdk::
                                                                             INTERP_NEAREST);
                                        buddy->setLogo(faceicon_orgin);
                                }
                        } else
                        {

                                /*
                                                            if (vcard && !vcard->photo().binval.empty()) {
                                                                    std::ofstream fout(filename);
                                                                    fout.write((const char *) vcard->
                                                                               photo().binval.c_str(),
                                                                               vcard->photo().binval.
                                                                               size());
                                                                    fout.close();

                                                                    try {
                                                                            faceicon_orgin =
                                                                                    Gdk::Pixbuf::
                                                                                    create_from_file(filename, 30,
                                                                                                     30);
                                                                    } catch (Glib::FileError e) {
                                                                            g_message("caught Glib::FileError in refreshBuddyStatus create from file");
                                                                    } catch (Gdk::PixbufError e) {
                                                                            g_message("Gdk::PixbufError in create_from_file");
                                                                    }

                                                                    if (faceicon_orgin == 0) {
                                                                            faceicon =
                                                                                    getPix30
                                                                                    ("default.png");
                                                                            printf
                                                                            ("pixbuf load file %s error\n",
                                                                             filename);
                                                                            buddy->setLogo(faceicon);
                                                                    } else {
                                                                            faceicon =
                                                                                    faceicon_orgin->scale_simple(30, 30,
                                                                                                                 Gdk::
                                                                                                                 INTERP_NEAREST);
                                                                            setBlistTag("buddy",
                                                                                        buddyname,
                                                                                        "icon",
                                                                                        filename);
                                                                            buddy->setLogo(faceicon_orgin);
                                                                    }
                                                            } else {
                                */
                                faceicon = getPix30("default.png");
                                buddy->setLogo(faceicon);
                                //}
                        }


                        switch (status_)
                        {

                        case Presence::XA:
                                emblem = getPix("Xa.png");
                                break;

                        case Presence::DND:
                                emblem = getPix("dnd.png");
                                break;

                        case Presence::Away:
                                emblem = getPix("extended_away.png");
                                break;

                        default:
                                break;
                        }

                        if (emblem)
                                emblem->composite(faceicon,
                                                  15 /*目标x */ ,
                                                  15 /*目标y */ , 15, 15,
                                                  15, 15, 1, 1,
                                                  Gdk::INTERP_BILINEAR,
                                                  255);

                        (*treeiter)[buddyColumns.icon] = faceicon;
                }

                int num = iter_n_children(listiter);

                Glib::ustring groupName = (*listiter)[buddyColumns.id];
                marktext =
                        g_markup_printf_escaped("<b>%s</b>(%d)",
                                                groupName.c_str(), num);
                (*listiter)[buddyColumns.nickname] = marktext;
                g_free(marktext);
        }

        //this->expand_all();
}

void BuddyView::newMsgBuddy(const Glib::ustring& f_jid, bool f_new)
{
        Gtk::TreeModel::Children children = m_treestore->children();
        Gtk::TreeModel::iterator listiter;

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(f_jid);
        StringList g = buddy->getGroups();

        if (g.empty()) {
                std::string mainGroup = "iCalk";
                g.push_back(mainGroup);
        }

        StringList::const_iterator it_g = g.begin();

        for (; it_g != g.end(); ++it_g) {
                listiter = getListIter(children, *it_g);

                if (listiter == children.end())
                        return ;

                Gtk::TreeModel::Children grandson = (*listiter).children();

                Gtk::TreeModel::iterator treeiter =
                        getListIter(grandson, f_jid);

                if (treeiter == grandson.end())
                        return ;

                if (f_new) {
                        Glib::RefPtr < Gdk::Pixbuf > new_msg_icon =
                                getPix("sip.png");
                        (*treeiter)[buddyColumns.icon] = new_msg_icon;
                } else {
                        refreshBuddyStatus(f_jid);
                }
        }

}

bool BuddyView::on_button_press_event(GdkEventButton * ev)
{
        bool result = Gtk::TreeView::on_button_press_event(ev);

        Glib::RefPtr < Gtk::TreeSelection > selection =
                this->get_selection();
        Gtk::TreeModel::iterator iter = selection->get_selected();

        if (!selection->count_selected_rows())
                return result;

        Gtk::TreeModel::Path path(iter);

        Gtk::TreeViewColumn * tvc;

        int cx, cy;

        /** get_path_at_pos() 是为确认鼠标是否在选择行上点击的*/
        if (!this->
                        get_path_at_pos((int) ev->x, (int) ev->y, path, tvc, cx, cy))
                return FALSE;

        Glib::ustring mid = (*iter)[buddyColumns.id];

        int type = (*iter)[buddyColumns.status];

        if ((ev->type == GDK_2BUTTON_PRESS ||
                        ev->type == GDK_3BUTTON_PRESS)) {
                if ((type != STATUS_GROUP) && (type != STATUS_ROOM)) {
                        Buddy *buddy =
                                Bodies::Get_Bodies().get_buddy_list().
                                find_buddy(mid);
                        //MsgPage *page_ = buddy->new_page();
                        buddy->new_page();
                        //buddy->new_session();
                        /*
                                             Bodies::Get_Bodies().get_msg_window().
                                             add_page(*page_);
                                             Bodies::Get_Bodies().get_msg_window().show();
                                             Bodies::Get_Bodies().get_msg_window().
                                             setCurrentPage(page_);
                        */
                } else if (STATUS_GROUP == type) {
                        //组的双击

                        if (this->row_expanded(path)) {
                                this->collapse_row(path);
                                //Glib::ustring group_ = (*iter)[buddyColumns.id];
                                //setBlistTag(group_,"group","collapsed");
                                //printf("row collapsed\n");
                        } else {
                                this->expand_row(path, false);
                                this->scroll_to_row(path);
                                //Glib::ustring group_ = (*iter)[buddyColumns.id];
                                //setBlistTag(group_,"group","expanded");
                                //printf("row expanded\n");
                        }
                } else if (STATUS_ROOM == type) {
                        //房间的双击
                        RoomItem *room =
                                Bodies::Get_Bodies().getRoomHandler().
                                findRoom(mid);

                        MsgPage *page = room->getPage();

                        if (NULL == page) {
                                room->join();
                                const std::string label =
                                        room->getRoomJID();
                                MsgPage *page_ =
                                        new MsgPage(label, room, 1);
                                room->setPage(page_);
                                page = room->getPage();
                                page->setSubject();
                                page->refreshMember();
                                Bodies::Get_Bodies().get_msg_window().
                                add_page(*page);
                        }

                        Bodies::Get_Bodies().get_msg_window().show();

                        Bodies::Get_Bodies().get_msg_window().
                        setCurrentPage(page);




                }
        } else if ((ev->type == GDK_BUTTON_PRESS)
                        && (ev->button == 3)) {
                if ((type != STATUS_GROUP) && (type != STATUS_ROOM)) {
                        Gtk::Menu* buddyMenu = m_parent.getBuddyMenu();

                        if (buddyMenu)
                                buddyMenu->popup(1, ev->time);

                } else if (STATUS_ROOM == type) {
                        Gtk::Menu* roomMenu = m_parent.getRoomMenu();
                        const std::string& blocked = getBlistTag("room", mid, "autojoin");

                        Glib::RefPtr<Gtk::ToggleAction>melem = m_parent.get_menu_action("RoomBlock");

                        if ("false" == blocked)
                                melem->set_active();
                        else
                                melem->set_active(false);

                        if (roomMenu)
                                roomMenu->popup(1, ev->time);

                }
        }

        return result;
}
