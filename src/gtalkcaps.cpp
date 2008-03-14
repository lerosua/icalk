/*
  Copyright (c) 2006-2007 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox
 
  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.
 
  This software is distributed without any warranty.
*/


#include <gloox/tag.h>
#include "gtalkcaps.h"

namespace gloox {

GtalkCaps::GtalkCaps(const std::string& ver, const std::string& ext):
StanzaExtension(ExtCaps), m_ver(ver), m_ext(ext) {}


GtalkCaps::GtalkCaps(Tag* tag):
StanzaExtension(ExtCaps), m_ext("share-v1 voice-v1"), m_ver("1.0.104") {
        if(tag && (tag->name() == "c" && tag->hasAttribute("xmlns", XMLNS_C_CAPS))) {
                tag->addAttribute("note", XMLNS_GOOGLE_CAPS);
                tag->addAttribute("ver", m_ver);
                tag->addAttribute("ext", m_ext);
        }
}

GtalkCaps::~GtalkCaps() {}

Tag* GtalkCaps::tag()const {
        Tag* t = 0;
        t = new Tag("c");
        t->addAttribute("note", XMLNS_GOOGLE_CAPS);
        t->addAttribute("ver", m_ver);
        t->addAttribute("ext", m_ext);
        t->addAttribute("xmlns", XMLNS_C_CAPS);

        return t;
}

const std::string& GtalkCaps::filterString()const {
        static const std::string filter = "gtalk ext support ";
        return filter;
}

}
