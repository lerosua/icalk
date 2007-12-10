/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "talk/base/logging.h"
#include "talk/session/phone/phonesessionclient.h"
#include "talk/xmpp/constants.h"
#include "talk/xmllite/qname.h"
namespace {

const std::string NS_PHONE("http://www.google.com/session/phone");
const std::string NS_EMPTY("");

const buzz::QName QN_PHONE_DESCRIPTION(true, NS_PHONE, "description");
const buzz::QName QN_PHONE_PAYLOADTYPE(true, NS_PHONE, "payload-type");
const buzz::QName QN_PHONE_PAYLOADTYPE_ID(true, NS_EMPTY, "id");
const buzz::QName QN_PHONE_PAYLOADTYPE_NAME(true, NS_EMPTY, "name");
const buzz::QName QN_PHONE_PAYLOADTYPE_RATE(true, NS_EMPTY, "clockrate");
const buzz::QName QN_PHONE_PAYLOADTYPE_BITRATE(true, NS_EMPTY, "bitrate");
const buzz::QName QN_PHONE_PAYLOADTYPE_CHANNELS(true, NS_EMPTY, "channels");
}

namespace cricket {

PhoneSessionClient::PhoneSessionClient(
    const buzz::Jid& jid, SessionManager *manager)
  : jid_(jid), session_manager_(manager) {
  // No call to start, and certainly no call with focus
  focus_call_ = NULL;

  // Start up the channel manager on a worker thread
  channel_manager_ = new ChannelManager(session_manager_->worker_thread());

  // Register ourselves as the handler of phone sessions.
  session_manager_->AddClient(NS_PHONE, this);
}


PhoneSessionClient::~PhoneSessionClient() {
  // Destroy all calls
  std::map<uint32, Call *>::iterator it;
  while (calls_.begin() != calls_.end()) {
    std::map<uint32, Call *>::iterator it = calls_.begin();
    DestroyCall((*it).second);
  }

  // Delete channel manager. This will wait for the channels to exit
  delete channel_manager_;

  // Remove ourselves from the client map.
  session_manager_->RemoveClient(NS_PHONE);
}

PhoneSessionDescription* PhoneSessionClient::CreateOfferSessionDescription() {
  PhoneSessionDescription* session_desc = new PhoneSessionDescription();


  MediaEngine *me = channel_manager_->media_engine();
  std::vector<Codec> codecs = me->codecs();
  std::vector<Codec>::iterator i;
  for (i = codecs.begin(); i < codecs.end(); i++) 
    session_desc->AddCodec(*i);
  
  session_desc->Sort();
  return session_desc;
}

PhoneSessionDescription* PhoneSessionClient::CreateAcceptSessionDescription(const SessionDescription* offer) {
  const PhoneSessionDescription* offer_desc =
      static_cast<const PhoneSessionDescription*>(offer);
  PhoneSessionDescription* accept_desc = new PhoneSessionDescription();
  std::vector<Codec> codecs = channel_manager_->media_engine()->codecs();
  std::vector<Codec>::iterator iter;
  for (unsigned int i = 0; i < offer_desc->codecs().size(); ++i) {
    if (channel_manager_->media_engine()->FindCodec(offer_desc->codecs()[i]))
        accept_desc->AddCodec(offer_desc->codecs()[i]); 
  }

  accept_desc->Sort();
  return accept_desc;
}

const SessionDescription *PhoneSessionClient::CreateSessionDescription(const buzz::XmlElement *element) {
  PhoneSessionDescription* desc = new PhoneSessionDescription();

  const buzz::XmlElement* payload_type = element->FirstNamed(QN_PHONE_PAYLOADTYPE);
  int num_payload_types = 0;

  while (payload_type) {
    if (payload_type->HasAttr(QN_PHONE_PAYLOADTYPE_ID)) {
      int id = atoi(payload_type->Attr(QN_PHONE_PAYLOADTYPE_ID).c_str());
      
      std::string name = "";
      if (payload_type->HasAttr(QN_PHONE_PAYLOADTYPE_NAME))
        name = payload_type->Attr(QN_PHONE_PAYLOADTYPE_NAME);
     
      int clockrate = 0;
      if (payload_type->HasAttr(QN_PHONE_PAYLOADTYPE_RATE))
	      clockrate = atoi(payload_type->Attr(QN_PHONE_PAYLOADTYPE_RATE).c_str());

      int bitrate = 0;
      if (payload_type->HasAttr(QN_PHONE_PAYLOADTYPE_BITRATE))
        bitrate = atoi(payload_type->Attr(QN_PHONE_PAYLOADTYPE_BITRATE).c_str());

      int channels = 1;
      if (payload_type->HasAttr(QN_PHONE_PAYLOADTYPE_CHANNELS))
	      channels = atoi(payload_type->Attr(QN_PHONE_PAYLOADTYPE_CHANNELS).c_str());

      desc->AddCodec(Codec(id, name, clockrate, bitrate, channels, 0));
    }

    payload_type = payload_type->NextNamed(QN_PHONE_PAYLOADTYPE);
    num_payload_types += 1;
  }

  // For backward compatability, we can assume the other client is (an old
  // version of Talk) if it has no payload types at all.
  if (num_payload_types == 0) {
    desc->AddCodec(Codec(103, "ISAC", 16000, -1, 1, 1));
    desc->AddCodec(Codec(0, "PCMU", 8000, 64000, 1, 0));
  }

  return desc;
}

buzz::XmlElement *PhoneSessionClient::TranslateSessionDescription(const SessionDescription *_session_desc) {
  const PhoneSessionDescription* session_desc =
      static_cast<const PhoneSessionDescription*>(_session_desc);
  buzz::XmlElement* description = new buzz::XmlElement(QN_PHONE_DESCRIPTION, true);


  for (size_t i = 0; i < session_desc->codecs().size(); ++i) {
    buzz::XmlElement* payload_type = new buzz::XmlElement(QN_PHONE_PAYLOADTYPE, true);

    char buf[32];
    sprintf(buf, "%d", session_desc->codecs()[i].id);
    payload_type->AddAttr(QN_PHONE_PAYLOADTYPE_ID, buf);

    payload_type->AddAttr(QN_PHONE_PAYLOADTYPE_NAME,
                          session_desc->codecs()[i].name.c_str());

    if (session_desc->codecs()[i].clockrate > 0) {
      sprintf(buf, "%d", session_desc->codecs()[i].clockrate);
      payload_type->AddAttr(QN_PHONE_PAYLOADTYPE_RATE, buf);
    }
    
    if (session_desc->codecs()[i].channels > 1) {
      sprintf(buf, "%d", session_desc->codecs()[i].channels);
      payload_type->AddAttr(QN_PHONE_PAYLOADTYPE_CHANNELS, buf);
    }

    if (session_desc->codecs()[i].bitrate > 0) {
      sprintf(buf, "%d", session_desc->codecs()[i].bitrate);
      payload_type->AddAttr(QN_PHONE_PAYLOADTYPE_BITRATE, buf);
    }

    description->AddElement(payload_type);
  }

  return description;
}

Call *PhoneSessionClient::CreateCall() {
  Call *call = new Call(this);
  calls_[call->id()] = call;
  SignalCallCreate(call);
  return call;
}

void PhoneSessionClient::OnSessionCreate(Session *session,
                                         bool received_initiate) {
  if (received_initiate) {
    session->SignalState.connect(this, &PhoneSessionClient::OnSessionState);

    Call *call = CreateCall();
    session_map_[session->id()] = call;
    call->AddSession(session);
  }
}

void PhoneSessionClient::OnSessionState(Session *session,
                                        Session::State state) {
  if (state == Session::STATE_RECEIVEDINITIATE) {
    // If our accept would have no codecs, then we must reject this call.
    PhoneSessionDescription* accept_desc =
        CreateAcceptSessionDescription(session->remote_description());
    if (accept_desc->codecs().size() == 0) {
      // TODO: include an error description with the rejection.
      session->Reject();
    }
    delete accept_desc;
  }
}

void PhoneSessionClient::DestroyCall(Call *call) {
  // Change focus away, signal destruction

  if (call == focus_call_)
    SetFocus(NULL);
  SignalCallDestroy(call);

  // Remove it from calls_ map and delete

  std::map<uint32, Call *>::iterator it = calls_.find(call->id());
  if (it != calls_.end())
    calls_.erase(it);

  delete call;
}

void PhoneSessionClient::OnSessionDestroy(Session *session) {
  // Find the call this session is in, remove it

  std::map<SessionID, Call *>::iterator it = session_map_.find(session->id());
  assert(it != session_map_.end());
  if (it != session_map_.end()) {
    Call *call = (*it).second;
    session_map_.erase(it);
    call->RemoveSession(session);
  }
}

Call *PhoneSessionClient::GetFocus() {
  return focus_call_;
}

void PhoneSessionClient::SetFocus(Call *call) {
  Call *old_focus_call = focus_call_;
  if (focus_call_ != call) {
    if (focus_call_ != NULL)
      focus_call_->EnableChannels(false);
    focus_call_ = call;
    if (focus_call_ != NULL)
      focus_call_->EnableChannels(true);
    SignalFocus(focus_call_, old_focus_call);
  }
}

void PhoneSessionClient::JoinCalls(Call *call_to_join, Call *call) {
  // Move all sessions from call to call_to_join, delete call.
  // If call_to_join has focus, added sessions should have enabled channels.

  if (focus_call_ == call)
    SetFocus(NULL);
  call_to_join->Join(call, focus_call_ == call_to_join);
  DestroyCall(call);
}

Session *PhoneSessionClient::CreateSession(Call *call) {
  Session *session = session_manager_->CreateSession(jid().Str(), NS_PHONE);
  session_map_[session->id()] = call;
  return session;
}

}
