/*
* =====================================================================================
*
*       Filename:  MsgPage.cpp
*
*    Description:  显示聊天窗口的标签页
*
*        Version:  1.0
*        Created:  2007年06月29日 21时28分08秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:  cyclone
*
* =====================================================================================
*/

#include <glib/gi18n.h>
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/dialog.h>
#include "Bodies.h"
#include "MsgPage.h"
#include "CommandBar.h"
#include "MsgLog.h"
#include <assert.h>
#include <sigc++/connection.h>
#include "pixmaps.h"
#include "sounds.h"

#define MSG_ME 1
#define TIME_NULL ""

MsgPage::MsgPage(const std::string& title, Buddy* f_buddy):
                m_buddy(f_buddy),
                m_mucroom(NULL),
                m_memberList(NULL),
                hasColor(false),
                m_titlelable(NULL),
                isRoom(0)

{
        const std::string& jid_ = m_buddy->get_jid();
        m_msgLog = new MsgLog(jid_);

        Gtk::VPaned* vPaned = Gtk::manage(new class Gtk::VPaned());
        pack1(*vPaned);

        //设置右边的图片栏
        Gtk::VBox* rightVbox = Gtk::manage(new Gtk::VBox());
        rightVbox->set_size_request(90, -1);
        pack2(*rightVbox);
        Glib::RefPtr<Gdk::Pixbuf> pix = m_buddy->getLogo();
        m_logo = Gtk::manage(new Gtk::Image(pix));
        Gtk::AspectFrame* aspectframe1 = Gtk::manage(new Gtk::AspectFrame());

        aspectframe1->add
        (*m_logo);

        rightVbox->pack_start(*aspectframe1);

        //rightVbox->pack_start(*m_logo);

        Glib::RefPtr<Gdk::Pixbuf> pix2 = Bodies::Get_Bodies().get_main_window().getLogo();

        Gtk::Image* logo2 = Gtk::manage(new Gtk::Image(pix2));

        Gtk::AspectFrame* aspectframe2 = Gtk::manage(new Gtk::AspectFrame());

        aspectframe2->add
        (*logo2);

        rightVbox->pack_end(*aspectframe2);

        //rightVbox->pack_end(*logo2);


        //设置消息显示TextView

        m_msgBox = Gtk::manage(new class MsgBox);

        Gtk::ScrolledWindow* scroll_out = Gtk::manage(new class Gtk::ScrolledWindow());

        scroll_out->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        scroll_out->add
        (*m_msgBox);

        m_msgBox->unset_flags(Gtk::CAN_FOCUS);

        m_msgBox->set_accepts_tab(false);

        m_msgBox->set_editable(false);

        Gtk::HBox* hbox2 = Gtk::manage(new Gtk::HBox());

        /* add frame*/

        Gtk::Frame* frame1 = Gtk::manage(new class Gtk::Frame());

        frame1->set_shadow_type(Gtk::SHADOW_IN);

        frame1->add
        (*scroll_out);

        hbox2->pack_start(*frame1);

        vPaned->pack1(*hbox2);


        //设置消息输入的TextView,它被包含在一个HBox里，右边还有一个textview用来装发送按钮
        Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());

        vPaned->pack2(*vbox, Gtk::SHRINK);

        Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox());

        vbox->pack_start(*hbox);

        Gtk::HButtonBox* hbbox = Gtk::manage(new Gtk::HButtonBox(Gtk::BUTTONBOX_END));

        vbox->pack_start(*hbbox, false, false);


        Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox());

        hbox->pack_start(*vbox2);

        CommandBar* toolbox = Gtk::manage(new class CommandBar(this));

        vbox2->pack_start(*toolbox, Gtk::PACK_SHRINK);

        m_inputMsgBox = Gtk::manage(new class MsgBox);

        Gtk::ScrolledWindow* scroll_in = Gtk::manage(new class Gtk::ScrolledWindow());

        scroll_in->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        scroll_in->add
        (*m_inputMsgBox);

        m_inputMsgBox->set_flags(Gtk::HAS_FOCUS);

        m_inputMsgBox->signal_focus_in_event().connect(
                sigc::mem_fun(*this, &MsgPage::on_inputMsgBox_focus_in_event));

        m_inputMsgBox->signal_focus_out_event().connect(
                sigc::mem_fun(*this, &MsgPage::on_inputMsgBox_focus_out_event));

        Gtk::HBox* hbox3 = Gtk::manage(new Gtk::HBox());

        Gtk::Frame* frame2 = Gtk::manage(new class Gtk::Frame());

        frame2->set_shadow_type(Gtk::SHADOW_IN);

        frame2->add
        (*hbox3);

        hbox3->pack_start(*scroll_in);

        scroll_in->set_size_request(360, -1);

        vbox2->pack_start(*frame2, true, true);

        Gtk::TextView* send_textview = Gtk::manage(new Gtk::TextView());

        send_textview->set_editable(false);

        send_textview->set_size_request(90, -1);

        hbox3->pack_end(*send_textview, false, false, 0);


        Gtk::Button* btSend = Gtk::manage(new Gtk::Button(_("Send(_S)"), true));

        //Gtk::Image* btSendImage = getImage("online.png");
        //btSend->set_image(*btSendImage);
        send_textview->add_child_in_window(*btSend, Gtk::TEXT_WINDOW_TEXT, 15, 25);

        btSend->signal_clicked().connect(sigc::mem_fun(*this, &MsgPage::sendMessage));

        btSend->set_flags(Gtk::CAN_FOCUS);

        btSend->set_relief(Gtk::RELIEF_NORMAL);

        btSend->set_size_request(60, 50);

        show_all();

        m_titlelable = Gtk::manage(new class Gtk::Label(Glib::Markup::escape_text(title), true));

        m_titlelable->set_use_markup(true);

        m_titlelable->set_single_line_mode(true);

        set_border_width(3);
}

