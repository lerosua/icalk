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

// GipsLiteMediaEngine is a GIPS Voice Engine Lite implementation of MediaEngine

#ifndef TALK_SESSION_PHONE_GIPSLITEMEDIAENGINE_H_
#define TALK_SESSION_PHONE_GIPSLITEMEDIAENGINE_H_

#include "talk/third_party/gips/Interface/GipsVoiceEngineLite.h"
#include "talk/third_party/gips/expiration.h"
#include "talk/session/phone/mediaengine.h"

namespace cricket {

class GipsLiteMediaEngine : public MediaEngine {
 public:
  GipsLiteMediaEngine();
  ~GipsLiteMediaEngine() {}
  virtual bool Init();
  virtual void Terminate();
  
  virtual MediaChannel *CreateChannel();
  
  virtual int SetAudioOptions(int options);
  virtual int SetSoundDevices(int wave_in_device, int wave_out_device);
  
  virtual int GetInputLevel() {return gips_.GIPSVE_GetInputLevel();}
  virtual bool FindCodec(const Codec &codec);
  virtual std::vector<Codec> codecs();
  bool FindGIPSCodec(Codec codec, GIPS_CodecInst* gips_codec);
  GipsVoiceEngineLite &gips() {return gips_;};

 private:
  GipsVoiceEngineLite & gips_;
  int GetGIPSCodecPreference(const char *name, int clockrate);
  std::vector<Codec> codecs_;
};

class GipsLiteMediaChannel : public MediaChannel, public GIPS_transport {
 public:
  GipsLiteMediaChannel(GipsLiteMediaEngine *me);
  virtual ~GipsLiteMediaChannel() {
    StopMediaMonitor();
    engine_->gips().GIPSVE_DeleteChannel(gips_channel_);
  }
  virtual void SetCodecs(const std::vector<Codec> &codecs);
  virtual void OnPacketReceived(const void *data, int len);

  virtual void SetPlayout(bool playout);
  virtual void SetSend(bool send);


  virtual int GetOutputLevel() {return engine_->gips().GIPSVE_GetOutputLevel(gips_channel_);}
  GipsLiteMediaEngine *engine();

  virtual void StartMediaMonitor(VoiceChannel *voice_channel, uint32 cms) {}
  virtual void StopMediaMonitor() {}

 private:
  GipsLiteMediaEngine *engine_;
  int gips_channel_;

  virtual int SendPacket(int channel, const void *data, int len) {if (network_interface_) network_interface_->SendPacket(data, len); return 1;}
  virtual int SendRTCPPacket(int channel, const void *data, int len) {return 1;}
};

}  // namespace cricket

#endif  // TALK_SESSION_PHONE_GIPSLITEMEDIAENGINE_H_
