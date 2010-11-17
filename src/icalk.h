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
*       Compiler:  g++
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
#define OS  "Linux"
#define DEFAULTSERVER  "talk.google.com"
#define DEFAULTPORT	5222

using std::list;

typedef list<std::string> USERLIST;


/** 定义Buddy的一些类型*/
enum BuddyType { TYPE_FRIEND = 0, TYPE_TRANPORT, TYPE_GROUPCHAT
                 , TYPE_BOT, TYPE_MSN, TYPE_ICQ, TYPE_YAHOO
				 , TYPE_QQ,  TYPE_IRC, TYPE_TWITTER, TYPE_FACEBOOK
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
 * @param role  角色权利？ 比如，只浏览聊天室，可在聊天室发言，聊天室管理员?
 * @param flags  成员标识， 比如，成员改名，成员被踢，成员被屏蔽，成员角色被改变，房间被删除
 * @param status 成员的状态签名
 * @param presence 成员的状态
 */

typedef struct Member
{
        std::string id;
        MUCRoomAffiliation affiliation;
        MUCRoomRole role;
        int flags;
        std::string newNick;
        std::string status;
        Presence::PresenceType presence;
}

Member;

typedef std::map<std::string, Member> MemberMap;

/**
 * why need ## in front of __VA_ARGS__
 * GCC4.1.1.pdf 236LINE
 * 5.14 Macros with a Variable Number of Arguments.
 */
#ifdef __DEBUG_D

#define DLOG(fmt, ...) \
    { \
        char buffer[36] = {0}; \
        time_t t = time(NULL); \
        strftime(buffer, 36, "%F %T ", localtime(&t)); \
        fprintf(stderr, "%s %s|%d| " fmt, \
                buffer, __FILE__, __LINE__, ##__VA_ARGS__); \
    }

#define RLOG(fmt, ...) \
    { \
        char buffer[36] = {0}; \
        time_t t = time(NULL); \
        strftime(buffer, 36, "%F %T ", localtime(&t)); \
        fprintf(stderr, "%s %s|%d| " fmt, \
                buffer, __FILE__, __LINE__, ##__VA_ARGS__); \
    }

#elif __RELEASE_D

#define DLOG(fmt, ...) \
        ;

#define RLOG(fmt, ...) \
    { \
        char buffer[36] = {0}; \
        time_t t = time(NULL); \
        strftime(buffer, 36, "%F %T ", localtime(&t)); \
        fprintf(stdout, "%s " fmt, buffer, ##__VA_ARGS__); \
    }

#else // by default: __RELEASE_D and __DEBUG_D are not present in compilation

#define DLOG(fmt, ...) \
    { \
        char buffer[36] = {0}; \
        time_t t = time(NULL); \
        strftime(buffer, 36, "%F %T ", localtime(&t)); \
        fprintf(stderr, "%s %s|%d| " fmt, \
                buffer, __FILE__, __LINE__, ##__VA_ARGS__); \
    }

#define RLOG(fmt, ...) \
    { \
        char buffer[36] = {0}; \
        time_t t = time(NULL); \
        strftime(buffer, 36, "%F %T ", localtime(&t)); \
        fprintf(stdout, "%s " fmt, buffer, ##__VA_ARGS__); \
    }

#endif


#if defined( __GUNC__)&&(__GNUC__ - 0 >3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 2 ))
 #define ICALK_DEPRECATED __attribute__ ( (__deprecated__) )
#elif defined( _MSC_VER )&&(_MSC_VER >=1300)
 #define ICALK_DEPRECATED __declspec(deprecated)
#else
 #define ICALK_DEPRECATED
#endif

#endif   /* ----- #ifndef ICALK_FILE_HEADER_INC  ----- */

