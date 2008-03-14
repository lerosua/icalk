/*
* =====================================================================================
* 
*       Filename:  MsgBox.h
* 
*    Description:  专门的TextView，专心处理输入输出的类
* 
*        Version:  1.0
*        Created:  2007年07月17日 22时44分11秒 CST
*       Revision:  none
*       Compiler:  g++
* 
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
* 
* =====================================================================================
*/

#ifndef  MSGBOX_FILE_HEADER_INC
#define  MSGBOX_FILE_HEADER_INC
#include <string>
#include <gtkmm/textview.h>
#include <gtkmm/image.h>
#include "TagList.h"
#include "SmileTree.h"

#define DELETE_CHAR 0
#define DELETE_WORD 1
#define DELETE_LINE 2 
/**
 * @brief 继承自TextView，处理消息输出的格式
 */

class MsgBox: public Gtk::TextView
{

public:
        /** 颜色表*/
        typedef Glib::RefPtr<Gtk::TextBuffer::TagTable> RefTagTable;
        typedef Glib::RefPtr<Gtk::TextTag> RefTag;
        MsgBox();

        /**
         * @brief 显示消息,比较上层,将分析其中的消息
         * @param msg_text 文本消息
         */
        void showMessage(const Glib::ustring& msg_text, const std::string& time_ = "");
        void showMessage(Glib::ustring::const_iterator first, Glib::ustring::const_iterator last);
        /**
         * @brief 显示简单的文本消息，最低层
         * @param msg_text 文本消息
         */
        void showSimpleMessage(const Glib::ustring& msg_text);
        /**
         * @brief 显示消息时前面的图标和用户名
         * @param sender 发送者ID
         * @param self   标识是否本人发的消息。为真则是本人发的消息
         */
        void showTitle(const std::string& sender, bool self = 0);
        void showTime(const std::string& time_ = "");
        /**
         * @brief 显示系统的消息
         * @param msg_ 系统消息文本
         */
        void showSystemMsg(const std::string& msg_ );
        /**
         * @brief 显示灰色字体的消息，用于显示聊天室里的历史消息
         * @param msg_ 消息文本
         */
        void showGrayMsg(const std::string& msg_);
        /**
         * @brief 插入图像
         * @param image 图像文件
         */
        void insertImage(Gtk::Image* image);
        /**
         * @brief 播放笑脸的代码
         * @param code 笑脸代码
         */
        void insertSmiley(const Glib::ustring& code);
        /**
         * @brief 获取本TextView的文本
         */
        void getText(Glib::ustring&);
        /**
         * @brief 模拟readline中的快捷键删除
         */
        void readline(int type);
        /**
         * 清空本TextView
         */
        void clear();

        SMILELIST& getSmileMap()
        {
                return smileTree.getSmileList();
        }

private:
        icalk::TagList taglist;
        SmileTree<Glib::ustring::value_type> smileTree;

};

#endif   /* ----- #ifndef MSGBOX_FILE_HEADER_INC  ----- */

