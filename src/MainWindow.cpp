/*
 * =====================================================================================
 *
 *       Filename:  MainWindow.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年05月24日 19时43分28秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  (lerosua xihel), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */



#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <gdkmm/pixbuf.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/textview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/dialog.h>
#include <glib/gi18n.h>
#include <sigc++/connection.h>

#include "Bodies.h"
#include "MainWindow.h"
#include "BuddyView.h"
#include "pixmaps.h"
#include "sounds.h"
#include "about.h"
#include "Unit.h"

#define SYSTEM_MENU 0
#define TIMEOUT 5
typedef std::map < std::string, Resource * >ResourceMap;
MainWindow::MainWindow(Bodies & bodies_):
bodies(bodies_),
logo(NULL),
systemMenu(*this), statusCombo(NULL), statusEntry(NULL), trayMenu(*this)
{
	groalSet.MUTE = false;
	groalSet.SHOWALLFRIEND = false;
	groalSet.STATUS = LOGIN_INIT;
	groalSet.TIMECOUNT = 1;

	main_xml = Gnome::Glade::Xml::create(main_ui, "main_notebook");
	main_notebook =
	    dynamic_cast <
	    Gtk::Notebook * >(main_xml->get_widget("main_notebook"));
	main_notebook->set_current_page(0);
	main_notebook->set_show_tabs(false);
	/** 第二页标签*/
	Gtk::TextView * loading_textview =
	    dynamic_cast <
	    Gtk::TextView * >(main_xml->get_widget("loading_textview"));
	loading_textview->set_editable(false);
	loading_textview->set_cursor_visible(false);
	Glib::RefPtr < Gtk::TextBuffer > buffer =
	    loading_textview->get_buffer();
	buffer->set_text("\n\n\n\n\n\n\n\n\n\n\n\n                      ");
	Glib::RefPtr < Gtk::TextChildAnchor > anchor =
	    buffer->create_child_anchor(buffer->end());
	Gtk::Image * image = getImage("loading.gif");
	loading_textview->add_child_at_anchor(*image, anchor);
	image->show();

	Gtk::Button * bt_cancel = dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("button_cancel"));
	bt_cancel->signal_clicked().
	    connect(sigc::mem_fun(*this, &MainWindow::on_logining_cancel));

	/** 第一页标签*/
	Gtk::Button * button_cancel =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("login_cancel"));
	Gtk::Button * button_ok =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("login_ok"));
	comboAccount = Gtk::manage(new Gtk::ComboBoxEntryText());
	Gtk::HBox * hbox =
	    dynamic_cast <
	    Gtk::HBox * >(main_xml->get_widget("account_box"));
	hbox->pack_start(*comboAccount);
	entryPasswd =
	    dynamic_cast <
	    Gtk::Entry * >(main_xml->get_widget("entryPasswd"));
	comboAccount->signal_changed().
	    connect(sigc::mem_fun(*this, &MainWindow::on_account_changed));
	USERLIST & userlist = bodies.getUserList();
	USERLIST::iterator iter = userlist.begin();
	for (; iter != userlist.end(); iter++)
		comboAccount->append_text(*iter);

	keepMe = dynamic_cast<Gtk::CheckButton*>
		(main_xml->get_widget("rememberMe"));
	keeppasswd = dynamic_cast<Gtk::CheckButton*>
		(main_xml->get_widget("keeppasswd"));
	entryServer = dynamic_cast<Gtk::Entry*>
		(main_xml->get_widget("entry_server"));
	entryPort = dynamic_cast<Gtk::Entry*>
		(main_xml->get_widget("entry_port"));

	Gtk::Button* button_clean= dynamic_cast<Gtk::Button*>
		(main_xml->get_widget("button_clean"));
	
	button_clean->signal_clicked().
		connect(sigc::mem_fun(*this, &MainWindow::on_clean_entry));
	button_cancel->signal_clicked().
	    connect(sigc::mem_fun(*this, &MainWindow::on_quit));
	button_ok->signal_clicked().
	    connect(sigc::mem_fun(*this, &MainWindow::on_login));

	/** 第三页标签*/
	Gtk::Widget * widget = Gtk::manage(main_xml->get_widget("vbMain"));
	list_view = Gtk::manage(new BuddyView(*this));
	Gtk::Container * list_window =
	    dynamic_cast <
	    Gtk::Container * >(main_xml->get_widget("listWindow"));

	list_window->add(*list_view);

	statusEntry =
	    dynamic_cast <
	    Gtk::Entry * >(main_xml->get_widget("entry_status"));
	statusCombo =
	    dynamic_cast <
	    Gtk::ComboBox * >(main_xml->get_widget("combobox_status"));
	Glib::RefPtr < Gtk::ListStore > m_refTreeModel;
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	statusCombo->set_model(m_refTreeModel);
	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_Columns.col_status] = _("online");
	row = *(m_refTreeModel->append());
	row[m_Columns.col_status] = _("alway");
	row = *(m_refTreeModel->append());
	row[m_Columns.col_status] = _("leave");
	row = *(m_refTreeModel->append());
	row[m_Columns.col_status] = _("don't disturb");
	row = *(m_refTreeModel->append());
	row[m_Columns.col_status] = _("extend leave");
	row = *(m_refTreeModel->append());
	row[m_Columns.col_status] = _("offline");

	statusCombo->pack_start(m_Columns.col_status);
	statusCombo->set_active(0);
	statusCombo->signal_changed().
	    connect(sigc::mem_fun(*this, &MainWindow::on_combo_change));
	buttonSystem =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("button_sys"));
	buttonSystem->signal_clicked().
	    connect(sigc::
		    mem_fun(*this, &MainWindow::on_btnSystem_clicked));
	Gtk::Button * btAudio = dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("button_audio"));
	btAudio->signal_clicked().
	    connect(sigc::
		    mem_fun(*this, &MainWindow::on_btnAudio_clicked));

	Gtk::Button * buttonLogo =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("button_logo"));
	buttonLogo->signal_clicked().
	    connect(sigc::mem_fun(*this, &MainWindow::on_btnLogo_clicked));

	Gtk::Button * btstatusmsgmanager =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("statusmsgbt"));
	btstatusmsgmanager->signal_clicked().
	    connect(sigc::
		    mem_fun(*this,
			    &MainWindow::on_btstatusmsgmanager_clicked));


	Gtk::Button * btlistexpand =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("button_list_expand"));
	btlistexpand->signal_clicked().
	    connect(sigc::
		    mem_fun(*this, &MainWindow::on_btlistexpand_clicked));
	Gtk::Button * btlistshow =
	    dynamic_cast <
	    Gtk::Button * >(main_xml->get_widget("button_list_show"));
	btlistshow->signal_clicked().
	    connect(sigc::
		    mem_fun(*this,
			    &MainWindow::on_btlistshowoffline_clicked));
	set_default_size(200, 600);
	set_title(_("Friend list"));
	Glib::RefPtr < Gdk::Pixbuf > pix = getPix("default.png");
	set_icon(pix);
	add(*main_notebook);

	show_all();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_logining_cancel()
{
	bodies.disconnect();
	on_relogin();
}

