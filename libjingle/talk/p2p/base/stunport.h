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

#ifndef __STUNPORT_H__
#define __STUNPORT_H__

#include "talk/base/asyncudpsocket.h"
#include "talk/p2p/base/udpport.h"
#include "talk/p2p/base/stunrequest.h"

namespace cricket {

extern const std::string STUN_PORT_TYPE;

// Communicates using the address on the outside of a NAT.
class StunPort : public UDPPort {
public:
  StunPort(talk_base::Thread* thread, talk_base::SocketFactory* factory, 
           talk_base::Network* network, 
           const talk_base::SocketAddress& local_addr, 
           const talk_base::SocketAddress& server_addr);
  virtual ~StunPort();

  const talk_base::SocketAddress& server_addr() const { return server_addr_; }
  void set_server_addr(const talk_base::SocketAddress& addr) 
    { server_addr_ = addr; }

  const talk_base::SocketAddress& server_addr2() const { return server_addr2_; }
  void set_server_addr2(const talk_base::SocketAddress& addr) 
    { server_addr2_ = addr; }

  virtual void PrepareAddress();

  // This will contact the secondary server and signal another candidate
  // address for this port (which may be the same as the first address).
  void PrepareSecondaryAddress();

  talk_base::SocketAddress GetLocalAddress() const {
    if (socket_) 
      return socket_->GetLocalAddress();
    return talk_base::SocketAddress();
  }

  virtual int SetOption(talk_base::Socket::Option opt, int value);
  virtual int GetError();

protected:
  virtual int SendTo(const void* data, size_t size, 
                     const talk_base::SocketAddress& addr, bool payload);

  void OnReadPacket(
      const char* data, size_t size, const talk_base::SocketAddress& remote_addr,
      talk_base::AsyncPacketSocket* socket);

private:
  talk_base::AsyncPacketSocket* socket_;
  talk_base::SocketAddress server_addr_;
  talk_base::SocketAddress server_addr2_;
  StunRequestManager requests_;
  int error_;

  friend class StunPortBindingRequest;

  // Sends STUN requests to the server.
  void OnSendPacket(const void* data, size_t size, StunRequest* req);
};

} // namespace cricket

#endif // __STUNPORT_H__
