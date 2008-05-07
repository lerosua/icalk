/*
* =====================================================================================
*
*       Filename:  MsgPage.h
*
*    Description:  显示聊天窗口的标签页
*
*        Version:  1.0
*        Created:  2007年06月29日 21时21分32秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:  cyclone
*
* =====================================================================================
*/

#ifndef _MSG_PAGE_H_
#define _MSG_PAGE_H_

#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA
#include <gtkmm/accelgroup.h>


class GlademmData
{

        Glib::RefPtr < Gtk::AccelGroup > accgrp;

public:

        GlademmData(Glib::RefPtr < Gtk::AccelGroup > ag): accgrp(ag)
        {}

        Glib::RefPtr < Gtk::AccelGroup > getAccelGroup()
        {
                return accgrp;
        }
};

#endif    //GLADEMM_DATA


#include <gtkmm.h>
#include <iostream>
#include <gloox/mucroom.h>
#include "MsgBox.h"
//#include "CommandBar.h"
#include "MemberList.h"
//#include "MsgLog.h" 
//#include "RoomItem.h"

using namespace gloox;

class Buddy;

class RoomItem;
class CommandBar;
class MsgLog;
/**
 * @brief 构造聊天窗口中的某一页，与某一会话相对应，嵌入到聊天窗口的笔记本控件中
 *
 */

class MsgPage: public Gtk::HPaned
{

public:
        typedef Glib::RefPtr < Gtk::TextBuffer::Tag > RefTag;


        /**
         * @brief MsgPage的Buddy的构造函数
         * @param title 标签页的标题
         * @param f_buddy 与之通信的Buddy成员
         */
        MsgPage(const std::string & title, Buddy * f_buddy);
        /**
         * @brief MsgPage的聊天室的构造函数
         * @param title 标签页的标题
         * @param f_isRroom 是否生成聊天室,默认当然是生成啦
         */
        MsgPage(const std::string & title, RoomItem * f_room, bool f_isRroom);

        virtual ~ MsgPage();

        Gtk::Image & get_logo()
        {
                return *m_logo;
        }

        Gtk::Label & get_lable()
        {
                return *m_titlelable;
        }

        Gtk::Widget * getFocusWidge()
        {
                return m_inputMsgBox;
        }

        SMILELIST& getSmileMap()
        {
                return m_inputMsgBox->getSmileMap();
        }

        void insertSmiley(const Glib::ustring& code)
        {
                m_inputMsgBox->insertSmiley(code);
        }

        /**
         * @brief 显示信息函数的第一层处理
         * @param sender  发送者
         * @param msg   显示的消息
         * @param time   消息的时间
         * @param self   确认信息是不是自己发送的,为真则是自己发送的.
         */
        void showMessage(const std::string & sender,
                         const Glib::ustring & msg,
                         const std::string& time = ""
                                                   , bool self = 0);

        /**
         * @brief 显示图片
         * @param picname 图片的文件名
         * @param self 确认信息不是自己发送的。
         */
        void showPicture(const char* picname, bool self = 0);
        /**
         * @brief 显示状态栏里的信息
         * @param msg 信息内容
         * @note 主要用于显示用户的一些动作，聊天室里的离开，进入等
         */
        void showStatusBarMsg(const std::string & msg, unsigned int id =
                                      0);
        /**
         * @brief 用于显示聊天室里的历史信息
         * @param sender 发送者
         * @param msg_   消息内容
         * @param time_  消息发送时的时间戳
         */
        void showHistroy(const std::string & sender,
                         const Glib::ustring & msg_
                         , const std::string& time_ = "");
        /**
         * @brief 显示其它一些消息，如用户关闭窗口
         * @param msg_ 消息内容
         */
        void showSystemMsg(const std::string & msg)
        {
                m_msgBox->showSystemMsg(msg);
        }

        /**
         * @brief 高层的发送信息函数
         * 获取输入框中的文本，调用Buddy中的会话发送函数
         * @code
         *  m_buddy->get_session()->send(utext);
         * @endcode
         * 然后用@link showMessage showMessage() @endlink显示文件
         */
        void sendMessage();
        /**
         * @brief 设置Msgpaga标签页的颜色
         */
        void setTitleColor(bool hascolor);
        /**
         * @brief 清除显示框
         */
        void clear();
        void deleteWord(int type);
        /**
         * @brief 关闭标签（MsgPage)
         * 调用m_buddy.close_session()关闭会话
         * 再得到MsgWindow,用来删除MsgPage
         */
        void close();

        /** 设置聊天室的主题*/
        void setSubject();
        /** 更新聊天室内成员列表*/
        void refreshMember();
        /** 获取聊天室内的成员列表*/
        MemberList *getMemberList()
        {
                return m_memberList;
        }

        void addMember(const std::string& name, const Member& f_member);
        void removeMember(const std::string& id);

        /**用于输入窗口获取焦点时处理*/
        bool on_inputMsgBox_focus_in_event(GdkEventFocus * ev);
        /** 输入窗口失去焦点时的处理*/
        bool on_inputMsgBox_focus_out_event(GdkEventFocus * ev);
        /** 聊天室内成员列表被双击的事件处理*/
        bool on_memberList_double_click_event(GdkEventButton * ev);
        void on_toolbar_fonts();
        void on_toolbar_link();
        void on_toolbar_smiley();
        void on_toolbar_image();

private:
        MsgBox * m_msgBox;
        MsgBox *m_inputMsgBox;
        CommandBar *toolbar;
        Gtk::Image * m_logo;
        Gtk::Label * m_titlelable;
        Gtk::Entry * m_subject;
        bool hasColor;
        bool isRoom;

        Buddy *m_buddy;
        RoomItem *m_mucroom;
        MemberList *m_memberList;
        MsgLog *m_msgLog;

};

#endif    // _MSG_PAGE_H_
