/*
 * =====================================================================================
 *
 *       Filename:  TalkRoomHandler.h
 *
 *    Description: 聊天室中事件处理类 
 *
 *        Version:  1.0
 *        Created:  2007年05月24日 19时43分28秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua xihels, lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */
#ifndef TALK_ROOM_HANDLE_HH_
#define TALK_ROOM_HANDLE_HH_

#include <gloox/mucroomhandler.h>
#include <gloox/mucroom.h>
#include <gloox/client.h>
#include <gloox/message.h>
#include <map>
#include "MsgPage.h"
#include "RoomItem.h"

using namespace gloox;
//class Bodies;
class RoomItem;

/**
 *  @brief 这是用来实现jabber聊天室事件驱动的类(Multi-User Chat).
 *
 *  继承自@link MUCRoomHandler gloox::MUCRoomHandler @endlink 类。
 *
 *  @author lerosua <lerosua@gmail.com>
 *
 */

class TalkRoomHandler:public MUCRoomHandler {
	public:
		TalkRoomHandler(){}

		typedef std::map<std::string,RoomItem*> ROOMLIST;

		/** 用于依次将加入所有的房间*/
		void joinRoom();
		/** 返回已存的房间列表 */
		ROOMLIST& getRoomList() { return roomList; }
		void addRoomItem(const std::string& jid, RoomItem* item);
		/** 查找列表中的房间,id的值为房间号: 如 linuxcn@conference.jabber.org*/
		RoomItem* findRoom(const std::string& id) const;
		RoomItem* findRoom(const MUCRoom* room) const;
		template <class Tfun>
			void for_each(Tfun fun) const { std::for_each(roomList.begin(), roomList.end(), fun); }

		/**
		 * @brief 这个回调函数会在聊天室成员(occupant)进入房间，更改状态或离开房间时被调用。
		 * @note MUCRoomParticipant 结构,以及在它之内的JIDs的指针，将会在函数返回时被清理。
		 * @param room 聊天室房间
		 * @param participant 描述occupant状态或动作的结构
		 * @param presence occupant的状态.
		 */
		void handleMUCParticipantPresence(MUCRoom * room ,
				const MUCRoomParticipant
				participant,
				Presence::PresenceType presence);

		/**
		 * @brief 当聊天室有消息到达时会调用这个函数。
		 * @note 这可能会是一个私有消息！如果是私有消息，并且你想回复它。你应该创建一个新
		 * 的MessageSession来与对方(user's full room nick)通信，
		 *
		 * @param room 消息来源的房间
		 * @param nick 房间里消息发送者的昵称(nickname)
		 * @param message 消息
		 * @param history 
		 * @param when 
		 * @param privateMessage 表明是否私有信息。
		 * @note since 1.0 the code is deprecated.
		void handleMUCMessage(MUCRoom * room ,
				const std::string & nick,
				const std::string & message,
				bool history, const std::string & when  ,
				bool privateMessage);
		 */

		/**
		 * @brief 当聊天室有消息到达时会调用这个函数。
		 * @param room 消息来源的房间
		 * @param msg  消息实体，1.0后采用 Message 类
		 * @param priv 表明是否私有信息
		 */
		void handleMUCMessage(MUCRoom* room,
				const Message& msg,
				bool priv);

		/**
		 * @brief 当聊天室的主题改变时函数会被调用。
		 * @param room 聊天室。
		 * @param nick 更改主题的那位聊天室成员的昵称。
		 * @param subject 聊天室新的主题。
		 */
		void handleMUCSubject(MUCRoom * room ,
				const std::string & nick,
				const std::string & subject);

		/**
		 * @brief 当某个错误发生或者在进入房间时出错调用。
		 * @note 下面的错误是MUC特定的错误
		 * @li @b Not @b Authorized:需要密码。
		 * @li @b Forbidden: 禁止访问，用户被屏蔽。
		 * @li @b Item @b Not @b Found: 房间不存在。
		 * @li @b Not  @b Allowed: 创建房间被限制了。
		 * @li @b Not  @b Acceptable: 房间昵称被锁定。
		 * @li @b Registration @b Required: 用户不在成员列表中。
		 * @li @b Conflict: 请求的房间昵称已经被使用或者注册另一个。
		 * @li @b Service @b Unavailable: 房间已达到人数上限。
		 *
		 * 其它的错误类型也许会出现，这取决于服务器的实现程度。
		 * @param room 聊天室。
		 * @param error 错误。
		 */
		void handleMUCError(MUCRoom * room , StanzaError error);

		/**
		 * @brief 此函数会在调用MUCRoom::getRoomInfo()以后回调。
		 * @param room 聊天室。
		 * @param features ORed MUCRoomFlag's
		 * @param name 服务器查找所需要的房间名。
		 * @param infoForm 包含房间信息的DataForm数据类型。 如果服务器不支持扩展房间信息，将会返回0
		 * 请查看 XEP-0045的15.5节，那里有详细定义。
		 *
		 * @note 此函数也可能在没有调用getRoomInfo()的时候回调，这发生在房间配置被房间的管理更改的时候。
		 *
		 */
		void handleMUCInfo(MUCRoom * room , int features,
				const std::string & name,
				const DataForm * infoForm);
		
		/**
		 * @brief 此函数在调用MUCRoom::getRoomItems()后回调。
		 *
		 * @param room 聊天室.
		 * @param Item 房间成员的list. 现为Item类的列表
		 */
		void handleMUCItems(MUCRoom * room , const Disco::ItemList& items);

		/**
		 * @brief 此函数在用户邀请某人进入房间时（调用MUCRoom::invite（））回调。
		 * but the invitation was declined by that person
		 * @param room 聊天室。
		 * @param invitee  The JID if the person that declined the invitation.
		 * @param reason   An optional  reason for declining the invitation.
		 */
		void handleMUCInviteDecline(MUCRoom * room ,
				const JID & invitee,
				const std::string & reason);
		bool handleMUCRoomCreation(MUCRoom * room);

	private:
		ROOMLIST roomList;
};
#endif
