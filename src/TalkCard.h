/*
 * =====================================================================================
 * 
 *       Filename:  TalkCard.h
 * 
 *    Description:  Talk版本的VCard处理类
 * 
 *        Version:  1.0
 *        Created:  2007年06月30日 16时52分02秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  TALKCARD_FILE_HEADER_INC
#define  TALKCARD_FILE_HEADER_INC

#include <gloox/vcardhandler.h>
#include <gloox/vcardmanager.h>
#include <gloox/vcard.h>
#include <gloox/client.h>
using namespace gloox;

class Bodies;
/**
 * @brief 处理VCard事件
 *
 */
class TalkCard : public VCardHandler
{
	public:
		TalkCard ();
		~TalkCard ();
		void fetch_vcard(const JID& jid);
		void store_vcard(VCard* vcard_);
		void set_manage(Client* jclient_);
		/**
		 * @brief 当VCard成功取回时调用此函数。
		 * @param jid 用户id
		 * @param vcard 用户的VCard
		 */
		void handleVCard(const JID& jid, VCard *vcard);

		/**
		 * @brief 函数在存贮VCard操作后或发生错误后调用
		 * @param context 某操作 FetchVCard/StoreVCard
		 * @param jid  用户id
		 * @param se  错误。没错误时为 StanzaErrorUndefined
		 */
		void handleVCardResult(VCardContext context, const JID& jid, StanzaError se);
		

	private:
		VCardManager* vcardmanager_;	

};
#endif   /* ----- #ifndef TALKCARD_FILE_HEADER_INC  ----- */

