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
#include <assert.h>
#include <sigc++/connection.h>
#include "pixmaps.h"
#include "sounds.h"

#define MSG_ME 1
#define TIME_NULL ""

MsgPage::MsgPage(const std::string& title, Buddy* buddy_):
	buddy(buddy_),
	mucroom(NULL),
	memberList(NULL),
	hasColor(false),
	titlelable(NULL),
	isRoom(0)

							  
{
	const std::string& jid_ = buddy->get_jid();
	msglog = new MsgLog(jid_);

	Gtk::VPaned* vPaned=Gtk::manage(new class Gtk::VPaned());
	pack1(*vPaned);

	//设置右边的图片栏
	Gtk::VBox* rightVbox = Gtk::manage(new Gtk::VBox());
	pack2(*rightVbox);
	Glib::RefPtr<Gdk::Pixbuf> pix = buddy->getLogo();
	logo = Gtk::manage(new Gtk::Image(pix));
	rightVbox->pack_start(*logo);

	Glib::RefPtr<Gdk::Pixbuf> pix2 = Bodies::Get_Bodies().get_main_window().getLogo();
	Gtk::Image* logo2 = Gtk::manage(new Gtk::Image(pix2));
	rightVbox->pack_end(*logo2);


	msgBox = Gtk::manage(new class MsgBox);
	Gtk::ScrolledWindow* scroll_out = Gtk::manage(new class Gtk::ScrolledWindow());
	scroll_out->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scroll_out->add(*msgBox);
	msgBox->unset_flags(Gtk::CAN_FOCUS);
	msgBox->set_accepts_tab(false);
	msgBox->set_editable(false);
	//msgBox->set_border_width(2);
	Gtk::HBox* hbox2 = Gtk::manage(new Gtk::HBox());
	
	/* add frame*/
	Gtk::Frame* frame1 = Gtk::manage(new class Gtk::Frame());
	frame1->set_shadow_type(Gtk::SHADOW_IN);
	frame1->add(*scroll_out);

	hbox2->pack_start(*frame1);
	vPaned->pack1(*hbox2);
	

	Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());
	vPaned->pack2(*vbox, Gtk::SHRINK);
		
	Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox());
	vbox->pack_start(*hbox);

	Gtk::HButtonBox* hbbox = Gtk::manage(new Gtk::HButtonBox(Gtk::BUTTONBOX_END));
	vbox->pack_start(*hbbox, false, false);

	/* 聊天窗口 输入框左边的那个好友图标*/
	//Glib::RefPtr<Gdk::Pixbuf> pix = buddy->getLogo();
	//logo = Gtk::manage(new Gtk::Image(pix));

	Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox());
	hbox->pack_start(*vbox2);

	CommandBar* toolbox = Gtk::manage(new class CommandBar(this));
	vbox2->pack_start(*toolbox,Gtk::PACK_SHRINK);

	inputMsgBox = Gtk::manage(new class MsgBox);
	Gtk::ScrolledWindow* scroll_in = Gtk::manage(new class  Gtk::ScrolledWindow());
	scroll_in->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scroll_in->add(*inputMsgBox);
	inputMsgBox->set_flags(Gtk::HAS_FOCUS);
	//inputMsgBox->set_border_width(2);
	inputMsgBox->signal_focus_in_event().connect(
			sigc::mem_fun(*this,&MsgPage::on_inputMsgBox_focus_in_event));
	inputMsgBox->signal_focus_out_event().connect(
			sigc::mem_fun(*this,&MsgPage::on_inputMsgBox_focus_out_event));
	
	Gtk::HBox* hbox3 = Gtk::manage(new Gtk::HBox());
	vbox2->pack_start(*hbox3, true, true);

	Gtk::Frame* frame2 = Gtk::manage(new class Gtk::Frame());
	frame2->set_shadow_type(Gtk::SHADOW_IN);
	frame2->add(*scroll_in);
	hbox3->pack_start(*frame2);

	Gtk::VBox* vbox3 = Gtk::manage(new Gtk::VBox());
	hbox3->pack_start(*vbox3,Gtk::PACK_SHRINK,0);

	Gtk::Button* btClose = Gtk::manage(new Gtk::Button(Gtk::Stock::CLOSE));
	vbox3->pack_end(*btClose);

	Gtk::Button* btSend = Gtk::manage(new Gtk::Button(_("Send(_S)"), true));
	Gtk::Image* btSendImage = getImage("online.png");
	btSend->set_image(*btSendImage);
	vbox3->pack_start(*btSend);

	btSend->signal_clicked().connect(sigc::mem_fun(*this,&MsgPage::sendMessage));

	btClose->signal_clicked().connect(sigc::mem_fun(*this,&MsgPage::close));
	btSend->set_flags(Gtk::CAN_FOCUS);
	btSend->set_relief(Gtk::RELIEF_NORMAL);
	btClose->set_flags(Gtk::CAN_FOCUS);
	btClose->set_relief(Gtk::RELIEF_NORMAL);

	statusbar = Gtk::manage(new class Gtk::Statusbar());
	vbox->pack_end(*statusbar,Gtk::PACK_SHRINK,0);

	show_all();

	titlelable = Gtk::manage(new class Gtk::Label(Glib::Markup::escape_text(title),true));
	titlelable->set_use_markup(true);
	titlelable->set_single_line_mode(true);

	set_border_width(5);
}

