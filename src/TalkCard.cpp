/*
* =====================================================================================
*
*       Filename:  TalkCard.cpp
*
*    Description:  Talk版VCard类的实现
*
*        Version:  1.0
*        Created:  2007年06月30日 22时20分18秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/
#include "Bodies.h"
#include "TalkCard.h"
TalkCard::TalkCard(Client * f_client): m_client(f_client)
{
        m_vcardmanage = new VCardManager(f_client);
}

TalkCard::~TalkCard()
{
        delete m_vcardmanage;
}


void TalkCard::fetch_vcard(const JID & jid)
{
        m_vcardmanage->fetchVCard(jid, this);
}

void TalkCard::store_vcard(VCard * f_vcard)
{
        //m_vcardmanage->storeVCard(f_vcard, this);
}

void TalkCard::handleVCard(const JID & jid, const VCard * vcard)
{
        JID myjid = Bodies::Get_Bodies().get_jid();

        if (myjid.bare() == jid.bare()) {
                Bodies::Get_Bodies().set_vcard(vcard);
                DLOG("geting %s vcard\n", jid.username().c_str());

                if (!vcard) {
                        DLOG("empty vcard!\n");
                        return ;
                }

                if (!vcard->photo().type.empty()) {
                        char *random =
                                g_strdup_printf("%x", g_random_int());
                        const char *dirname = GUnit::getIconPath();
                        char *filename =
                                g_build_filename(dirname, random, NULL);

                        std::ofstream fout(filename);
                        fout.write((const char *) vcard->
                                   photo().binval.c_str(),
                                   vcard->photo().binval.
                                   size());
                        fout.close();

                        Bodies::Get_Bodies().setAccountTag("icon", filename);
                        Bodies::Get_Bodies().get_main_window().set_logo(filename);
                }

        }

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(jid.
                                bare
                                ());

        if (buddy == NULL)
                return ;

        if (!vcard) {
                return ;
        }

        buddy->set_vcard(vcard);

        buddy->refreshinfo();
}

void TalkCard::handleVCardResult(VCardContext context, const JID & jid,
                                 StanzaError se)
{
	DLOG("jid vcard result error -- %s --StanzaError id %d\n",jid.bare().c_str(),se);
}
