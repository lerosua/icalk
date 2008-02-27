/*
 * =====================================================================================
 * 
 *       Filename:  icalk.h
 * 
 *    Description:  各种常量定义
 * 
 *        Version:  1.0
 *        Created:  2008年02月22日 20时05分59秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  ICALK_FILE_HEADER_INC
#define  ICALK_FILE_HEADER_INC

#include <gloox/gloox.h>
#include <gloox/presence.h>

#define ICALK_VERSION  "0.04"
#define OS		"Linux"

using std::list;

typedef list<std::string> USERLIST;


/** 定义Buddy的一些类型*/
enum BuddyType
{ TYPE_FRIEND,TYPE_TRANPORT,TYPE_GROUPCHAT
	, TYPE_BOT,TYPE_MSN, TYPE_ICQ, TYPE_YAHOO
	, TYPE_OTHER
};
/**定义Buddy的状态的类型，用于扩展组和聊天室*/
#define STATUS_GROUP 9
#define STATUS_ROOM 11

#ifndef DEBUG
#define PBUG printf
#else
#define PBUG
#endif


using namespace gloox;

/** 
 * @brief 聊天室里成员的数据结构 
 * @param id full jid string, example : linuxcn@jabber.org/lerosua 
 * @param affiliation 成员角色，比如管理员，创建者，普通会员.
 * @param role		角色权利？ 比如，只浏览聊天室，可在聊天室发言，聊天室管理员?
 * @param flags		成员标识， 比如，成员改名，成员被踢，成员被屏蔽，成员角色被改变，房间被删除
 * @param status	成员的状态签名
 * @param presence	成员的状态
 */
typedef struct Member{
	std::string id;
	MUCRoomAffiliation affiliation;
	MUCRoomRole role;
	int flags;
	std::string newNick;
	std::string status;
	Presence::PresenceType presence;
}Member;

typedef std::map<std::string, Member> MemberMap;


#endif   /* ----- #ifndef ICALK_FILE_HEADER_INC  ----- */

