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
#include <assert.h>
#include <sys/stat.h>
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
        if (!vcard) {
                DLOG("empty %s's vcard!\n", jid.full().c_str());
                return ;
        }

        Bodies& bodies = Bodies::Get_Bodies();
        const JID& myjid = bodies.get_jid();

        char *random =
                g_strdup_printf("%x", g_random_int());
        const char *dirname = GUnit::getIconPath();
        char *filename =
                g_build_filename(dirname, random, NULL);
        int f_size;
        int n_size;

        struct stat f_stat;
        DLOG("geting %s vcard\n", jid.username().c_str());

        //获取自己的vcard照片

        if (myjid.bare() == jid.bare()) {
                //bodies.set_vcard(vcard);

                const std::string m_file = bodies.getAccountTag("icon");

                if ((!m_file.empty())
                                && (!access(m_file.c_str(), F_OK))) {


                        if (stat(m_file.c_str(), &f_stat))
                                return ;

                        f_size = f_stat.st_size;
                } else
                        f_size = 0;

                if (!vcard->photo().type.empty()) {
                        n_size = vcard->photo().binval.size();

                        if (f_size != n_size) {
                                DLOG("orig picture size = %d,new size= %d\n", f_size, n_size);
                                std::ofstream fout(filename);
                                fout.write((const char *) vcard->
                                           photo().binval.c_str(),
                                           vcard->photo().binval.
                                           size());
                                fout.close();

                                bodies.setAccountTag("icon", filename);
                                bodies.get_main_window().set_logo(filename);
                        }
                }

        }

        Buddy *buddy =
                bodies.get_buddy_list().find_buddy(jid.
                                                   bare
                                                   ());
        assert(buddy);
        //buddy->set_vcard(vcard);

        const std::string & buddyname = buddy->get_jid();
        const std::string & f_file =
                bodies.get_main_window().get_buddy_view().getBlistTag("buddy", buddyname, "icon");

        if ((!f_file.empty())
                        && (!access(f_file.c_str(), F_OK))) {

                if (stat(f_file.c_str(), &f_stat))
                        return ;

                f_size = f_stat.st_size;
        } else
                f_size = 0;

        if (!vcard->photo().binval.empty()) {
                n_size = vcard->photo().binval.size();

                if (f_size != n_size) {
                        DLOG("orig picture size = %d,new size= %d\n", f_size, n_size);
                        std::ofstream fout(filename);
                        fout.write((const char *) vcard->
                                   photo().binval.c_str(),
                                   n_size);
                        fout.close();
                        bodies.get_main_window().get_buddy_view().setBlistTag("buddy",
                                        buddyname,
                                        "icon",
                                        filename);
                }
        }


        buddy->refreshinfo();
}

void TalkCard::handleVCardResult(VCardContext context, const JID & jid,
                                 StanzaError se)
{
        DLOG("jid vcard result error -- %s --StanzaError id %d\n", jid.bare().c_str(), se);
}