void MainWindow::on_login()
{

	Glib::ustring name;
	Glib::ustring passwd;
	Glib::ustring server;
	Glib::ustring port;
	name = comboAccount->get_entry()->get_text();
	passwd = entryPasswd->get_text();
	server = entryServer->get_text();
	port   = entryPort->get_text();
	if (name.empty() || passwd.empty()) {
		return;
	}

	GUnit::init(name.c_str());
	bodies.loadAccountTag();

	if(keepMe->get_active()|keeppasswd->get_active())
	{
	USERLIST & userlist = bodies.getUserList();
	USERLIST::iterator iter =
	    find(userlist.begin(), userlist.end(), name);
	if (iter == userlist.end())
		bodies.saveUserList(name);
	if(!server.empty())
		bodies.setAccountTag("server",server);
	if(!port.empty())
		bodies.setAccountTag("port",port);
	if(keeppasswd->get_active())
	{
		bodies.setAccountTag("keeppasswd","true");
		bodies.setAccountTag("passwd",passwd);
	}
	else
	{
		bodies.setAccountTag("keeppasswd","false");
		bodies.setAccountTag("passwd","xxxxxxx");
	}
	}
	bodies.login(name, passwd);
}

void MainWindow::on_initalize()
{
	list_view->loadBlistTag();
	const std::string & sound = bodies.getAccountTag("sound");
	if (sound == "MUTE")
		sounds::mute(1);
	else
		sounds::mute(0);
}

