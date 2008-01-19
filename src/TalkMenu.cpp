

#include <glib/gi18n.h>
#include <gtkmm/image.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/menuitem.h>
#include "TalkMenu.h"
#include "pixmaps.h"
#include "sounds.h"
#include "MainWindow.h"

TrayMenu::TrayMenu(MainWindow& wnd)
{
	// Setting up the UIManager:
	// 设置tray的右键菜单
	Glib::RefPtr < Gtk::ActionGroup > refActionGroup =
	    Gtk::ActionGroup::create();
	refActionGroup->add(Gtk::Action::create("Info",Gtk::Stock::INFO),
			sigc::mem_fun(wnd, &MainWindow::on_about_activate));
	refActionGroup->
	    add(Gtk::Action::
		create("Preferences", Gtk::Stock::PREFERENCES),
		sigc::mem_fun(wnd, &MainWindow::on_setPrefer_activate));
	refActionGroup->
	    add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
		sigc::mem_fun(wnd, &MainWindow::on_quit));
	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(refActionGroup);

	Glib::ustring ui_info =
	    "<ui>"
	    "  <popup name='Popup'>"
	    "    <menuitem action='Preferences' />"
	    "    <menuitem action='Info' />"
	    "    <separator/>"
	    "    <separator/>"
	    "    <menuitem action='Quit' />" "  </popup>" "</ui>";

	m_refUIManager->add_ui_from_string(ui_info);
}

void TrayMenu::popup(guint button,guint time_)
{
	Gtk::Menu * pMenu =
	    static_cast <
	    Gtk::Menu * >(m_refUIManager->get_widget("/Popup"));
	if (pMenu)
		pMenu->popup(button, time_);
}
BuddyMenu::BuddyMenu(MainWindow& wnd)
{
	/*菜单-信息*/
    Gtk::Image* image = getImage("info.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Information"),*image));
    //it's strange that gtk-info does'nt work
    Gtk::ImageMenuItem* itemInfo = (Gtk::ImageMenuItem*)&this->items().back();


    /*菜单-开聊*/
    image = getImage("menu_chat.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Chat(_C)"), *image));
    Gtk::ImageMenuItem* itemChat = (Gtk::ImageMenuItem *)&this->items().back();

    /*菜单-日志*/
    image = getImage("menu_log.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Log(_L)"), *image));
    Gtk::ImageMenuItem* itemLog = (Gtk::ImageMenuItem*)&this->items().back();

    /*菜单-屏蔽*/
    this->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(_("block(_B)"),sigc::mem_fun(wnd,&MainWindow::on_block_friend)));

    /*分隔符*/
    this->items().push_back(Gtk::Menu_Helpers::SeparatorElem());

    /*菜单-类型*/
    image = getImage("groupchat.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Type"),*image));
    Gtk::ImageMenuItem* itemType = (Gtk::ImageMenuItem*)&this->items().back();
    /*菜单-别名*/
    image = getImage("edit.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Alias"),*image));
    Gtk::ImageMenuItem* itemNickNameEdit = (Gtk::ImageMenuItem*)&this->items().back();
    /*菜单-删除好友*/
    this->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-delete")));
    Gtk::ImageMenuItem* itemRemove = (Gtk::ImageMenuItem*)&this->items().back();

    this->show_all();

    itemChat->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_buddyChat_activate));
    itemNickNameEdit->signal_activate().connect(
	sigc::mem_fun(wnd,&MainWindow::on_buddyNameEdit_activate));
    itemLog->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_buddyLog_activate));
    itemInfo->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_buddyInfo_activate));
    itemRemove->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_buddyRemove_activate));
    itemType->signal_activate().connect(
		    sigc::mem_fun(wnd, &MainWindow::on_buddyType_activate));

}