MsgPage::MsgPage(const std::string& title, RoomItem* f_room, bool f_isRroom):
                m_mucroom(f_room),
                m_buddy(NULL),
                hasColor(false),
                m_titlelable(NULL),
                isRoom(f_isRroom)
{
        const std::string& jid_ = m_mucroom->getRoomJID();
        m_msgLog = new MsgLog(jid_);

        Gtk::VPaned* vPaned = Gtk::manage(new class Gtk::VPaned());
        pack1(*vPaned);

        //设置消息显示TextView

        m_msgBox = Gtk::manage(new class MsgBox);

        Gtk::ScrolledWindow* scroll_out = Gtk::manage(new class Gtk::ScrolledWindow());
        scroll_out->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        scroll_out->add
        (*m_msgBox);

        m_msgBox->unset_flags(Gtk::CAN_FOCUS);

        m_msgBox->set_accepts_tab(false);

        m_msgBox->set_editable(false);

        Gtk::HPaned* hpaned2 = Gtk::manage(new Gtk::HPaned());

        //hpaned2->set_border_width(5);

        /* add frame*/

        Gtk::Frame* frame1 = Gtk::manage(new class Gtk::Frame());

        frame1->set_shadow_type(Gtk::SHADOW_IN);

        frame1->add
        (*scroll_out);

        hpaned2->pack1(*frame1, Gtk::EXPAND);

        Gtk::VBox* bigbox = Gtk::manage(new Gtk::VBox());

        Gtk::HBox* subbox = Gtk::manage(new Gtk::HBox());

        Gtk::Label* sub_label = Gtk::manage(new Gtk::Label(_(" subject: ")));

        m_subject = Gtk::manage(new Gtk::Entry());

        subbox->pack_start(*sub_label, Gtk::PACK_SHRINK);

        subbox->pack_start(*m_subject);

        bigbox->pack_start(*subbox, 0, 0, 1);

        bigbox->pack_start(*hpaned2);

        vPaned->pack1(*bigbox);


        //添加聊天室成员列表

        m_memberList = Gtk::manage(new class MemberList);

        m_memberList->unset_flags(Gtk::HAS_FOCUS);

        Gtk::ScrolledWindow* scrolledwindowmemberList =

                Gtk::manage(new class Gtk::ScrolledWindow());

        scrolledwindowmemberList->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        scrolledwindowmemberList->add
        (*m_memberList);

        scrolledwindowmemberList->set_size_request(120, -1);

        Gtk::Frame* frame3 = Gtk::manage(new class Gtk::Frame());

        frame3->set_shadow_type(Gtk::SHADOW_IN);

        frame3->add
        (*scrolledwindowmemberList);

        Gtk::VBox* list_box = Gtk::manage(new class Gtk::VBox());

        m_list_number = Gtk::manage(new class Gtk::Label());

        //m_list_number->set_label("Many people chat room");
        int number = m_memberList->getMemberNumber();

        char buf[256];

        sprintf(buf, _("there has %d people in the room"), number);

        m_list_number->set_label(buf);

        m_list_number->set_justify(Gtk::JUSTIFY_CENTER);

        list_box->pack_start(*m_list_number, Gtk::PACK_SHRINK);

        list_box->pack_start(*frame3);

        //hpaned2->pack2(*frame3,Gtk::SHRINK);
        //将列表置于右边的架构中
        //pack2(*frame3, Gtk::SHRINK);
        pack2(*list_box, Gtk::SHRINK);

        //m_memberList->signal_button_press_event().connect(sigc::mem_fun(*this,
        //   &MsgPage::on_memberList_double_click_event),false);


        Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());

        vPaned->pack2(*vbox, Gtk::SHRINK);

        Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox());

        vbox->pack_start(*hbox);

        Gtk::HButtonBox* hbbox = Gtk::manage(new Gtk::HButtonBox(Gtk::BUTTONBOX_END));

        vbox->pack_start(*hbbox, false, false);

        Glib::RefPtr<Gdk::Pixbuf> pix = getPix("room.png");

        m_logo = Gtk::manage(new Gtk::Image(pix));

        Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox());

        hbox->pack_start(*vbox2);

        CommandBar* toolbox = Gtk::manage(new class CommandBar(this));

        vbox2->pack_start(*toolbox, Gtk::PACK_SHRINK);

        m_inputMsgBox = Gtk::manage(new class MsgBox);

        Gtk::ScrolledWindow* scroll_in = Gtk::manage(new class Gtk::ScrolledWindow());

        scroll_in->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        scroll_in->add
        (*m_inputMsgBox);

        m_inputMsgBox->set_flags(Gtk::HAS_FOCUS);

        m_inputMsgBox->signal_focus_in_event().connect(
                sigc::mem_fun(*this, &MsgPage::on_inputMsgBox_focus_in_event));

        Gtk::HBox* hbox3 = Gtk::manage(new Gtk::HBox());

        Gtk::Frame* frame2 = Gtk::manage(new class Gtk::Frame());

        frame2->set_shadow_type(Gtk::SHADOW_IN);

        frame2->add
        (*hbox3);

        hbox3->pack_start(*scroll_in);

        scroll_in->set_size_request(360, -1);

        vbox2->pack_start(*frame2, true, true);

        Gtk::TextView* send_textview = Gtk::manage(new Gtk::TextView());

        send_textview->set_editable(false);

        send_textview->set_size_request(90, -1);

        hbox3->pack_end(*send_textview, Gtk::PACK_SHRINK, 0);



        Gtk::Button* btSend = Gtk::manage(new Gtk::Button(_("Send(_S)"), true));

        //Gtk::Image* btSendImage = getImage("online.png");
        //btSend->set_image(*btSendImage);
        send_textview->add_child_in_window(*btSend, Gtk::TEXT_WINDOW_TEXT, 15, 25);

        btSend->signal_clicked().connect(sigc::mem_fun(*this, &MsgPage::sendMessage));

        btSend->set_flags(Gtk::CAN_FOCUS);

        btSend->set_relief(Gtk::RELIEF_NORMAL);

        btSend->set_size_request(60, 50);

        show_all();

        m_titlelable = Gtk::manage(new class Gtk::Label(Glib::Markup::escape_text(title), true));

        m_titlelable->set_use_markup(true);

        m_titlelable->set_single_line_mode(true);

        set_border_width(3);
}


