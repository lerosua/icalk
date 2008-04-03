#include <iostream>
#include "stun.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc,char **argv)
{
	int fd;
	StunAddress4 mappedAddr;
	StunAddress4 stunServerAddr;
	stunServerAddr.addr=0;
	StunAddress4 sAddr;
	int srcPort=0;
//	char *servername="larry.gloo.net";
	//char *servername="stun.fwdnet.net";
	char *servername="stunserver.org";
	bool verbose=false;

	sAddr.addr=0;
	sAddr.port=0;

	bool ret=stunParseServerName(servername,stunServerAddr);
	if(ret!=true)
	{
		cout<<servername<<" is not a valid host name "<<endl;
		exit(-1);
	}

	fd=stunOpenSocket(stunServerAddr,&mappedAddr,srcPort,&sAddr,verbose);
	cout<<"Got ip "<<mappedAddr<<endl;
	struct in_addr ip;
	ip.s_addr=ntohl(mappedAddr.addr);	
	cout<<"ip is "<<inet_ntoa(ip)<<endl;
	//close(fd);	
	return 0;
}

