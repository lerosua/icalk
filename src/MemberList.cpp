
#include "MemberList.h"
#include <iostream>

MemberList::MemberList()
{
        MemberList* memberList = this;
        memberList->set_flags(Gtk::CAN_FOCUS);
        memberList->set_headers_visible(false);
        memberList->set_rules_hint(false);
        memberList->set_reorderable(false);
        memberList->set_enable_search(true);

        refListStore = Gtk::ListStore::create(columns);

        refListStore->set_default_sort_func(sigc::mem_fun(*this, &MemberList::on_sort_compare));
        refListStore->set_sort_column_id(
                Gtk::TreeSortable::DEFAULT_SORT_COLUMN_ID,
                Gtk::SORT_ASCENDING);
        memberList->set_model(refListStore);
        memberList->append_column("face", columns.icon);
        memberList->append_column("nickname", columns.name);
        memberList->append_column("status", columns.status);
        memberList->show();

}

bool MemberList::isMember(const std::string& mid)
{
        /*
        Gtk::TreeModel::Children children = tree_store->children();
        Gtk::TreeModel::iterator listiter;
        listiter = find_if(children.begin(),
                children.end(),
                bind2nd(CompareBuddy(MemberColumns), mid));
        if(listiter == children.end())
         return false;
        */ 
        return true;

}

Glib::RefPtr<Gdk::Pixbuf> MemberList::getPixfromAffilliation(MUCRoomAffiliation affiliation)
{
        std::string pix;

        switch (affiliation) {

        case AffiliationNone:
                pix.assign("voice.png");
                break;

        case AffiliationOutcast:
                pix.assign("outcast.png");
                break;

        case AffiliationMember:
                pix.assign("voice.png");
                break;

        case AffiliationOwner:
                pix.assign("owner.png");
                break;

        case AffiliationAdmin:
                pix.assign("admin.png");
                break;

        case AffiliationInvalid:
                pix.assign("invalid.png");
                break;

        default:
                pix.assign("voice.png");
                break;
        }

        return getPix(pix.c_str());
}

void MemberList::addMember(const std::string& name, const Member& member)
{
        Gtk::TreeModel::iterator iter = refListStore->append();

        //(*iter)[columns.icon] = getPix("voice.png");
        (*iter)[columns.icon] = getPixfromAffilliation(member.affiliation);
        (*iter)[columns.name] = name;
        (*iter)[columns.mid] = member.id;
        (*iter)[columns.status] = member.status;
        (*iter)[columns.presence] = 0;

}

void MemberList::clearMember()
{
        Gtk::TreeModel::Children children = refListStore->children();
        Gtk::TreeModel::iterator iter = children.begin();

        while ( iter != children.end()) {
                //Glib::ustring name = (*iter)[columns.name];
                //std::cout<<"erase ============"<<name<<std::endl;
                iter = refListStore->erase(iter);
        }
}

void MemberList::removeMember(const std::string& name)
{
        /*
           Gtk::TreeModel::iterator iter =
           getListIter(refListstore->children(), id);
           if (iter != refListstore->children().end())
           refListstore->erase(iter);
           */

}

Glib::ustring MemberList::getSelectMemberid()
{

        Glib::RefPtr<Gtk::TreeSelection> selection = get_selection();

        if (!selection->count_selected_rows())
                return Glib::ustring();

        Gtk::TreeModel::iterator iter = selection->get_selected();

        return (*iter)[columns.mid];
}

bool MemberList::on_button_press_event(GdkEventButton * ev)
{

        bool result = Gtk::TreeView::on_button_press_event(ev);

        Glib::RefPtr < Gtk::TreeSelection > selection =
                this->get_selection();
        Gtk::TreeModel::iterator iter = selection->get_selected();

        if (!selection->count_selected_rows())
                return result;

        Glib::ustring name = (*iter)[columns.mid];

        Gtk::TreeModel::Path path(iter);

        Gtk::TreeViewColumn * tvc;

        int cx, cy;

        /** get_path_at_pos() 是为确认鼠标是否在选择行上点击的*/
        if (!this->
                        get_path_at_pos((int) ev->x, (int) ev->y, path, tvc, cx, cy))
                return FALSE;

        if ((ev->type == GDK_2BUTTON_PRESS ||
                        ev->type == GDK_3BUTTON_PRESS)) {
                std::cout << "双击" << name << std::endl;


        } else if ((ev->type == GDK_BUTTON_PRESS)
                        && (ev->button == 3)) {
                std::cout << "右击" << name << std::endl;
        }

}
