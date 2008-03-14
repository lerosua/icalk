/*
* =====================================================================================
* 
*       Filename:  MemberList.h
* 
*    Description:  聊天室里的成员列表
* 
*        Version:  1.0
*        Created:  2007年08月25日 11时13分33秒 CST
*       Revision:  none
*       Compiler:  gcc
* 
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
* 
* =====================================================================================
*/

#ifndef  MEMBERLIST_FILE_HEADER_INC
#define  MEMBERLIST_FILE_HEADER_INC


#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include "pixmaps.h"
#include <functional>
#include "icalk.h" 
//#include "RoomItem.h"
using std::unary_function;


class MemberList : public Gtk::TreeView
{

public:
        MemberList();


        Glib::ustring getSelectMemberid();
        void addMember(const std::string& name, const Member& member_);
        void removeMember(const std::string& id);
        void clearMember();
        bool isMember(const std::string& mid);

protected:
        bool on_button_press_event(GdkEventButton *);

private:

struct MemberColumns : public Gtk::TreeModel::ColumnRecord
        {
                MemberColumns()
                {
                        add
                                (icon);

                        add
                                (name);

                        add
                                (mid);

                        add
                                (status);

                        add
                                (presence);
                }

                Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >icon;
                Gtk::TreeModelColumn < Glib::ustring > name;
                Gtk::TreeModelColumn < Glib::ustring > mid;
                Gtk::TreeModelColumn < Glib::ustring > status;
                Gtk::TreeModelColumn < int >presence;

        };

private:
        //排序函数
        int on_sort_compare(const Gtk::TreeModel::iterator& a,
                            const Gtk::TreeModel::iterator& b)
        {
                int result = (*a)[columns.presence] - (*b)[columns.presence];
                return result;
        }

        /** 比较好友*/
        /*
        struct CompareBuddy:public binary_function < Gtk::TreeModel::Row,
           const std::string, bool > {
        explicit CompareBuddy(const MemberColumns &
                column_):column(column_) {
        } bool operator () (const Gtk::TreeRow & lhs,
              const std::string & var) const {
         return lhs[column.mid] == var;
        } const MemberColumns & column;
        };
        */


private:
        MemberColumns columns;
        Glib::RefPtr<Gtk::ListStore> refListStore;

};



#endif   /* ----- #ifndef MEMBERLIST_FILE_HEADER_INC  ----- */

