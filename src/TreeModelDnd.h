#ifndef TREEMODELDND_H_
#define TREEMODELDND_H_

#include <gtkmm.h>
/**@brief 解释一下BuddyColumns里行的组成结构
 * @param icon Buddy成员的图标，一般由对方上传的头像，默认会设置一个头像。
 * 离线时会是离线时的头像。
 * @param id Buddy成员的用户id，原始的jid string形式，如 icalk@gmail.com
 * @param status 代码Buddy成员的状态。普通的Buddy成员有六种状态。数字0-6,
 *   下面将解说特殊Buddy的status值。
 *   @li STATUS_GROUP: 表示列表中的组。
 *   @li STATUS_ROOM: 表示房间。
 *
 * @note 这种方式表示似乎有些局限性。考虑另起一个变量来表示类型，以支持
 * 像网关，Gtalk类型的聊天群这样有状态的类型。
 *
 * @param audioicon 本意是用以表示用户能否支持呼叫功能。但现在未实现，暂时表
 * 示上线的状态而已。
 */
/** 列表行的数据结构*/

class BuddyColumns: public Gtk::TreeModel::ColumnRecord
{

public:
        BuddyColumns();
        Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >icon;
        Gtk::TreeModelColumn < Glib::ustring > nickname;
        Gtk::TreeModelColumn < Glib::ustring > id;
        Gtk::TreeModelColumn < int >status;
        Gtk::TreeModelColumn < int >type;
        Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >audioicon;
};

class TreeModelDnd: public Gtk::TreeStore
{

protected:
        TreeModelDnd(const Gtk::TreeModelColumnRecord & columns);

public:
        static Glib::RefPtr < TreeModelDnd >
        create(const Gtk::TreeModelColumnRecord & columns);

protected:
        /** 标识行能否被拖放，返回真则能，返回假侧不能*/
        virtual bool row_draggable_vfunc(const Gtk::TreeModel::
                                         Path & path) const;
        /** 拖放的处理函数*/
        virtual bool row_drop_possible_vfunc(const Gtk::TreeModel::
                                             Path & dest,
                                             const Gtk::
                                             SelectionData &
                                             selection_data) const;

private:
        BuddyColumns buddyColumns;
};




#endif
