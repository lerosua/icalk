/*
* =====================================================================================
*
*       Filename:  Bodies.cpp
*
*    Description:  主类,负责创建其它各个功能类.

*
*        Version:  1.0
*        Created:  2007年06月22日 21时08分56秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  wind (xihe), xihels@gmail.com
*        Company:
*
* =====================================================================================
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <libintl.h>
#include <exception>
#include <gtkmm.h>
#include <libglademm/xml.h>
#include <libglademm/variablesmap.h>
#include <gloox/disco.h>
#include <gloox/stanzaextension.h>
#include <gloox/delayeddelivery.h>
#include <gloox/xhtmlim.h> 
//#include "gtalkcaps.h"
#include "MainWindow.h" 
//#include "TalkFT.h"
//#include "TalkCard.h"
#include "Bodies.h"
#include "Unit.h"
#include "sounds.h"

using namespace std;

Bodies& Bodies::Get_Bodies()
{
        static Bodies bodies;
        return bodies;
}

Bodies::Bodies():
                m_talkFT(NULL)
                , m_cardManage(NULL)
                , m_vcard(NULL)
                , accountTag(NULL)
{
        main_window = new MainWindow(*this);
        msg_window = new MsgWindow();
        statusIcon = new TrayIcon(main_window);

        // 绑定登录回调函数
        main_window->signal_on_login(this, &Bodies::on_login);
        m_talkConnect.signal_has_login(this, &Bodies::has_login);
        m_talkConnect.signal_relogin(this, &Bodies::relogin);
}

Bodies::~Bodies()
{
        disconnect();

        if (main_window) {
                delete main_window;
                main_window = NULL;
        }

        if (msg_window) {
                delete msg_window;
                msg_window = NULL;
        }

        if (statusIcon) {
                delete statusIcon;
                statusIcon = NULL;
        }


        if (m_vcard) {
                delete m_vcard;
                m_vcard = NULL;
        }

        //logout();
}

const std::string Bodies::getAccountTag(const std::string& name)
{
        Tag* tmpTag = accountTag->findChild(name);

        if (tmpTag == NULL) {
                return std::string();
        }

        return tmpTag->cdata();
}

void Bodies::setAccountTag(const std::string& name, const std::string& value)
{
        Tag* tmpTag = accountTag->findChild(name);

        if (tmpTag == NULL) {
                tmpTag = new Tag(name, value);
                accountTag->addChild(tmpTag);
        } else {
                tmpTag->setCData(value);
        }

        saveAccountTag();
}

void Bodies::fetch_self_vcard()
{
        m_cardManage->fetch_vcard(m_jid);
}

void Bodies::set_status(Presence::PresenceType f_status, Glib::ustring f_msg)
{
        m_client->setPresence(f_status, 1, f_msg);
        statusIcon->on_status_change(f_status, m_jid.username(), f_msg);
        //setAccountTag("status",f_status);
        setAccountTag("message", f_msg);
}

void Bodies::promptMsg(const Glib::ustring& f_jid, bool f_new)
{
        if (f_new) {
                buddy_list.addNewMsgBuddy(f_jid);
                msg_count++;
                statusIcon->setBlinking(f_new);
                DLOG("have new message form %s\n", f_jid.c_str());
        } else {
                buddy_list.delNewMsgBuddy(f_jid);
                msg_count--;

                if (msg_count < 1)
                        statusIcon->setBlinking(f_new);
        }
}

USERLIST& Bodies::getUserList()
{
        char buf[512];
        char* homedir = getenv("HOME");
        char homepath[512];
        snprintf(homepath, 512, "%s/.icalk/", homedir);
        mkdir(homepath, S_IRUSR | S_IWUSR | S_IXUSR);
        snprintf(buf, 512, "%s/userlist.xml", homepath);
        std::ifstream infile(buf);

        if (!infile) {
                std::cout << "erro in load userlist\n";
                std::ofstream outfile(buf);
                outfile.close();
                return userlist;
        }

        std::string line;

        if (infile) {
                while (getline(infile, line)) {
                        userlist.push_back(line);
                }
        }

        infile.close();

        return userlist;
}

void Bodies::saveUserList(const std::string& jid)
{
        //userlist.push_back(jid);
        char buf[512];

        char* homedir = getenv("HOME");
        char homepath[512];
        snprintf(homepath, 512, "%s/.icalk/", homedir);
        mkdir(homepath, S_IRUSR | S_IWUSR | S_IXUSR);
        snprintf(buf, 512, "%s/userlist.xml", homepath);
        std::ofstream outfile(buf, std::ios::app);
        /*
        USERLIST::iterator iter=userlist.begin();
        for(; iter!=userlist.end();iter++)
        {
         DLOG("userlist %s\n", (*iter).c_str());
        }
        */
        outfile << jid << std::endl;
        outfile.close();
}

void Bodies::saveAccountTag()
{
        char buf[512];
        snprintf(buf, 512, "%s/account.xml", GUnit::getUserPath());

        try {
                ConfXml accoutxml;
                accoutxml.setTagXml(accountTag);
                accoutxml.xml_saveto_file(buf);
        } catch (exception& e) {
                fprintf(stderr, "%s\n", e.what());
        }
}