void MainWindow::on_login_finial()
{
	main_notebook->set_current_page(LOGIN_FINIAL);
	groalSet.STATUS = LOGIN_FINIAL;
	const std::string & msgtimeout =
	    bodies.getAccountTag("MsgTimeout");
	if ("on" == msgtimeout)
		addStatusMsgTimeout();
}

void MainWindow::on_account_changed()
{
	if (!comboAccount->get_active_text().empty()) {
		Glib::ustring name = comboAccount->get_entry()->get_text();
		printf("select name %s\n",name.c_str());
		GUnit::init(name.c_str());
		bodies.loadAccountTag();
		std::string keep_passwd = bodies.getAccountTag("keeppasswd");
		if("true" == keep_passwd)
		{
			keepMe->set_active();
			keeppasswd->set_active();
			std::string passwd_ = bodies.getAccountTag("passwd");
			entryPasswd->set_text(passwd_);
			std::string server_ = bodies.getAccountTag("server");
			std::string port_ =   bodies.getAccountTag("port");
			if(!server_.empty())
				entryServer->set_text(server_);
			else
				entryServer->set_text("");
			if(!port_.empty())
				entryPort->set_text(port_);
			else
				entryPort->set_text("");
		}
		else
		{
			keepMe->set_active(false);
			keeppasswd->set_active(false);
		}



	}
}
void MainWindow::set_logo(const std::string & iconpath)
{
	//VCard *vcard = bodies.get_vcard();
	//Glib::RefPtr<Gdk::Pixbuf> pix;
	if (!iconpath.empty()) {
		logo = Gdk::Pixbuf::create_from_file(iconpath, 36, 36);

		/*
		   std::ifstream fin(iconpath.c_str(),ios::binary);
		   const std::string type="image/png";
		   std::string binval;
		   std::copy((std::istreambuf_iterator<char>(fin)),
		   std::istreambuf_iterator<char>(),
		   std::inserter(binval,binval.begin()));
		   fin.close();
		   vcard->setPhoto(type,binval);
		   bodies.get_cardManage().store_vcard(vcard);
		   */
	} else
		logo =
		    Gdk::Pixbuf::
		    create_from_file(DATA_DIR "/images/avatar.png", 36,
				     36);
	//Glib::RefPtr < Gdk::Pixbuf > border =
	//   getPix("border.png");
	//border->composite(logo,1,1,36,36,0,0,1,1,Gdk::INTERP_BILINEAR,255);
	//logo->composite(border,0,0,48,48,0,0,1,1,Gdk::INTERP_BILINEAR,255);

	Gtk::Image * logo_ =
	    dynamic_cast <
	    Gtk::Image * >(main_xml->get_widget("image_logo"));
	logo_->set(logo);
	//logo_->set(border);

}

void MainWindow::set_label()
{
	Gtk::Label * name_label =
	    dynamic_cast <
	    Gtk::Label * >(main_xml->get_widget("label_name"));
	name_label->set_use_markup(true);
	name_label->set_ellipsize(Pango::ELLIPSIZE_END);
	std::string text = bodies.get_jid().bare();
	char *marktext =
	    g_markup_printf_escaped
	    ("<span weight='heavy' color='#6cb349'>%s</span>",
	     text.c_str());
	name_label->set_label(marktext);
	g_free(marktext);

	/*顺便设置签名档 */
	const std::string msg = bodies.getAccountTag("message");
	if (!msg.empty())
		statusEntry->set_text(msg);

}

bool MainWindow::on_delete_event(GdkEventAny *)
{
	hide();
}

void MainWindow::on_clean_entry()
{
	comboAccount->get_entry()->set_text("");
	entryPasswd->set_text("");
	entryServer->set_text("");
	entryPort->set_text("");
	keepMe->set_active(false);
	keeppasswd->set_active(false);
}
void MainWindow::on_quit()
{
	//bodies.logout();
	//bodies.~Bodies();
	Gtk::Main::quit();
}

bool MainWindow::on_key_press_event(GdkEventKey * ev)
{
	if (ev->type != GDK_KEY_PRESS)
		return Gtk::Window::on_key_press_event(ev);
	switch (ev->keyval) {
	case GDK_Return:
	case GDK_KP_Enter:
		if (LOGIN_FINIAL == groalSet.STATUS)
			on_entryStatus_change();

		else if (LOGIN_INIT == groalSet.STATUS)
			on_login();
		break;
	default:
		return Gtk::Window::on_key_press_event(ev);
	}
}
void MainWindow::on_combo_change()
{
	int status = statusCombo->get_active_row_number();
	Presence::PresenceType status_ = (Presence::PresenceType) (status + 1);

	Glib::ustring msg_ = statusEntry->get_text();
	bodies.set_status(status_, msg_);
}

