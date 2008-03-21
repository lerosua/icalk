/*
* =====================================================================================
*
*       Filename:  BuddyView.h
*
*    Description:  显示好友列表的控件
*
*        Version:  1.0
*        Created:  2007年06月28日 19时01分59秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe) lerosua, xihels@gmail.com lerosua@gmail.com
*        Company:  Cyclone Team
*
* =====================================================================================
*/

#ifndef _BUDDY_VIEW_H_
#define _BUDDY_VIEW_H_

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeselection.h>
#include <gtkmm/tooltips.h>
#include <assert.h>
#include <functional>
#include <vector>
#include <gloox/gloox.h>
#include <gloox/bookmarkhandler.h>
#include <sigc++/connection.h>
#include "RoomItem.h"
#include "TalkRoomHandler.h"
//#include "TalkMenu.h"
#include "BuddyList.h"
#include "ConfXml.h"
#include "TreeModelDnd.h"
#include "TreeViewTooltips.h"
//using std::vector;
//using std::unary_function;

using namespace std;

using namespace gloox;

class BuddyList;

class Bodies;

class MainWindow;

class TreeViewTooltips;
/**
 * @brief  用于显示好友列表的控件.继承Gtk::TreeView
 * @author xihels@gmail.com
 *
 */

class BuddyView: public Gtk::TreeView
{

public:
        BuddyView(MainWindow & parent_);
        ~BuddyView();

        /**初始化好友列表，将调用initBuddy*/
        void initial();
        /**添加好友进列表*/

        void add
        (const Glib::ustring & jid);

        /**删除列表中的好友显示*/
        bool remove
        (const Glib::ustring & id);

        bool on_buddy_login(const Glib::ustring& id);

        /**更新好友状态*/
        void refreshBuddyStatus(const Glib::ustring & jid_ctr);

        /** 刷新列表*/
        void refreshList();

        /**初始化时添加好友进列表*/
        void initBuddy(Buddy * value);

        /** 初始化好友的类型*/
        void initBuddyType(Buddy* value);

        /** 显示/隐藏空组，未处理*/
        void showGroup(bool mode);

        /** 初始化聊天室列表*/
        void initRoomList();

        /** 添加聊天室项*/
        void addRoom(const ConferenceListItem& ci);

        /** 删除聊天室项*/
        void delRoom(const Glib::ustring& jid);

        /**
         * @brief 隐藏或显示离线好友
         * @param mode 真为显示，假为隐藏
         * */
        void showOffline(bool mode);

        /**
         * @brief 展开或折叠全列表项
         */
        void expanded_list();

        /**
         * @brief 获取TreeModel里的ID
         * @param iter TreeView里的某行
         * @return 返回此行的ID
         */
        Glib::ustring getIDfromIter(Gtk::TreeModel::iterator iter) {
                return (*iter)[buddyColumns.id];
        }

        /** 以ID号获取到TreeView里的某行指针*/
        Gtk::TreeModel::iterator getListIter(Gtk::TreeModel::
                                             Children children,
                                             const Glib::ustring & id);

        /** 返回组现有成员数*/
        int iter_n_children(Gtk::TreeModel::iterator listiter);

        /** 在列表上添加组*/
        Gtk::TreeModel::iterator addBuddyGroup(const Glib::
                                               ustring & groupName);


        /** 当行开始编辑时*/
        void cellrender_on_editing_start(Gtk::CellEditable * cell_editable,
                                         const Glib::ustring & path);

        /** 行编辑结束时*/
        void cellrender_on_edited(const Glib::ustring & path_string,
                                  const Glib::ustring & new_text);

        /**
         * @brief 设置行编辑功能
         * @param mode true 时为可以编辑
         *  false 时禁止编辑
         */
        void cellrender_edited(bool mode);

        void tvc_connect_cell_data(Gtk::CellRenderer * renderer,
                                   const Gtk::TreeModel::iterator & iter);

        /** 列表的TreeModelFilter 查找功能 基其功能是对treeview里的每个成员递归调用，并判断其是否匹配显示，返回真则显示，返回假则隐藏*/
        bool list_visible_func(const Gtk::TreeIter& iter);

        /** TreeModelFilter 查找开始*/
        void setFilterText(const Glib::ustring& text);

        /** 保存列表中的配置*/
        void saveBlistTag();

        /** 读取列表相关的配置*/
        void loadBlistTag();

        /**
         * @brief 用于取得配置文件Tag类中属于buddy的attr属性
         * @param name_ buddy的名字
         * @param attr  buddy的属性
         * @return  返回buddy的某attr属性的属性值
         */
        const std::string getBlistTag(const std::string& tagname, const std::string& name, const std::string& attr);

        void setBlistTag(const std::string& tagname, const std::string& name, const std::string& attr, const std::string & value);

        void removeBlistTag(const std::string& tagname, const std::string& name);

        /**得到当前treeview中的所有分组*/
        StringList getGroupList()const {
                return groupList;
        }

        /**
         * @brief 好友的信息提示超时函数
         * @param ev 传回的好友栏的事件,以取得当前所在坐标
         * @return 超时函数返回值，false就停止，true还继续执行
         */
        bool tooltip_timeout(GdkEventMotion* ev);

protected:
        bool on_button_press_event(GdkEventButton *);

        /** 鼠标在　TreeView里移动的信号回调*/
        bool on_motion_event(GdkEventMotion* ev);

        /** 鼠标离开时的回调*/
        bool on_leave_event(GdkEventCrossing* ev);

        bool on_enter_event(GdkEventCrossing* ev);

private:

        /*
           template < typename T >
           struct UpdateList:public std::unary_function <
           const typename T::value_type &, void > {

           UpdateList(const BuddyView & view_):view(view_) {
           };
           void operator () (const typename T::value_type & value);

           const BuddyView & view;
           };
         */


private:
        MainWindow & m_parent;

        TreeViewTooltips* m_tooltips;

        /*列表中好友的本地配置的配置文件类 */
        Tag *blistTag;

        Glib::RefPtr <TreeModelDnd > m_treestore;

        Glib::RefPtr <Gtk::TreeModelFilter> m_treemodelfilter;

        Glib::ustring m_filterText;

        BuddyColumns buddyColumns;

        Gtk::CellRendererText m_rendtext;

        /*全局组列表*/
        StringList groupList;

        sigc::connection m_tipTimeout;

        bool SHOWALL;


        /** 比较好友*/

struct CompareBuddy: public binary_function < Gtk::TreeModel::Row,
                                const Glib::ustring, bool > {
                explicit CompareBuddy(const BuddyColumns &
                                      column_): column(column_) {}

                bool operator () (const Gtk::TreeRow & lhs,
                                  const Glib::ustring & var) const {
                        return lhs[column.id] == var;
                }

                const BuddyColumns & column;
        };

        /** TreeView的排序函数*/
        int on_sort_compare(const Gtk::TreeModel::iterator & a,
                            const Gtk::TreeModel::iterator & b) {
                int result;

                if ((result =
                                        (*a)[buddyColumns.status] - (*b)[buddyColumns.status]) == 0)

                        ;

                {
                        Glib::ustring an = (*a)[buddyColumns.nickname];
                        Glib::ustring bn = (*b)[buddyColumns.nickname];
                        result = an.compare(bn);
                }

                return result;
        }
};

#endif    // _BUDDY_VIEW_H_