MsgPage::MsgPage(const std::string& title,RoomItem* room_,bool isRoom_):
	mucroom(room_),
	buddy(NULL),
	hasColor(false),
	titlelable(NULL),
	isRoom(isRoom_)
							  
{
	const std::string& jid_ = mucroom->getRoomJID();
	msglog = new MsgLog(jid_);

	Gtk::VPaned* vPaned=Gtk::manage(new class Gtk::VPaned());
	pack1(*vPaned);

	msgBox = Gtk::manage(new class MsgBox);
	Gtk::ScrolledWindow* scroll_out = Gtk::manage(new class Gtk::ScrolledWindow());
	scroll_out->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scroll_out->add(*msgBox);
	msgBox->unset_flags(Gtk::CAN_FOCUS);
	msgBox->set_accepts_tab(false);
	msgBox->set_editable(false);
	//msgBox->set_border_width(2);
	Gtk::HPaned* hpaned2 = Gtk::manage(new Gtk::HPaned());
	hpaned2->set_border_width(5);
	
	/* add frame*/
	Gtk::Frame* frame1 = Gtk::manage(new class Gtk::Frame());
	frame1->set_shadow_type(Gtk::SHADOW_IN);
	frame1->add(*scroll_out);

	hpaned2->pack1(*frame1, Gtk::EXPAND);
	Gtk::VBox* bigbox = Gtk::manage(new Gtk::VBox());
	Gtk::HBox* subbox = Gtk::manage(new Gtk::HBox());
	Gtk::Label* sub_label= Gtk::manage(new Gtk::Label(_(" subject: ")));
	subject = Gtk::manage(new Gtk::Entry());
	subbox->pack_start(*sub_label,Gtk::PACK_SHRINK);
	subbox->pack_start(*subject);
	bigbox->pack_start(*subbox,0,0,1);
	bigbox->pack_start(*hpaned2);
	vPaned->pack1(*bigbox);


	//添加聊天室成员列表
		memberList = Gtk::manage(new class MemberList);
		memberList->unset_flags(Gtk::HAS_FOCUS);

		Gtk::ScrolledWindow* scrolledwindowmemberList = 
			Gtk::manage(new class Gtk::ScrolledWindow());
		scrolledwindowmemberList->set_policy(Gtk::POLICY_AUTOMATIC,Gtk::POLICY_AUTOMATIC);
		scrolledwindowmemberList->add(*memberList);
		scrolledwindowmemberList->set_size_request(120,-1);

	Gtk::Frame* frame3 = Gtk::manage(new class Gtk::Frame());
	frame3->set_shadow_type(Gtk::SHADOW_IN);
	frame3->add(*scrolledwindowmemberList);
		//hpaned2->pack2(*frame3,Gtk::SHRINK);
	//将列表置于右边的架构中
	pack2(*frame3,Gtk::SHRINK);
		//memberList->signal_button_press_event().connect(sigc::mem_fun(*this,
		//			&MsgPage::on_memberList_double_click_event),false);


	Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());
	vPaned->pack2(*vbox, Gtk::SHRINK);
		
	Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox());
	vbox->pack_start(*hbox);

	Gtk::HButtonBox* hbbox = Gtk::manage(new Gtk::HButtonBox(Gtk::BUTTONBOX_END));
	vbox->pack_start(*hbbox, false, false);

	Glib::RefPtr<Gdk::Pixbuf> pix = getPix("room.png");
	logo = Gtk::manage(new Gtk::Image(pix));

	Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox());
	hbox->pack_start(*vbox2);

	CommandBar* toolbox = Gtk::manage(new class CommandBar(this));
	vbox2->pack_start(*toolbox,Gtk::PACK_SHRINK);

	inputMsgBox = Gtk::manage(new class MsgBox);
	Gtk::ScrolledWindow* scroll_in = Gtk::manage(new class  Gtk::ScrolledWindow());
	scroll_in->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scroll_in->add(*inputMsgBox);
	inputMsgBox->set_flags(Gtk::HAS_FOCUS);
	//inputMsgBox->set_border_width(2);
	inputMsgBox->signal_focus_in_event().connect(
			sigc::mem_fun(*this,&MsgPage::on_inputMsgBox_focus_in_event));
	
	Gtk::HBox* hbox3 = Gtk::manage(new Gtk::HBox());
	vbox2->pack_start(*hbox3, true, true);

	Gtk::Frame* frame2 = Gtk::manage(new class Gtk::Frame());
	frame2->set_shadow_type(Gtk::SHADOW_IN);
	frame2->add(*scroll_in);
	hbox3->pack_start(*frame2);

	Gtk::VBox* vbox3 = Gtk::manage(new Gtk::VBox());
	hbox3->pack_start(*vbox3,Gtk::PACK_SHRINK,0);

	Gtk::Button* btClose = Gtk::manage(new Gtk::Button(Gtk::Stock::CLOSE));
	vbox3->pack_end(*btClose);

	Gtk::Button* btSend = Gtk::manage(new Gtk::Button(_("Send(_S)"), true));
	Gtk::Image* btSendImage = getImage("online.png");
	btSend->set_image(*btSendImage);
	vbox3->pack_start(*btSend);

	btSend->signal_clicked().connect(sigc::mem_fun(*this,&MsgPage::sendMessage));

	btClose->signal_clicked().connect(sigc::mem_fun(*this,&MsgPage::close));
	btSend->set_flags(Gtk::CAN_FOCUS);
	btSend->set_relief(Gtk::RELIEF_NORMAL);
	btClose->set_flags(Gtk::CAN_FOCUS);
	btClose->set_relief(Gtk::RELIEF_NORMAL);

	statusbar = Gtk::manage(new class Gtk::Statusbar());
	vbox->pack_end(*statusbar,Gtk::PACK_SHRINK,0);

	show_all();

	titlelable = Gtk::manage(new class Gtk::Label(Glib::Markup::escape_text(title),true));
	titlelable->set_use_markup(true);
	titlelable->set_single_line_mode(true);

	set_border_width(5);
}