void MainWindow::on_entryStatus_change()
{
	int status = statusCombo->get_active_row_number();
	Presence::PresenceType status_ = (Presence::PresenceType) (status + 1);

	Glib::ustring msg_ = statusEntry->get_text();
	bodies.set_status(status_, msg_);
	if (!msg_.empty()) {
		char buf[512];
		snprintf(buf, 512, "%s/StatusMsgFile",
			 GUnit::getUserPath());
		std::ofstream outfile(buf, std::ios::app);
		outfile << msg_ << std::endl;
		outfile.close();
	}
}
void MainWindow::addStatusMsgTimeout()
{
	if (msgTimeout.connected())
		msgTimeout.disconnect();
	int delay = 60000;
	msgTimeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
								  &MainWindow::
								  statusMsgTimeout),
						    delay);
	bodies.setAccountTag("MsgTimeout", "on");
}

void MainWindow::delStatusMsgTimeout()
{
	if (msgTimeout.connected())
		msgTimeout.disconnect();
	bodies.setAccountTag("MsgTimeout", "off");
}

bool MainWindow::statusMsgTimeout()
{

	/** 用于计算是否达到TIMEOUT所指的分钟时间，因为超时是每分钟
	 * 调用，所以这里只有用检测的办法达到第N分钟才设置状态*/
	if (groalSet.TIMECOUNT != TIMEOUT) {
		groalSet.TIMECOUNT = groalSet.TIMECOUNT + 1;
		//printf(" timecout is %d\n",groalSet.TIMECOUNT);
		return true;
	}
	groalSet.TIMECOUNT = 1;

	char buf[512];
	guint number = 0;
	guint linenum;
	guint rannum = (guint) g_random_int();
	snprintf(buf, 512, "%s/StatusMsgFile", GUnit::getUserPath());
	std::ifstream msgfile(buf);
	if (!msgfile) {
		printf("not context\n");
		return true;
	}
	std::string line;
	if (msgfile) {
		while (getline(msgfile, line)) {
			//std::cout<<"read line is "<<line<<std::endl;
			number++;
		}
	}
	if (rannum > number)
		linenum = rannum % number;
	else
		linenum = rannum;
	//printf(" linenum(%d) = random(%d) de  line(%d) \n", linenum,rannum,number);
	//msgfile.seekg(0,std::ios::beg);
	msgfile.close();
	msgfile.open(buf);
	for (int i = 0; i <= linenum; i++) {
		getline(msgfile, line);
		//std::cout<<"read line is "<<i<<"  "<<line<<std::endl;
	}
	msgfile.close();

	/*
	   char buf[512];
	   snprintf(buf, 512, "/tmp/.icalkStatusMsgFileTimeout");
	   std::ifstream msgfile(buf);
	   if (!msgfile) {
	   printf("not context\n");
	   return true;
	   }
	   std::string line;
	   if (msgfile) {
	   getline(msgfile, line);
	   }
	   msgfile.close();
	 */
	std::cout << "set message is " << line << std::endl;

	setStatusMsg(line);
	return true;
}
void MainWindow::setStatusMsg(const std::string & msg)
{
	int status = statusCombo->get_active_row_number();
	Presence::PresenceType status_ = (Presence::PresenceType) (status + 1);
	bodies.set_status(status_, msg);
	statusEntry->set_text(msg);
}

void MainWindow::on_btstatusmsgmanager_clicked()
{
	StatusMsgWidget *statusMsg = new StatusMsgWidget(this);

}

void MainWindow::on_btlistexpand_clicked()
{
	list_view->expanded_list();
}

void MainWindow::on_btlistshowoffline_clicked()
{
	if (groalSet.SHOWALLFRIEND) {
		list_view->showOffline(false);
		groalSet.SHOWALLFRIEND = false;
	} else {
		list_view->showOffline(true);
		groalSet.SHOWALLFRIEND = true;
	}
}

