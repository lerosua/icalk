/*
  Copyright (c) 2006-2007 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef FCAPS_H__
#define FCAPS_H__

#include <gloox/stanzaextension.h>
#include <gloox/macros.h>

#include <string>

namespace gloox
{

const std::string XMLNS_C_CAPS = "http://jabber.org/protocol/caps";

  class Tag;

  class GLOOX_API FCAPS : public StanzaExtension
  {
    public:
	FCAPS(const std::string& ver,const std::string& ext);
	    FCAPS(Tag* tag);
	    virtual ~FCAPS();
		
	    const std::string& ext()const{ return m_ext;}
	    const std::string& ver()const{ return m_ver;}

	    Tag* tag()const;
    private:
	    std::string m_ext;
	    std::string m_ver;

  };
}
#endif