MsgPage::~MsgPage()
{
        delete m_titlelable;
        delete m_logo;
        delete m_msgBox;
        delete m_inputMsgBox;
        delete m_msgLog;
}

void MsgPage::showHistroy(const std::string& sender_, const Glib::ustring& msg_, const std::string& time_)
{
        m_msgBox->showTitle(sender_, false);
        m_msgBox->showGrayMsg(msg_);
        m_msgBox->showGrayMsg("\n");

}

void MsgPage::showStatusBarMsg(const std::string& msg, unsigned int id)
{
        Bodies::Get_Bodies().get_msg_window().showStatusBarMsg(msg, id);
        // statusbar->pop(id);
        // statusbar->push(msg,id);
}

void MsgPage::showMessage(const std::string& sender_, const Glib::ustring& msg_, const std::string& time_, bool self)
{
        if (!isRoom) {
                Glib::RefPtr<Gdk::Pixbuf>pix_;

                if (self) {
                        setTitleColor(false);
                        pix_ = Bodies::Get_Bodies().get_main_window().getLogo()->scale_simple(30, 30, Gdk::INTERP_NEAREST);
                } else {
                        sounds::play(sounds::RECEIVE_SOUND);
                        setTitleColor(true);
                        //pix_ = m_buddy->getLogo()->scale_simple(30,30,Gdk::INTERP_NEAREST);
                }

                //Gtk::Image* image = Gtk::manage(new Gtk::Image(pix_));
                //m_msgBox->insertImage(image);
                if ((TYPE_GROUPCHAT != m_buddy->getType()) || self)
                        m_msgBox->showTitle(sender_, self);
        } else {
                sounds::play(sounds::RECEIVE_SOUND);
                setTitleColor(true);
                m_msgBox->showTitle(sender_, self);
        }

        m_msgBox->showMessage(msg_, time_);

        m_msgLog->write(sender_, msg_);
}

