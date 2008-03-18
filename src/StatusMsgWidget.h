/*
* =====================================================================================
*
*       Filename:  StatusMsgWidget.h
*
*    Description:  状态签名管理
*
*        Version:  1.0
*        Created:  2007年09月27日 20时11分23秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  STATUSMSGWIDGET_FILE_HEADER_INC
#define  STATUSMSGWIDGET_FILE_HEADER_INC

#include <gtkmm.h>
#include <libglademm/xml.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeselection.h>
#include <functional>
#include <vector>
#define statusmsg_ui     DATA_DIR"/ui/statusMsg.glade"

using namespace std;

typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class MainWindow;

class MsgLine;

class StatusMsgWidget: public Gtk::Window
{

public:
        StatusMsgWidget(MainWindow* parent_);
        virtual ~StatusMsgWidget();
        void on_button_ok();
        void on_button_cancel();
        void on_button_add();
        void on_button_del();
        void on_button_edit();
        void on_checkbutton_clicked();
        bool on_key_press_event(GdkEventKey* ev);

private:
        MainWindow* parent;
        MsgLine* msgline;
        Gtk::CheckButton* timeoutButton;

};

class MsgLine : public Gtk::TreeView
{

public:
        MsgLine();
        void init();
        Glib::ustring getLineMsg(Gtk::TreeModel::iterator iter);
        int getLineNumber(Gtk::TreeModel::iterator iter);
        void addLine(int num, const Glib::ustring& msg);
        void addLine(const Glib::ustring& msg);
        void editLine(const int num, const Glib::ustring& msg);
        void delLine(const int num);
        void delLine(Gtk::TreeModel::iterator iter);
        void Save();
        Gtk::TreeModel::iterator getListIter(Gtk::TreeModel::Children children,
                                             const int id);

protected:
        bool on_button_press_event(GdkEventButton *);

private:

struct MsgColumns: public Gtk::TreeModel::ColumnRecord {
                MsgColumns() {
                        add
                        (num);

                        add
                        (message);
                }

                Gtk::TreeModelColumn<Glib::ustring> message;
                Gtk::TreeModelColumn<int> num;
        };

private:
        MsgColumns columns;
        int number;
        Glib::RefPtr<Gtk::ListStore> m_liststore;


private:
        /** 比较Message*/

struct CompareMsg: public binary_function < Gtk::TreeModel::Row,
                                const int, bool > {
                explicit CompareMsg(const MsgColumns &
                                    column_): column(column_) {}

                bool operator () (const Gtk::TreeRow & lhs,
                                  const int num) const {
                        return lhs[column.num] == num;
                }

                const MsgColumns & column;
        };
};

#endif   /* ----- #ifndef STATUSMSGWIDGET_FILE_HEADER_INC  ----- */