MsgPage::~MsgPage()
{
	delete titlelable;
	delete logo;
	delete  msgBox;
	delete  inputMsgBox;
	delete msglog;
}

void MsgPage::showHistroy(const std::string& sender_, const Glib::ustring& msg_,const std::string& time_)
{
	msgBox->showTitle(sender_,false);
	msgBox->showGrayMsg(msg_);
	msgBox->showGrayMsg("\n");

}

void MsgPage::showStatusBarMsg(const std::string& msg,unsigned int id)
{
	statusbar->pop(id);
	statusbar->push(msg,id);
}

void MsgPage::showMessage(const std::string& sender_, const Glib::ustring& msg_, const std::string& time_,bool self)
{
	if(!isRoom){	
	Glib::RefPtr<Gdk::Pixbuf>pix_;
	if(self)
	{
	setTitleColor(false);
	pix_  = Bodies::Get_Bodies().get_main_window().getLogo()->scale_simple(30,30,Gdk::INTERP_NEAREST);
	}
	else{
	sounds::play(sounds::RECEIVE_SOUND);
	setTitleColor(true);
	//pix_ = buddy->getLogo()->scale_simple(30,30,Gdk::INTERP_NEAREST);
	}
	//Gtk::Image* image = Gtk::manage(new Gtk::Image(pix_));
	//msgBox->insertImage(image);
	if((TYPE_GROUPCHAT != buddy->getType())||self)
		msgBox->showTitle(sender_,self);
	}
	else
	{
	sounds::play(sounds::RECEIVE_SOUND);
	setTitleColor(true);
	msgBox->showTitle(sender_,self);
	}
	msgBox->showMessage(msg_,time_);
	msglog->write(sender_,msg_);
}

void MsgPage::showPicture(const char* picname,bool self)
{

	Glib::RefPtr < Gdk::Pixbuf > pic= Gdk::Pixbuf::create_from_file(picname);
	Gtk::Image* picimage = Gtk::manage(new Gtk::Image(pic));
	std::string sender;

	Glib::RefPtr<Gdk::Pixbuf>pix_;
	if(self)
	{
	setTitleColor(false);
	pix_  = Bodies::Get_Bodies().get_main_window().getLogo()->scale_simple(30,30,Gdk::INTERP_NEAREST);
	sender=Bodies::Get_Bodies().get_jid().username();
	}
	else{
	sounds::play(sounds::RECEIVE_SOUND);
	setTitleColor(true);
	pix_ = buddy->getLogo()->scale_simple(30,30,Gdk::INTERP_NEAREST);
	sender=buddy->get_nickname();
	if(sender.empty())
		sender = buddy->getJID().username();
	}
	Gtk::Image* image = Gtk::manage(new Gtk::Image(pix_));
	msgBox->insertImage(image);
	msgBox->showTitle(sender,self);
	msgBox->insertImage(picimage);
	msgBox->showMessage("");

}

