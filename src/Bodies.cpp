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

Bodies::Bodies()
{
	main_window = new MainWindow(*this);
	msg_window = new MsgWindow();
	statusIcon = new TrayIcon(main_window);
}

Bodies::~Bodies()
{
	delete statusIcon;
	delete talkFT;
	main_window=NULL;
	statusIcon=NULL;
	//logout();
}

USERLIST& Bodies::getUserList()
{
	char buf[512];
	char* homedir = getenv("HOME");
	char homepath[512];
	snprintf(homepath,512,"%s/.icalk/",homedir);
	mkdir(homepath, S_IRUSR | S_IWUSR | S_IXUSR);
	snprintf(buf,512,"%s/userlist.xml",homepath);
	std::ifstream infile(buf);
	if(!infile){
		std::cout<<"erro in load userlist\n";
		std::ofstream outfile(buf);
		outfile.close();
		return userlist;
	}
	std::string line;
	if(infile)
	{
		while(getline(infile,line))
		{
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
	snprintf(homepath,512,"%s/.icalk/",homedir);
	mkdir(homepath, S_IRUSR | S_IWUSR | S_IXUSR);
	snprintf(buf,512,"%s/userlist.xml",homepath);
	std::ofstream outfile(buf,std::ios::app);
	/*
	USERLIST::iterator iter=userlist.begin();
	for(; iter!=userlist.end();iter++)
	{
		PBUG("userlist %s\n", (*iter).c_str());
	}
	*/
	outfile<<jid<<std::endl;
	outfile.close();
}

void Bodies::saveAccountTag()
{
	char buf[512];
	snprintf(buf,512,"%s/account.xml",GUnit::getUserPath());

	try{
		ConfXml accoutxml;
		accoutxml.setTagXml(accountTag);
		accoutxml.xml_saveto_file(buf);
	}
	catch (exception& e){
		fprintf(stderr, "%s\n",e.what());
	}
}
void Bodies::loadAccountTag()
{
	char buf[512];
	snprintf(buf,512,"%s/account.xml",GUnit::getUserPath());

	try{
		ConfXml accoutxml;
		accountTag = accoutxml.xml_from_file(buf);
	}
	catch (exception& e)
	{
		fprintf(stderr, "%s\n",e.what());
	}
}
bool Bodies::callback(Glib::IOCondition condition)
{
	ConnectionError ce=ConnNoError;
	//std::cout<<"talk connecting...: "<<ce<<std::endl;
	if ( ce == ConnNoError)
	{
		ce=jclient->recv();
		//talkFT->loopRecv();
		//IBBSHandler.sendIBBData("lerosua icalk testing");
		
	}
	return true;
}

int Bodies::connect(const char *name, const char* passwd)
{
	//GUnit::init(name);
	/*读取AccountTag类*/
	//loadAccountTag();
	/*设置AccountTag类的name*/
	std::string name_value(name);
	setAccountTag("name",name_value);

	std::string server= getAccountTag("server");
	std::string portstring = getAccountTag("port");

	/*设置client，进行登录*/
	//assert(NULL == jclient.get());
	if(NULL != jclient.get())
	{
		//std::cout<<"重新连接中"<<std::endl;
		connectIO.disconnect();
		delete talkFT;
	}
	jid=new JID(name);
	jclient.reset(new Client(*jid,passwd));
	jclient->disco()->setVersion("iCalk",ICALK_VERSION,OS);
	jclient->setResource("iCalk");
	jclient->setPresence(Presence::Available,-1);
	jclient->disco()->setIdentity("Client","iCalk");
	jclient->registerMessageHandler(&talkmsg);
	jclient->rosterManager()->registerRosterListener(&buddy_list);
	jclient->registerMessageSessionHandler(&talkmsg,0);
	jclient->registerConnectionListener(&talkconnect);
	jclient->registerStanzaExtension(new DelayedDelivery(0));
	jclient->registerStanzaExtension(new XHtmlIM(0));
	jclient->logInstance().registerLogHandler(LogLevelDebug,LogAreaAll,&talkconnect);

	PBUG("login\n");
	if(!server.empty())
		jclient->setServer(server);
	
	if(!portstring.empty())
	{
		int port = atoi(portstring.c_str());
		jclient->setPort(port);
	}
	/*
	Tag* t=new Tag("c");
	t->addAttribute("xmlns",XMLNS_C_CAPS);
	FCAPS* se=new FCAPS(t) ;
	jclient->addPresenceExtension(se);
	*/
	

	//jclient->disco()->registerDiscoHandler(&discohandler);
	//bookMark= new TalkBookMark(jclient.get());

	/** 初始化VCard管理类*/
	cardManage.set_manage(jclient.get());
	/** 初始化文件传输接收类*/
	talkFT=new TalkFT();
	talkFT->initFT(jclient.get());
	/** 初始化带内数据传输处理类*/
	//IBBSHandler.init(jclient.get());
	if(jclient->connect(false))
	{
		PBUG("connect call success\n");
		return dynamic_cast<ConnectionTCPClient*>(jclient->connectionImpl())->socket();
	}
	else
		PBUG("connect error\n");
	return -1;
}
	
bool Bodies::login(const std::string name,const std::string passwd)
{
	int mysock = -1;
	if ((mysock = connect(name.c_str(), passwd.c_str())) != -1)
	{
	connectIO = Glib::signal_io().connect(
			sigc::mem_fun(*this, &Bodies::callback),
			mysock,
			Glib::IO_IN | Glib::IO_HUP);
	}

	main_window->on_initalize();
	main_window->on_logining();
	main_window->set_label();
	const std::string iconpath = getAccountTag("icon");
	main_window->set_logo(iconpath);
	//fetch_self_vcard();

	return true;
}
void Bodies::set_vcard(VCard* vcard_)
{
	if (vcard != vcard_) {
		delete vcard;
		vcard = vcard_;
		//vcard = VCard(vcard_->tag());
	}
}
void Bodies::logout()
{
	jclient->disconnect();
}

void Bodies::disco_test()
{
	//jclient->disco()->getDiscoInfo( JID("jabber.org"),"",&discohandler,0,"");
}
int main(int argc,char *argv[])
{

        bindtextdomain (GETTEXT_PACKAGE, ITALK_LOCALEDIR);
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
        textdomain (GETTEXT_PACKAGE);

	struct sigaction act, oact;
	act.sa_handler = sounds::on_play_exit;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	if(sigaction(SIGCHLD, &act, &oact)<0){
			perror("Sigaction failed");
			exit(1);
			}

	Gtk::Main kit(argc,argv);

	Bodies::Get_Bodies();
	//if (Bodies::Get_Bodies().login())
		kit.run();

	return 0;
}

