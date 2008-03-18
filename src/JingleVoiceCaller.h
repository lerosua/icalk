/*
* =====================================================================================
*
*       Filename:  JingleVoiceCaller.h
*
*    Description:
*
*        Version:  1.0
*        Created:  2008年01月23日 12时40分30秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef JINGLEVOICECALLER_H_
#define JINGLEVOICECALLER_H_


using namespace XMPP;

namespace cricket
{

class SocketServer;

class Thread;

class NetworkManager;

class BasicPortAllocator;

class SessionManager;

class PhoneSessionClient;

class Call;

class SocketAddress;
}

namespace jingle
{

class JingleVoiceCaller
{

public:
        JingleVoiceCaller();
        ~JingleVoiceCaller();

        virtual bool calling(const Jid&);
        virtual void initialize();
        virtual void deinitialize();
        virtual void call(const Jid&);
        virtual void accept(const Jid&);
        virtual void reject(const Jid&);
        virtual void terminate(const Jid&);

protected:
        void registerCall(const Jid&, cricket::Call*);
        void removeCall(const Jid&);

private:
        bool initialized_;
        static cricket::SocketServer* socket_server_ ;
        static cricket::Thread* thread_ ;
        static cricket::NetworkManager* network_manager_;
        static cricket::BasicPortAllocator* port_allocator_;
        static cricket::SocketAddress* stun_addr_;
        cricket::SessionManager* session_manager_;
        cricket::PhoneSessionClient* phone_client_;


};

}

#endif
