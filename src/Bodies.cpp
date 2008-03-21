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
                , jid(NULL)
                , m_cardManage(NULL)
                , m_vcard(NULL)
                , accountTag(NULL)
{
        main_window = new MainWindow(*this);
        msg_window = new MsgWindow();
        statusIcon = new TrayIcon(main_window);
}

Bodies::~Bodies()
{
        disconnect();
        // has been done in disconnect()
        // if (m_talkFT) {
        //     delete m_talkFT;
        //     m_talkFT = NULL;
        // }
        // if (m_cardManage) {
        //     delete m_cardManage;
        //     m_cardManage = NULL;
        // }

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

        if (jid) {
                delete jid;
                jid = NULL;
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
        m_cardManage->fetch_vcard(*jid);
}

void Bodies::set_status(Presence::PresenceType f_status, Glib::ustring f_msg)
{
        m_client->setPresence(f_status, 1, f_msg);
        statusIcon->on_status_change(f_status, jid->username(), f_msg);
        //setAccountTag("status",f_status);
        setAccountTag("message", f_msg);
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
                        // 2. use macro define fpr..(err.., .., __FILE__, __LINE__)
                        fprintf(stderr, "!!! %s:%d\n", __FILE__, __LINE__);
                        throw;
                }
        } catch (exception& e) {
                fprintf(stderr, "%s\n", e.what());
        }
}

bool Bodies::callback(Glib::IOCondition condition)
{
        ConnectionError ce = ConnNoError;
        //std::cout<<"talk connecting...: "<<ce<<std::endl;

        if ( ce == ConnNoError) {
                ce = m_client->recv(1000); // microseconds, not milliseconds
        }

        return true;
}

// XXX
// 应该把 m_talkFT 和 m_cardManage 的处理放到
//   - 连接建立成功
//   - 登录成功
//   - 好友列表初始化成功
// 之后
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
        jid = new JID(name);
        m_client.reset(new Client(*jid, passwd)); // auto_ptr

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
        m_client->setServer(server);
        m_client->setPort(port);

        /*
        Tag* t=new Tag("c");
        t->addAttribute("xmlns",XMLNS_C_CAPS);
        FCAPS* se=new FCAPS(t) ;
        m_client->addPresenceExtension(se);
        */

        //bookMark= new TalkBookMark(m_client.get());

        /** 初始化VCard管理类*/
        m_cardManage = new TalkCard(m_client.get());

        /** 初始化文件传输接收类*/
        m_talkFT = new TalkFT(m_client.get());
        m_talkFT->initFT();

        if (m_client->connect(false)) {
                return dynamic_cast<ConnectionTCPClient*>(m_client->connectionImpl())->socket();
        }

        DLOG("connect error\n");

        return -1;
}

bool Bodies::login(const std::string& name, const std::string& passwd)
{
        int mysock = -1;
        int port = atoi(getAccountTag("port").c_str());
        string server = getAccountTag("server");

        mysock = connect(name, passwd, server, port);

        if (mysock < 0) {
                DLOG("!!!connect failed\n");
                return false;
        }

        DLOG("connected successfully\n");

        connectIO = Glib::signal_io().connect(
                            sigc::mem_fun(*this, &Bodies::callback),
                            mysock,
                            Glib::IO_IN | Glib::IO_HUP);

        main_window->on_initialize();
        main_window->on_logining();
        main_window->set_label();
        const std::string iconpath = getAccountTag("icon");
        main_window->set_logo(iconpath);
        //fetch_self_vcard();

        return true;
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

