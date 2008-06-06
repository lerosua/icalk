/*
* =====================================================================================
*
*       Filename:  FTWidget.h
*
*    Description:  File transport window
*
*        Version:  1.0
*        Created:  2008年03月18日 09时16分26秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef FTWIDGET_H_
#define FTWIDGET_H_
#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeselection.h>
#include <functional>
#include <vector>

//#define ftwidget_ui DATA_DIR"/ui/ftwidget.glade"

using namespace std;

//typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class MainWindow;

/**
 * 显示文件传输状态的窗口
 */

class FTWidget: public Gtk::Window
{

public:
        FTWidget(MainWindow* parent_);
        ~FTWidget();
        /** 窗口退出*/
        void on_button_quit();
        /** 对于文件传输窗口里的所选项的传输过程的停止*/
        void on_button_stop();
        /** 对于文件传输窗口里的所选项的删除*/
        void on_button_del();
        /** 对于文件传输窗口里的所选项的传输继续，如果传输未完成的情况下*/
        void on_button_continue();
        /** 文件传输窗口的按钮处理*/
        bool on_key_press_event(GdkEventKey* ev);
        bool on_delete_event(GdkEventAny*);

        Gtk::TreeModel::iterator getListIter(Gtk::TreeModel::
                                             Children children,
                                             const Glib::ustring & f_sid);
        /**
         * @brief 添加新的文件传输选项
         * @param f_sid 此次传输的sid
         * @param f_filename 此次传输的文件的文件名
         * @param f_target 传输的对方的名字
         * @param f_size 传输文件的大小
         * @param f_type 正在传输的状态
         */
        void addXfer(const Glib::ustring& f_sid, const std::string& f_filename, const std::string& f_target, long f_size, const std::string& type);
        /**
         * @brief 更新传输选项状态
         * @param f_sid 传输的sid号
         * @param percent 传输状态的百分比
         */
        void updateXfer(const Glib::ustring& f_sid, int percent);
        /**
         * @brief 完成传输设置
         * @param f_sid 传输的sid号
         * @param error 标识此次传输是否有错误，0为完成传输
         */
        void doneXfer(const Glib::ustring& f_sid, bool error = 0);
        /**将long类型的大小转化为可读的MB，GB之类的显示字符,取自libjingle代码*/
        std::string filesize_to_string(long size);

private:

class XferColumns: public Gtk::TreeModel::ColumnRecord
        {

        public:
                XferColumns()
                {
                        add
                                (m_icons);

                        add
                                (m_sid);

                        add
                                (m_filename);

                        add
                                (m_size);

                        add
                                (m_totalsize);

                        add
                                (m_showsize);

                        add
                                (m_target);

                        add
                                (m_percent);

                        add
                                (m_type);
                }

                /** 传输过程的图标显示*/
                Gtk::TreeModelColumn < Glib::RefPtr < Gdk::Pixbuf > >m_icons;
                /** 此传输的sid标识*/
                Gtk::TreeModelColumn < Glib::ustring> m_sid;
                /** 传输的文件的名字*/
                Gtk::TreeModelColumn < std::string> m_filename;
                /** 传输文件的总大小*/
                Gtk::TreeModelColumn <long > m_totalsize;
                /** 以可读的形式显示文件的大小*/
                Gtk::TreeModelColumn <std::string> m_showsize;
                /** 已经传输的数据的大小*/
                Gtk::TreeModelColumn <long > m_size;
                /** 传输的接收方*/
                Gtk::TreeModelColumn <Glib::ustring> m_target;
                /** 传输进行时的完成的文件百分比*/
                Gtk::TreeModelColumn <int > m_percent;
                /** 标识此次传输为发送或接收*/
                Gtk::TreeModelColumn <std::string> m_type;
        };

        XferColumns m_columns;

private:

struct CompareColumns: public binary_function < Gtk::TreeModel::Row,
                                const Glib::ustring, bool >
        {
                explicit CompareColumns(const XferColumns &
                                        column_): column(column_)
                {}

                bool operator () (const Gtk::TreeRow & lhs,
                                  const Glib::ustring & var) const
                {
                        return lhs[column.m_sid] == var;
                }

                const XferColumns & column;
        };

private:
        MainWindow* m_parent;
        Gtk::TreeView m_TreeView;
        Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
        Gtk::ScrolledWindow m_ScrolledWindow;
        Gtk::VBox m_VBox;
        Gtk::Frame* frame;
        Gtk::HButtonBox m_ButtonBox;
        Gtk::Button* m_Button_Quit;
        Gtk::Button* m_Button_Stop;
        Gtk::Button* m_Button_Delete;
        Gtk::Button* m_Button_Continue;

};

#endif
