
#include <iostream>
#include "TreeModelDnd.h"
#include "icalk.h"

TreeModelDnd::TreeModelDnd(const Gtk::TreeModelColumnRecord& columns)
{
        set_column_types(columns);
}

Glib::RefPtr<TreeModelDnd> TreeModelDnd::create(const Gtk::TreeModelColumnRecord& columns)
{
        return Glib::RefPtr<TreeModelDnd>(new TreeModelDnd(columns));
}

bool TreeModelDnd::row_draggable_vfunc(const Gtk::TreeModel::Path& path)const
{
        TreeModelDnd* unconstThis = const_cast<TreeModelDnd*>(this);
        const_iterator iter = unconstThis->get_iter(path);

        if (iter) {
                Row row = *iter;
                int status_ = row[buddyColumns.status];
                printf("status_ is %d\n", status_);

                if (STATUS_GROUP == status_)
                        return false;
                else
                        return true;
        }

#ifdef GLIBMM_VFUNCS_ENABLED
        return Gtk::TreeStore::row_draggable_vfunc(path);

#else

        return false;

#endif

}

bool TreeModelDnd::row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest,
                const Gtk::SelectionData& selection_data)const
{
        Gtk::TreeModel::Path dest_parent = dest;

        bool dest_is_not_top_level = dest_parent.up();

        if (!dest_is_not_top_level || dest_parent.empty()) {
                //The user wants to move something to the top-level.
                //Let's always allow that.
                printf(" drag this \n");
        } else {
                TreeModelDnd* unconstThis = const_cast<TreeModelDnd*>(this);
                const_iterator iter_dest_parent = unconstThis->get_iter(dest_parent);
                Row row = *iter_dest_parent;
                Glib::ustring name = row[buddyColumns.id];
                printf("name is %s\n", name.c_str());
                int status_ = row[buddyColumns.status];
                printf("status_ is %d\n", status_);

                if (iter_dest_parent) {
                        return true;
                }
        }

#ifdef GLIBMM_VFUNCS_ENABLED
        return Gtk::TreeStore::row_drop_possible_vfunc(dest, selection_data);

#else

        return false;

#endif
}



BuddyColumns::BuddyColumns()
{
        add
                (icon);

        add
                (nickname);

        add
                (audioicon);

        add
                (id);

        add
                (status);

        add
                (type);
}