void MsgPage::sendMessage()
{
	Glib::ustring utext;
	 inputMsgBox->getText(utext);

	 if(utext.empty())
		 return;
	 if(!isRoom){
	/*然后发送utext*/
		 if(buddy->get_session())
		 {
		buddy->get_session()->send(utext);
		/**发出停止打字信号*/
		buddy->setChatState( ChatStatePaused );
		 }
	 }
	 else{
		 mucroom->send(utext);
	 }


	 if(!isRoom){
	std::string sender = Bodies::Get_Bodies().get_jid().username(); 
	showMessage(sender,utext,TIME_NULL,MSG_ME);
	sounds::play(sounds::SEND_SOUND);
	 }
	inputMsgBox->clear();
}

void MsgPage::deleteWord(int type)
{
	inputMsgBox->readline(type);
}

void MsgPage::clear()
{
	msgBox->clear();
}

bool MsgPage::on_inputMsgBox_focus_in_event(GdkEventFocus* ev)
{
	setTitleColor(false);
	/** 发送正在打字的事件*/
	if(!isRoom&&buddy)
	//buddy->raiseMessageEvent(MessageEventComposing);
	return false;
}

bool MsgPage::on_inputMsgBox_focus_out_event(GdkEventFocus* ev)
{
	/** 发送取消打字的事件*/
	if(!isRoom&&buddy)
	//buddy->raiseMessageEvent(MessageEventCancel);
	return false;

}
void MsgPage::setTitleColor(bool hascolor)
{
	Bodies::Get_Bodies().get_msg_window().setNewMsgTitle(hascolor);
	if((NULL != titlelable) && (hascolor != hasColor))
	{
		if(hascolor && !(inputMsgBox->is_focus())){
			char* marktext = g_markup_printf_escaped("<span color='red'>%s</span>",
					titlelable->get_text().c_str());
			titlelable->set_label(marktext);
			hasColor = true;
			g_free(marktext); // C free
		}else {
			titlelable->set_label(titlelable->get_text());
			hasColor = false;
		}
	}
}

void MsgPage::close()
{
	msgBox->clear();
	inputMsgBox->clear();
	if(!isRoom){
	/**发出关闭对话框的信号给对方*/
	//buddy->setChatState( ChatStateGone );
	buddy->close_session();
	buddy=NULL;
	}
	else{
		mucroom->closePage();
	}

	Bodies::Get_Bodies().get_msg_window().del_page(*this);
}
 

void MsgPage::refreshMember()
{
	const MemberMap& items = mucroom->getMemberList();
	
	memberList->clearMember();
	//printf("已经清空列表？\n");
	MemberMap::const_iterator it = items.begin();
	for (; it != items.end(); ++it) 
		memberList->addMember((*it).first, (*it).second);	
	
}
bool MsgPage::on_memberList_double_click_event(GdkEventButton* ev)
{
	const std::string mid = memberList->getSelectMemberid();


	if ((ev->type == GDK_2BUTTON_PRESS ||
	     ev->type == GDK_3BUTTON_PRESS)) {
		std::cout<<"点击成员"<<mid<<std::endl;
	}

}

void MsgPage::setSubject()
{
	Glib::ustring sub_ = mucroom->getSubject();
	subject->set_text(sub_);
}

void MsgPage::on_toolbar_fonts()
{
	//	buddy->sendPicture("/tmp/bot.png");
	printf(" buttons for fonts press\n");
}
void MsgPage::on_toolbar_link(){}
void MsgPage::on_toolbar_smiley(){}
void MsgPage::on_toolbar_image()
{
	if(NULL == buddy)
		return;
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
	case (Gtk::RESPONSE_OK):
		{
			filename = dialog.get_filename();	//注意：这里取回的并不是Glib::ustring, 而是std::string.

			break;
		}
	case (Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cannel choose icon" << std::endl;
			return;
			//break;
		}
	default:
		{

			std::cout << "Cannel choose icon" << std::endl;
			return;
			//break;
		}
	}
			std::cout<<"选择文件： "<<filename<<std::endl;
			buddy->sendPicture(filename);
}
