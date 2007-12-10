/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
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

#include "talk/base/autodetectproxy.h"
#include "talk/base/httpcommon.h"
#include "talk/xmpp/xmppclientsettings.h"
#include "talk/base/proxydetect.h"

using namespace talk_base;

enum { MSG_TIMEOUT = SignalThread::ST_MSG_FIRST_AVAILABLE };

const talk_base::ProxyType TEST_ORDER[] = {
  talk_base::PROXY_HTTPS, /*talk_base::PROXY_SOCKS5,*/ talk_base::PROXY_UNKNOWN
};

AutoDetectProxy::AutoDetectProxy(const std::string& user_agent)
: agent_(user_agent), socket_(NULL), next_(0)
{
}

AutoDetectProxy::~AutoDetectProxy() {
  delete socket_;
}

void AutoDetectProxy::DoWork() {
  if (!server_url_.empty()) {
    LOG(LS_INFO) << "GetProxySettingsForUrl(" << server_url_ << ") - start";
    GetProxySettingsForUrl(agent_.c_str(), server_url_.c_str(), proxy_, true);
    LOG(LS_INFO) << "GetProxySettingsForUrl - stop";
  }
  talk_base::Url<char> url(proxy_.address.IPAsString());
  if (url.valid()) {
    LOG(LS_WARNING) << "AutoDetectProxy removing http prefix on proxy host";
    proxy_.address.SetIP(url.server());
  }
  if (proxy_.type == talk_base::PROXY_UNKNOWN) {
    //LOG(LS_INFO) << "Proxy classification start";
    Next();
    // Process I/O until Stop()
    Thread::Current()->ProcessMessages(kForever);
  }
}

void AutoDetectProxy::OnMessage(Message *msg) {
  if (MSG_TIMEOUT == msg->message_id) {
    OnCloseEvent(socket_, ETIMEDOUT);
  } else {
    SignalThread::OnMessage(msg);
  }
}

void AutoDetectProxy::Next() {
  if (TEST_ORDER[next_] >= talk_base::PROXY_UNKNOWN) {
    Complete(talk_base::PROXY_UNKNOWN);
    return;
  }

  LOG(LS_VERBOSE) << "AutoDetectProxy connecting to "
                  << proxy_.address.ToString();

  if (socket_) {
    Thread::Current()->Clear(this, MSG_TIMEOUT);
    socket_->Close();
    Thread::Current()->Dispose(socket_);
    socket_ = NULL;
  }

  socket_ = Thread::Current()->socketserver()->CreateAsyncSocket(SOCK_STREAM);
  socket_->SignalConnectEvent.connect(this, &AutoDetectProxy::OnConnectEvent);
  socket_->SignalReadEvent.connect(this, &AutoDetectProxy::OnReadEvent);
  socket_->SignalCloseEvent.connect(this, &AutoDetectProxy::OnCloseEvent);
  socket_->Connect(proxy_.address);

  // Timeout after 2 seconds
  Thread::Current()->PostDelayed(2000, this, MSG_TIMEOUT);
}

void AutoDetectProxy::Complete(talk_base::ProxyType type) {
  Thread::Current()->Clear(this, MSG_TIMEOUT);
  socket_->Close();

  proxy_.type = type;
  talk_base::LoggingSeverity sev
    = (proxy_.type == talk_base::PROXY_UNKNOWN)
      ? talk_base::LS_ERROR : talk_base::LS_VERBOSE;
  LOG_V(sev) << "AutoDetectProxy detected " << proxy_.address.ToString()
             << " as type " << proxy_.type;

  Thread::Current()->MessageQueue::Stop();
}

void AutoDetectProxy::OnConnectEvent(talk_base::AsyncSocket * socket) {
  std::string probe;

  switch (TEST_ORDER[next_]) {
  case talk_base::PROXY_HTTPS:
    probe.assign("\005\001"
                 "CONNECT www.google.com:443 HTTP/1.0\r\n"
                 "User-Agent: ");
    probe.append(agent_);
    probe.append("\r\n"
                 "Host: www.google.com\r\n"
                 "Content-Length: 0\r\n"
                 "Proxy-Connection: Keep-Alive\r\n"
                 "\r\n");
    //probe = "CONNECT www.google.com:443 HTTP/1.0\r\n\r\n";
    break;
  case talk_base::PROXY_SOCKS5:
    probe.assign("\005\001\000", 3);
    break;
  }

  LOG(LS_VERBOSE) << "AutoDetectProxy probing type " << TEST_ORDER[next_]
                  << " sending " << probe.size() << " bytes";
  socket_->Send(probe.data(), probe.size());
}

void AutoDetectProxy::OnReadEvent(talk_base::AsyncSocket * socket) {
  char data[257];
  int len = socket_->Recv(data, 256);
  if (len > 0) {
    data[len] = 0;
    LOG(LS_VERBOSE) << "AutoDetectProxy read " << len << " bytes";
  }

  switch (TEST_ORDER[next_]) {
  case talk_base::PROXY_HTTPS:
    if ((len >= 2) && (data[0] == '\x05')) {
      Complete(talk_base::PROXY_SOCKS5);
      return;
    }
    if ((len >= 5) && (strncmp(data, "HTTP/", 5) == 0)) {
      Complete(talk_base::PROXY_HTTPS);
      return;
    }
    break;
  case talk_base::PROXY_SOCKS5:
    if ((len >= 2) && (data[0] == '\x05')) {
      Complete(talk_base::PROXY_SOCKS5);
      return;
    }
    break;
  }

  ++next_;
  Next();
}

void AutoDetectProxy::OnCloseEvent(talk_base::AsyncSocket * socket, int error) {
  LOG(LS_VERBOSE) << "AutoDetectProxy closed with error: " << error;
  ++next_;
  Next();
}