void Bodies::loadAccountTag()
{
        char buf[512];
        snprintf(buf, 512, "%s/account.xml", GUnit::getUserPath());

        try {
                ConfXml accoutxml;
                accountTag = accoutxml.xml_from_file(buf);

                if (!accountTag) {
                        // TODO
                        // 1. remove try..catch
                        DLOG("accountTag == NULL\n");
                        throw;
                }
        } catch (exception& e) {
                DLOG("%s\n", e.what());
        }
}

bool Bodies::callback(Glib::IOCondition condition)
{
        ConnectionError ce = ConnNoError;

        if ( ce == ConnNoError) {
                ce = m_client->recv(1000); // microseconds, not milliseconds
		if(ce != ConnNoError)
			DLOG("recv error %d\n",ce);
        }

        //DLOG("recv return %d\n", ce);

        return true;
}

void Bodies::disconnect()
{
        connectIO.disconnect();

        if (m_talkFT) {
                delete m_talkFT;
                m_talkFT = NULL;
        }

        if (m_cardManage) {
                delete m_cardManage;
                m_cardManage = NULL;
        }
}

int Bodies::connect(const string& name, const string& passwd, const string& server, const int port = 5222)
{
        DLOG("%s is connecting %s:%d\n", name.c_str(), server.c_str(), port);
        m_jid.setJID(name);
        m_client.reset(new Client(m_jid, passwd)); // auto_ptr

        m_client->disco()->setVersion("iCalk", ICALK_VERSION, OS);
        m_client->setResource("iCalk");
        m_client->setPresence(Presence::Available, -1);
        m_client->disco()->setIdentity("Client", "iCalk");
        m_client->disco()->registerDiscoHandler(&m_discoHandler);
        m_client->registerMessageHandler(&m_talkMsg);
        m_client->rosterManager()->registerRosterListener(&buddy_list);
        m_client->registerMessageSessionHandler(&m_talkMsg, 0);
        m_client->registerConnectionListener(&m_talkConnect);
        m_client->registerStanzaExtension(new DelayedDelivery(0));
        m_client->registerStanzaExtension(new XHtmlIM(0));
        m_client->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, &m_talkConnect);
        m_client->registerMUCInvitationHandler(&m_roominvitation);
        m_client->setServer(server);
        m_client->setPort(port);

        // connect(true)  -- block until connection has been established.
        // connect(false) -- unblock, return immediately.

        if (m_client->connect(false)) {
                return dynamic_cast<ConnectionTCPClient*>(m_client->connectionImpl())->socket();
        }

        DLOG("connect: unknown error\n");

        return -1;
}

bool Bodies::on_login(string f_name, string f_passwd, string f_server, int f_port)
{
        if (f_name.empty() || f_passwd.empty() || (f_port >= 65535) || (f_port <= 1024)) {
                return false;
        }

        GUnit::init(f_name.c_str());

        loadAccountTag();

        if (main_window->KeepUser() || main_window->KeepPassword()) {
                USERLIST::iterator iter =
                        find(userlist.begin(), userlist.end(), f_name);

                if (iter == userlist.end())
                        saveUserList(f_name);

                if (!f_server.empty())
                        setAccountTag("server", f_server);

                { // port
                        char p[6] = "";
                        sprintf(p, "%d", f_port);
                        setAccountTag("port", p);
                }

                if (main_window->KeepPassword()) {
                        setAccountTag("keeppasswd", "true");
                        setAccountTag("passwd", f_passwd);
                } else {
                        setAccountTag("keeppasswd", "false");
                        setAccountTag("passwd", "xxxxxxx");
                }
        }

        setAccountTag("name", f_name);

        int mysock = connect(f_name, f_passwd, f_server, f_port);

        if (mysock < 0) {
                return false;
        }

        connectIO = Glib::signal_io().connect(

                            sigc::mem_fun(*this, &Bodies::callback),
                            mysock,
                            Glib::IO_IN | Glib::IO_HUP);

        return true;
}

void Bodies::has_login()
{
        string label = m_jid.bare();
        string msg = getAccountTag("message");

        main_window->on_login_finial();
        main_window->on_combo_change();
        main_window->initRoom();
        main_window->set_label(label, msg);
        main_window->set_logo(getAccountTag("icon"));

        /** 初始化VCard管理类*/
        m_cardManage = new TalkCard(m_client.get());

        /** 初始化文件传输接收类*/
        m_talkFT = new TalkFT(m_client.get());
        m_talkFT->initFT();

}

void Bodies::set_vcard(const VCard* f_vcard)
{
        if (NULL != m_vcard)
                delete m_vcard;

        m_vcard = new VCard(*f_vcard);
}

void Bodies::logout()
{
        m_client->disconnect();
	disconnect();
}

void Bodies::relogin()
{
        main_window->on_relogin();
}

void Bodies::reconnect()
{
        connectIO.disconnect();
}

void Bodies::disco_node(const std::string& node)
{
        m_client->disco()->getDiscoInfo( JID(node), "", &m_discoHandler, 0, "");
        m_client->disco()->getDiscoItems( JID(node), "", &m_discoHandler, 0, "");
}

int main(int argc, char *argv[])
{

        bindtextdomain (GETTEXT_PACKAGE, ITALK_LOCALEDIR);
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
        textdomain (GETTEXT_PACKAGE);

        struct sigaction act, oact;
        act.sa_handler = sounds::on_play_exit;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        if (sigaction(SIGCHLD, &act, &oact) < 0) {
                perror("Sigaction failed");
                exit(1);
        }

        Gtk::Main kit(argc, argv);

        Bodies::Get_Bodies();
        kit.run();

        return 0;
}