RoomMenu::RoomMenu(MainWindow& wnd)
{
	/* 菜单-加入*/
    Gtk::Image* image = getImage("menu_chat.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_(_("join in(_C)")), *image));
    Gtk::ImageMenuItem* itemJoin = (Gtk::ImageMenuItem *)&this->items().back();
    /*菜单-屏蔽*/
    this->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(_("block(_B)"),sigc::mem_fun(wnd,&MainWindow::on_block_room)));
    /*菜单-日志*/
    image = getImage("menu_log.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Log(_L)"), *image));
    Gtk::ImageMenuItem* itemLog = (Gtk::ImageMenuItem*)&this->items().back();
    /*菜单-删除好友*/
    this->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-delete"),sigc::mem_fun(wnd, &MainWindow::on_delRoom_activate)));
    //Gtk::ImageMenuItem* itemRemove = (Gtk::ImageMenuItem*)&this->items().back();
}


SystemMenu::SystemMenu(MainWindow& wnd)
{
	/*菜单-关于*/
    this->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-about")));
    Gtk::ImageMenuItem* itemAbout = (Gtk::ImageMenuItem *)&this->items().back();


    /*菜单-添加好友*/
    Gtk::Image* image = getImage("menu_add.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Add Friend(_A)"),*image));
    Gtk::ImageMenuItem* itemAdd = (Gtk::ImageMenuItem *)&this->items().back();

    /*菜单-添加聊天室*/
    image = getImage("menu_add.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Add Room(_H)"),*image));
    Gtk::ImageMenuItem* itemRoomAdd = (Gtk::ImageMenuItem *)&this->items().back();


    /*菜单-查找好友*/
    this->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-find")));
    Gtk::ImageMenuItem* itemFind = (Gtk::ImageMenuItem *)&this->items().back();

    /*菜单-查找服务*/
    image=getImage("disco.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Server Disco"),*image));
    Gtk::ImageMenuItem* itemDisco= (Gtk::ImageMenuItem *)&this->items().back();

    /*菜单-刷新好友列表*/
    image=getImage("disco.png");
    this->items().push_back(Gtk::Menu_Helpers::ImageMenuElem(_("Refresh List"),*image));
    Gtk::ImageMenuItem* itemFreshList= (Gtk::ImageMenuItem *)&this->items().back();
    /*菜单-显示离线好友*/
    this->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(_("Show offline"),sigc::mem_fun(wnd,&MainWindow::on_show_all_friends)));
    /*静音*/
    this->items().push_back(Gtk::Menu_Helpers::CheckMenuElem(_("Mutt"),sigc::mem_fun(wnd,&MainWindow::on_sound_activate)));
    //Gtk::CheckMenuItem* itemsound= (Gtk::CheckMenuItem*)&this->items().back();
    //if(sounds::mute(3))
//	    itemsound->set_active();
    /*菜单-首选项*/
    this->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-preferences")));
    Gtk::ImageMenuItem* itemPrefer = (Gtk::ImageMenuItem *)&this->items().back();
    
    /*分隔符*/
    this->items().push_back(Gtk::Menu_Helpers::SeparatorElem());
    this->items().push_back(Gtk::Menu_Helpers::StockMenuElem(Gtk::StockID("gtk-quit")));
    Gtk::ImageMenuItem* itemQuit = (Gtk::ImageMenuItem *)&this->items().back();
    this->show_all();


    itemAdd->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_addBuddy_activate));
    itemRoomAdd->signal_activate().connect(
		    sigc::mem_fun(wnd,&MainWindow::on_addRoom_activate));
    itemFind->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_findBuddy_activate));
    itemDisco->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_serverDisco_activate));
    itemFreshList->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_freshLIst_activate));
    itemAbout->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_about_activate));
    itemPrefer->signal_activate().connect(
	    sigc::mem_fun(wnd, &MainWindow::on_setPrefer_activate));
    itemQuit->signal_activate().connect(
		    sigc::mem_fun(wnd, &MainWindow::on_quit));
}