void MainWindow::on_btnLogo_clicked()
{
	Gtk::FileChooserDialog dialog(_("Please select a file"),
				      Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

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

	Gtk::FileFilter filter_any;
	filter_any.set_name("Any files");
	filter_any.add_mime_type("*");
	dialog.add_filter(filter_any);

	int result = dialog.run();

	switch (result) {
	case (Gtk::RESPONSE_OK):
		{
			std::string filename = dialog.get_filename();	//注意：这里取回的并不是Glib::ustring, 而是std::string.
			this->set_logo(filename);
			bodies.setAccountTag("icon", filename);

		VCard *vcard = bodies.get_vcard();
		std::ifstream fin(filename.c_str(),ios::binary);
		   const std::string type="image/png";
		   std::string binval;
		   std::copy((std::istreambuf_iterator<char>(fin)),
		   std::istreambuf_iterator<char>(),
		   std::inserter(binval,binval.begin()));
		   fin.close();
		   std::cout<<"photo size is "<<binval.size()<<std::endl;
		   //vcard->setPhoto(type,binval);
		   vcard->setNickname("cyclone blog");
		   bodies.get_cardManage().store_vcard(vcard);


			break;
		}
	case (Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cannel choose icon" << std::endl;
			break;
		}
	default:
		{

			std::cout << "Cannel choose icon" << std::endl;
			break;
		}
	}
}


void MainWindow::on_btnSystem_clicked()
{
	systemMenu.popup(sigc::bind <
			 int >(sigc::
			       mem_fun(this,
				       &MainWindow::on_popup_menu_pos),
			       SYSTEM_MENU), 1, GDK_CURRENT_TIME);

}

void MainWindow::on_btnAudio_clicked()
{
	bodies.disco_test();
}

void MainWindow::on_buddyChat_activate()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();

	if (!selection->count_selected_rows())
		return;

	Gtk::TreeModel::iterator iter = selection->get_selected();

	Glib::ustring name = list_view->getIDfromIter(iter);
	Buddy *buddy = bodies.get_buddy_list().find_buddy(name);
	buddy->new_session();
	MsgPage *page_ = buddy->get_page();
	bodies.get_msg_window().add_page(*page_);
	bodies.get_msg_window().show();
	bodies.get_msg_window().setCurrentPage(page_);

}

void MainWindow::on_buddyNameEdit_activate()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();
	if (!selection->count_selected_rows())
		return;

	Gtk::TreeModel::iterator iter = selection->get_selected();
	Gtk::TreeModel::Path path(iter);
	Gtk::TreeViewColumn * tvc;
	list_view->get_cursor(path, tvc);

	list_view->cellrender_edited(true);
	list_view->set_cursor(path, *tvc, true);


}

void MainWindow::on_buddyLog_activate()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();

	if (!selection->count_selected_rows())
		return;
}

void MainWindow::on_buddyInfo_activate()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();

	if (!selection->count_selected_rows())
		return;
	Gtk::TreeModel::iterator iter = selection->get_selected();
	Glib::ustring name = list_view->getIDfromIter(iter);
	Buddy *buddy = bodies.get_buddy_list().find_buddy(name);
	BuddyInfoWindow *info = new BuddyInfoWindow(buddy);
}


