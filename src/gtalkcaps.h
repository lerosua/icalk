/*
 Copyright (c) 2006-2007 by Jakob Schroeter <js@camaya.net>
 This file is part of the gloox library. http://camaya.net/gloox

 This software is distributed under a license. The full license
 agreement can be found in the file LICENSE in this distribution.
 This software may not be copied, modified, sold or distributed
 other than expressed in the named license agreement.

 This software is distributed without any warranty.
*/


#ifndef GtalkCaps_H__
#define GtalkCaps_H__

#include <gloox/stanzaextension.h>
#include <gloox/macros.h>

#include <string>

namespace gloox
{

const std::string XMLNS_C_CAPS("http://jabber.org/protocol/caps");
const std::string XMLNS_GOOGLE_CAPS("http://www.google.com/xmpp/client/caps");

class Tag;

class GLOOX_API GtalkCaps : public StanzaExtension
{

public:
        GtalkCaps(const std::string& ver, const std::string& ext);
        GtalkCaps(Tag* tag);
        virtual ~GtalkCaps();

        const std::string& ext()const
        {
                return m_ext;
        }

        const std::string& ver()const
        {
                return m_ver;
        }

        virtual const std::string& filterString() const;
        virtual StanzaExtension* newInstance(const Tag* tag) const
        {
                return new GtalkCaps(tag);
        };


        Tag* tag()const;

private:
        std::string m_ext;
        std::string m_ver;

};
}

#endif