void MsgPage::showPicture(const char* picname, bool self)
{

        Glib::RefPtr < Gdk::Pixbuf > pic = Gdk::Pixbuf::create_from_file(picname);
        Gtk::Image* picimage = Gtk::manage(new Gtk::Image(pic));
        std::string sender;

        Glib::RefPtr<Gdk::Pixbuf>pix_;

        if (self) {
                setTitleColor(false);
                pix_ = Bodies::Get_Bodies().get_main_window().getLogo()->scale_simple(30, 30, Gdk::INTERP_NEAREST);
                sender = Bodies::Get_Bodies().get_jid().username();
        } else {
                sounds::play(sounds::RECEIVE_SOUND);
                setTitleColor(true);
                pix_ = m_buddy->getLogo()->scale_simple(30, 30, Gdk::INTERP_NEAREST);
                sender = m_buddy->get_nickname();

                if (sender.empty())
                        sender = m_buddy->getJID().username();
        }

        Gtk::Image* image = Gtk::manage(new Gtk::Image(pix_));

        m_msgBox->insertImage(image);
        m_msgBox->showTitle(sender, self);
        m_msgBox->insertImage(picimage);
        m_msgBox->showMessage("");

}

void MsgPage::sendMessage()
{
        Glib::ustring utext;
        m_inputMsgBox->getText(utext);

        if (utext.empty())
                return ;

        if (!isRoom) {
                /*然后发送utext*/

                if (m_buddy->get_session()) {
                        m_buddy->get_session()->send(utext);
                        /**发出停止打字信号*/
                        m_buddy->setChatState( ChatStatePaused );
                }
        } else {
                m_mucroom->send(utext);
        }


        if (!isRoom) {
                std::string sender = Bodies::Get_Bodies().get_jid().username();
                showMessage(sender, utext, TIME_NULL, MSG_ME);
                sounds::play(sounds::SEND_SOUND);
        }

        m_inputMsgBox->clear();
}

void MsgPage::deleteWord(int type)
{
        m_inputMsgBox->readline(type);
}

void MsgPage::clear()
{
        m_msgBox->clear();
}

bool MsgPage::on_inputMsgBox_focus_in_event(GdkEventFocus* ev)
{
        setTitleColor(false);
        /** 发送正在打字的事件*/

        if (!isRoom && m_buddy)
                //m_buddy->raiseMessageEvent(MessageEventComposing);
                return false;
}

bool MsgPage::on_inputMsgBox_focus_out_event(GdkEventFocus* ev)
{
        /** 发送取消打字的事件*/

        if (!isRoom && m_buddy)
                m_buddy->raiseMessageEvent(MessageEventCancel);

        return false;

}

