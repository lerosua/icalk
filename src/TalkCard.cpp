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
TalkCard::TalkCard()
{
}
TalkCard::~TalkCard()
{
	delete vcardmanager_;
}

void TalkCard::set_manage(Client * jclient_)
{
	vcardmanager_ = new VCardManager(jclient_);
}

void TalkCard::fetch_vcard(const JID & jid)
{
	vcardmanager_->fetchVCard(jid, this);
}

void TalkCard::store_vcard(VCard * vcard_)
{
	//vcardmanager_->storeVCard(vcard_, this);
}

void TalkCard::handleVCard(const JID & jid, const VCard * vcard)
{
	JID myjid = Bodies::Get_Bodies().get_jid();
	if (myjid.bare() == jid.bare()) {
		Bodies::Get_Bodies().set_vcard(vcard);
		printf("geting %s vcard\n", jid.username().c_str());
		if (!vcard) {
			printf("empty vcard!\n");
			return;
		}
		std::cout << "nickname is " << vcard->
		    nickname() << std::endl;
		std::cout << "url is " << vcard->url() << std::endl;
		std::cout << "photo is " << vcard->photo().binval << std::endl;
	} else {
		Buddy *buddy =
		    Bodies::Get_Bodies().get_buddy_list().find_buddy(jid.
								     bare
								     ());
		if (!vcard) {
			printf("empty vcard!\n");
			return;
		}
		buddy->set_vcard(vcard);
		buddy->refreshinfo();
	}
}

void TalkCard::handleVCardResult(VCardContext context, const JID & jid,
				 StanzaError se)
{
	std::cout<<"jid vcard result error?"<<jid.bare()<<"StanzaError "<<se<<std::endl;
}
