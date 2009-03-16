/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  BotCalk interface
 *
 *        Version:  1.0
 *        Created:  2009年03月13日 19时51分09秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "BotCalk.h"

bool get_param(const std::string& line,const std::string& name, std::string& dst_str)
{
	size_t pos = line.find(name);
	if(pos == std::string::npos)
		return false;
	pos = line.find("=");
	if(pos == std::string::npos)
		return false;
	dst_str = line.substr(pos+1,std::string::npos);
	return true;

}

int main(int argc,char *argv[])
{
 
	if(argc<3){
		printf("BotCalk usage: botcalk -f botcalk.conf\n");
		return -1;
	}
	if(strcmp(argv[1],"-f")){
		printf("BotCalk usage: botcalk -f botcalk.conf\n");
		printf("error 2\n");
		return -1;
	}

	std::ifstream conf_file(argv[2]);
	if(!conf_file){
		printf("open file error %s\n",argv[2]);
		return -1;
	}

	std::string jid;
	std::string pwd;
	std::string server;
	std::string port;
	std::string mucroom;
	std::string line;

	while(std::getline(conf_file,line)){
		get_param(line,"jid",jid);
		get_param(line,"passwd",pwd);
		get_param(line,"server",server);
		get_param(line,"port",port);
		get_param(line,"room",mucroom);
		
	}
	conf_file.close();

	if((mkfifo(FIFO_MSG,PERMS))<0){
		perror("mkfifo");
	}
	if((mkfifo(FIFO_SEND,PERMS))<0){
		perror("mkfifo");
	}

	int num=atoi(port.c_str());
	  BotCalk *bot = new BotCalk();
	  bot->start(jid,pwd,server,num);

	  delete( bot );
	    return 0;
}