void MainWindow::on_buddyRemove_activate()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();

	if (!selection->count_selected_rows())
		return;
	Gtk::TreeModel::iterator iter = selection->get_selected();
	Glib::ustring name = list_view->getIDfromIter(iter);

	Gtk::MessageDialog dialog(*this, _("delete friend"), false,
				  Gtk::MESSAGE_QUESTION,
				  Gtk::BUTTONS_OK_CANCEL);
	Glib::ustring msg =
	    _("You will delete the friend ") + name +
	    _(". are you ready to do this ?");
	dialog.set_secondary_text(msg);
	int result = dialog.run();
	switch (result) {
	case (Gtk::RESPONSE_OK):
		{
			const JID *jid = new JID(name);
			bodies.get_client().rosterManager()->
			    unsubscribe(*jid, "");
			bodies.get_client().rosterManager()->remove(*jid);
			break;
		}
	case (Gtk::RESPONSE_CANCEL):
		{
			break;
		}
	default:
		{
			break;
		}
	}




}
void MainWindow::on_buddyType_activate()
{

}
void MainWindow::on_addBuddy_activate()
{
	Glib::RefPtr < Gnome::Glade::Xml >
	    addDialog_xml =
	    Gnome::Glade::Xml::create(main_ui, "dialog_addBuddy");

	auto_ptr < Gtk::Dialog > addBuddyDialog(dynamic_cast <
						Gtk::Dialog *
						>(addDialog_xml->
						  get_widget
						  ("dialog_addBuddy")));

	Gtk::ComboBoxEntry * comboGroup =
	    dynamic_cast <
	    Gtk::ComboBoxEntry *
	    >(addDialog_xml->get_widget("add_entry_group"));

	class ModelColumns:public Gtk::TreeModel::ColumnRecord {
	      public:
		ModelColumns() {
			add(m_col_string);
		} Gtk::TreeModelColumn < Glib::ustring > m_col_string;
	};
	ModelColumns m_Columns;
	Glib::RefPtr < Gtk::ListStore > m_refTreeModel =
	    Gtk::ListStore::create(m_Columns);
	comboGroup->set_model(m_refTreeModel);
	Gtk::TreeModel::Row row;
	StringList groupList = list_view->getGroupList();
	StringList::iterator iter = groupList.begin();
	for (; iter != groupList.end(); iter++) {
		row = *(m_refTreeModel->append());
		row[m_Columns.m_col_string] = *iter;
	}
	comboGroup->set_text_column(m_Columns.m_col_string);

	addBuddyDialog->raise();
	int result = addBuddyDialog->run();
	switch (result) {
	case (Gtk::RESPONSE_OK):
		{
			std::cout << "OK clicked" << std::endl;
			Gtk::Entry * name_entry =
			    dynamic_cast <
			    Gtk::Entry *
			    >(addDialog_xml->get_widget("add_entry_name"));
			Gtk::Entry * alias_entry =
			    dynamic_cast <
			    Gtk::Entry *
			    >(addDialog_xml->
			      get_widget("add_entry_nickname"));

			Glib::ustring name = name_entry->get_text();
			Glib::ustring alias = alias_entry->get_text();
			Glib::ustring group =
			    comboGroup->get_entry()->get_text();
			printf("name_entry %s\n", name.c_str());
			printf("alias_entry %s\n", alias.c_str());
			if (!name.empty()) {
				JID *jid = new JID(name);
				StringList g;
				g.push_back(group);
				bodies.get_client().rosterManager()->
				    subscribe(*jid, alias, g);

			}


			break;
		}
	case (Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked" << std::endl;
			break;
		}
	default:
		{
			std::cout << "nothing clicked" << std::endl;
			break;
		}
	}

}

void MainWindow::initRoom()
{
	list_view->initRoomList();
}

void MainWindow::on_addRoom_activate()
{
	Glib::RefPtr < Gnome::Glade::Xml >
	    addRoom_xml =
	    Gnome::Glade::Xml::create(main_ui, "dialog_addRoom");
	auto_ptr < Gtk::Dialog > addRoomDialog(dynamic_cast <
					       Gtk::Dialog *
					       >(addRoom_xml->
						 get_widget
						 ("dialog_addRoom")));
	Gtk::Entry * name_entry =
	    dynamic_cast <
	    Gtk::Entry * >(addRoom_xml->get_widget("name_entry"));
	std::string name_ = bodies.get_jid().username();
	name_entry->set_text(name_);
	addRoomDialog->raise();
	int result = addRoomDialog->run();
	switch (result) {
	case (Gtk::RESPONSE_OK):
		{
			Gtk::Entry * room_entry =
			    dynamic_cast <
			    Gtk::Entry *
			    >(addRoom_xml->get_widget("room_entry"));
			Gtk::Entry * server_entry =
			    dynamic_cast <
			    Gtk::Entry *
			    >(addRoom_xml->get_widget("server_entry"));
			Gtk::Entry * passwd_entry =
			    dynamic_cast <
			    Gtk::Entry *
			    >(addRoom_xml->get_widget("pass_entry"));
			Gtk::Entry * alias_entry =
			    dynamic_cast <
			    Gtk::Entry *
			    >(addRoom_xml->get_widget("alias_entry"));

			if (room_entry->get_text().empty())
				return;

			ConferenceListItem CLitem;
			CLitem.name = alias_entry->get_text();
			if (CLitem.name.empty())
				CLitem.name = room_entry->get_text();
			CLitem.nick = name_entry->get_text();
			CLitem.password = passwd_entry->get_text();
			CLitem.jid =
			    room_entry->get_text() + "@" +
			    server_entry->get_text();
			CLitem.autojoin = true;


			list_view->setBlistTag("room", CLitem.jid,
					       "nickname", CLitem.nick);
			list_view->setBlistTag("room", CLitem.jid,
					       "roomname", CLitem.name);
			list_view->setBlistTag("room", CLitem.jid,
					       "passwd", CLitem.password);
			list_view->setBlistTag("room",CLitem.jid,
					"autojoin", "true");
			list_view->addRoom(CLitem);
			break;
		}
	case (Gtk::RESPONSE_CANCEL):
		{
			break;
		}
	default:
		{
			break;
		}
	}

}
void MainWindow::on_findBuddy_activate()
{
	printf("on_findBuddy_activeate clicked\n");
}