void MsgPage::setTitleColor(bool hascolor)
{
        Bodies::Get_Bodies().get_msg_window().setNewMsgTitle(hascolor);

        if ((NULL != m_titlelable) && (hascolor != hasColor)) {
                if (hascolor && !(m_inputMsgBox->is_focus())) {
                        char* marktext = g_markup_printf_escaped("<span color='red'>%s</span>",
                                         m_titlelable->get_text().c_str());
                        m_titlelable->set_label(marktext);
                        hasColor = true;
                        g_free(marktext); // C free
                } else {
                        m_titlelable->set_label(m_titlelable->get_text());
                        hasColor = false;
                }
        }
}

void MsgPage::close()
{
        m_msgBox->clear();
        m_inputMsgBox->clear();

        if (!isRoom) {
                /**发出关闭对话框的信号给对方*/
                //m_buddy->setChatState( ChatStateGone );
                m_buddy->close_session();
                m_buddy = NULL;
        } else {
                m_mucroom->closePage();
        }

        Bodies::Get_Bodies().get_msg_window().del_page(*this);
}


void MsgPage::addMember(const std::string& name, const Member& f_member)
{
        m_memberList->addMember(name, f_member);
        int number = m_memberList->getMemberNumber();
        char buf[256];
        sprintf(buf, _("there has %d people in the room"), number);
        m_list_number->set_label(buf);
}

void MsgPage::removeMember(const std::string& id)
{
        m_memberList->removeMember(id);
        int number = m_memberList->getMemberNumber();
        char buf[256];
        sprintf(buf, _("there has %d people in the room"), number);
        m_list_number->set_label(buf);
}

void MsgPage::refreshMember()
{
        const MemberMap& items = m_mucroom->getMemberMap();

        m_memberList->clearMember();
        MemberMap::const_iterator it = items.begin();

        for (; it != items.end(); ++it)
                m_memberList->addMember((*it).first, (*it).second);

        int number = m_memberList->getMemberNumber();

        char buf[256];

        sprintf(buf, _("there has %d people in the room"), number);

        m_list_number->set_label(buf);

}

bool MsgPage::on_memberList_double_click_event(GdkEventButton* ev)
{
        const std::string mid = m_memberList->getSelectMemberid();


        if ((ev->type == GDK_2BUTTON_PRESS ||
                        ev->type == GDK_3BUTTON_PRESS)) {
                std::cout << "点击成员" << mid << std::endl;
        }

}

void MsgPage::setSubject()
{
        Glib::ustring sub_ = m_mucroom->getSubject();
        m_subject->set_text(sub_);
}

void MsgPage::on_toolbar_fonts()
{
        m_buddy->sendPicture("/tmp/bot.png");
        printf(" buttons for fonts press\n");
}

void MsgPage::on_toolbar_link()
{}

void MsgPage::on_toolbar_smiley()
{}

void MsgPage::on_toolbar_image()
{
        if (NULL == m_buddy)
                return ;

        Gtk::FileChooserDialog dialog(_("Please select a file"),
                                      Gtk::FILE_CHOOSER_ACTION_OPEN);

        dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

        dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

        Gtk::FileFilter filter_png;

        filter_png.set_name("PNG files");

        filter_png.add_mime_type("image/png");

        dialog.add_filter(filter_png);

        Gtk::FileFilter filter_jpg;

        filter_jpg.set_name("JPG files");

        filter_jpg.add_mime_type("image/jpg");

        dialog.add_filter(filter_jpg);

        Gtk::FileFilter filter_gif;

        filter_gif.set_name("GIF files");

        filter_gif.add_mime_type("image/gif");

        dialog.add_filter(filter_gif);


        Gtk::FileFilter filter_any;

        filter_any.set_name("Any files");

        filter_any.add_mime_type("*");

        dialog.add_filter(filter_any);

        std::string filename ;

        //dialog.set_current_folder("~/Desktop");
        int result = dialog.run();

        switch (result) {
        case (Gtk::RESPONSE_OK): {
                        filename = dialog.get_filename(); //注意：这里取回的并不是Glib::ustring, 而是std::string.

                        break;
                }

        case (Gtk::RESPONSE_CANCEL): {
                        std::cout << "Cannel choose icon" << std::endl;
                        return ;
                        //break;
                }

        default: {

                        std::cout << "Cannel choose icon" << std::endl;
                        return ;
                        //break;
                }
        }

        std::cout << "选择文件： " << filename << std::endl;

        m_buddy->sendPicture(filename);
}
