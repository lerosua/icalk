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
TalkCard::TalkCard(Client * jclient_): m_client(jclient_)
{
        m_vcardmanage = new VCardManager(jclient_);
}

TalkCard::~TalkCard()
{
        delete m_vcardmanage;
}


void TalkCard::fetch_vcard(const JID & jid)
{
        m_vcardmanage->fetchVCard(jid, this);
}

void TalkCard::store_vcard(VCard * vcard_)
{
        //m_vcardmanage->storeVCard(vcard_, this);
}

void TalkCard::handleVCard(const JID & jid, const VCard * vcard)
{
        JID myjid = Bodies::Get_Bodies().get_jid();

        if (myjid.bare() == jid.bare())
        {
                Bodies::Get_Bodies().set_vcard(vcard);
                printf("geting %s vcard\n", jid.username().c_str());

                if (!vcard)
                {
                        printf("empty vcard!\n");
                        return ;
                }

                std::cout << "nickname is " << vcard->
                nickname() << std::endl;
                //std::cout << "photo is " << vcard->photo().binval << std::endl;

                if (!vcard->photo().type.empty())
                {
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
        else
        {
                Buddy *buddy =
                        Bodies::Get_Bodies().get_buddy_list().find_buddy(jid.
                                        bare
                                        ());

                if (!vcard)
                {
                        //printf("empty vcard!\n");
                        return ;
                }

                buddy->set_vcard(vcard);
                buddy->refreshinfo();
        }
}

void TalkCard::handleVCardResult(VCardContext context, const JID & jid,
                                 StanzaError se)
{
        std::cout << "jid vcard result error? " << jid.bare() << " StanzaError " << se << std::endl;
}