void MainWindow::on_serverDisco_activate()
{
	//ServerDiscoWindow* discowindow = new ServerDiscoWindow(this);
}

void MainWindow::on_freshLIst_activate()
{
	list_view->refreshList();
}

void MainWindow::on_about_activate()
{
	new createAboutDialog();

}

void MainWindow::on_setPrefer_activate()
{
	printf("on _setPrefer_clicked \n");
}

void MainWindow::on_sound_activate()
{
	Gtk::CheckMenuItem * melem =
	    dynamic_cast < Gtk::CheckMenuItem * >(systemMenu.get_active());
	if (melem->get_active()) {
		sounds::mute(1);
		groalSet.MUTE = true;
		bodies.setAccountTag("sound", "MUTE");
	} else {
		sounds::mute(0);
		groalSet.MUTE = false;
		bodies.setAccountTag("sound", "ON");
	}

}

void MainWindow::on_show_all_friends()
{
	Gtk::CheckMenuItem * melem =
	    dynamic_cast < Gtk::CheckMenuItem * >(systemMenu.get_active());


	if (melem->get_active()) {
		if (groalSet.SHOWALLFRIEND)
			return;
		else {
			list_view->showOffline(true);
			groalSet.SHOWALLFRIEND = true;
		}
	} else {
		if (groalSet.SHOWALLFRIEND) {
			list_view->showOffline(false);
			list_view->showGroup(false);
			groalSet.SHOWALLFRIEND = false;
		} else
			return;

	}

}

void MainWindow::show_tray_menu(guint button, guint activate_time)
{
	trayMenu.popup(button, activate_time);
}
void MainWindow::on_popup_menu_pos(int &x, int &y, bool & push_in, int id)
{
	GtkWidget *active;
	GtkWidget *child;
	GtkRequisition requisition;
	GList *children;

	int menu_xpos;
	int menu_ypos;
	int menu_height;

	GtkWidget *widget;
	GtkMenu *menu;
	if (SYSTEM_MENU == id) {
		widget = GTK_WIDGET(buttonSystem->gobj());
		menu = systemMenu.gobj();
	} else {
		//widget = GTK_WIDGET(buttonUser->gobj());
		//menu= userMenu.gobj();
	}


	gtk_widget_get_child_requisition(GTK_WIDGET(menu), &requisition);
	menu_height = requisition.height;

	gdk_window_get_origin(widget->window, &menu_xpos, &menu_ypos);

	menu_xpos += widget->allocation.x + widget->allocation.width / 2;
	menu_ypos +=
	    widget->allocation.y - menu_height +
	    widget->allocation.height / 2;


	if (menu_xpos < 0)
		menu_xpos = 0;

	if (menu_ypos < 0)
		menu_ypos = 0;

	x = menu_xpos;
	y = menu_ypos;
	push_in = true;
}


void MainWindow::toggle_visibility()
{
	if (this->is_visible()) {
		this->get_position(win_x, win_y);
		this->hide();
	} else {
		this->move(win_x, win_y);
		this->show();
	}
}

void MainWindow::on_block_room()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();

	if (!selection->count_selected_rows())
		return;

	Gtk::TreeModel::iterator iter = selection->get_selected();
	Glib::ustring id = list_view->getIDfromIter(iter);
	RoomItem *room = bodies.getRoomHandler().findRoom(id);
	room->leave();
	list_view->setBlistTag("room",id,
				"autojoin", "false");



}

void MainWindow::on_block_friend()
{
}
void MainWindow::on_delRoom_activate()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
	    list_view->get_selection();

	if (!selection->count_selected_rows())
		return;
	Gtk::TreeModel::iterator iter = selection->get_selected();
	Glib::ustring jid = list_view->getIDfromIter(iter);
	RoomItem *room = bodies.getRoomHandler().findRoom(jid);
	room->leave();
	list_view->delRoom(jid);
}
