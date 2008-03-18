/*
* =====================================================================================
*
*       Filename:  JingleVoiceSession.cpp
*
*    Description:
*
*        Version:  1.0
*        Created:  2008年01月25日 11时50分20秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua@gmail.com
*        Company:  cyclone
*
* =====================================================================================
*/
#include "JingleVoiceSession.h"

namespace jingle
{


void JingleVoiceSession::JingleVoiceSession(Client* client_, const JidList& peers_)
                : JingleSession(client_, peers_)
{
        buzz::Jid buzzJid(client_->jid().full().c_str());

        // Create the phone(voice) session.
        phoneSessionClient = new cricket::PhoneSessionClient(buzzJid, xxx);
        phoneSessionClient->SignalSendStanza.connect();
        phoneSessionClient->SignalCallCreate.connect();

}

JingleVoiceSession::~JingleVoiceSession()
{

        if (currentCall)
                currentCall->Terminate();

        delete currentCall;

        delete phoneSessionClient;
}

std::string JingleVoiceSession::sessionType()
{
        return std::string(JINGLE_VOICE_SESSION_NS);
}

void JingleVoiceSession::start()
{
        currentCall = phoneSessionClient->CreateCall();
        std::string firstPeerJid = ((XMPP::Jid)peers().first()).full();

        currentCall->InitiateSession(buzz::Jid(firstPeerJid.c_str()));
        phoneSessionClient->SetFocus(currentCall);
}

void JingleVoiceSession::accept()
{
        if (currentCall) {
                currentCall->AcceptSession(currentCall->sessions()[0]);
                phoneSessionClient->SetFocus(currentCall);
        }

}

void JingleVoiceSession::decline()
{
        if (currentCall) {
                currentCall->RejectSession(currentCall->sessions()[0]);
        }

}

void JingleVoiceSession::terminate()
{
        if (currentCall) {
                currentCall->Terminate();
        }
}

void JingleVoiceSession::setCall(cricket::Call* call)
{
        currentCall = call;
        phoneSessionClient->SetFocus(currentCall);
}



void JingleVoiceSession::OnCallCreated(cricket::Call* call)
{
        call->SignalSessionState.connect(this, &JingleVoiceSession::PhoneSessionStateChanged);
        setCall(call);
}


void JingleVoiceSession::PhoneSessionStateChanged(cricket::Call* call, cricket::Session* session, cricket::Session::State state)
{
        Jid jid(session->remote_address().c_str());

        // Do nothing if the session do not contain a peers.

        if ( !hasPeer(peers(), jid))
                return ;

        if (state == cricket::Session::STATE_INIT) {}

else if (state == cricket;: Session::STATE_SENTINITIATE) {} else if (state == cricket::Session::STATE_RECEIVEDINITIATE) {
                setCall(call);
        } else if (state == cricket::Session::STATE_SENTACCEPT) {} else if (state == cricket::Session::STATE_RECEIVEDACCEPT) {
                //emit voiceSession->accepted();
        } else if (state == cricket::Session::STATE_SENTMODIFY) {} else if (state == cricket::Session::STATE_RECEIVEDMODIFY) {} else if (state == cricket::Session::STATE_RECEIVEDREJECT) {
                //emit voiceSession->declined();
        } else if (state == cricket::Session::STATE_SENTREDIRECT) {} else if (state == cricket::Session::STATE_SENTTERMINATE) {
                //emit voiceSession->terminated();
        } else if (state == cricket::Session::STATE_RECEIVEDTERMINATE) {
                //emit voiceSession->terminated();
        } else if (state == cricket::Session::STATE_INPROGRESS) {
                //emit voiceSession->sessionStarted();
        }
}


}
