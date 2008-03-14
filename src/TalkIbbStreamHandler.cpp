/*
 * =====================================================================================
 *
 *       Filename:  IbbStreamHandler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年10月21日 16时35分02秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <glib/gi18n.h>
#include "Bodies.h"
#include "TalkIbbStreamHandler.h"
#include "Unit.h"

TalkIbbStreamHandler::TalkIbbStreamHandler() {
}

TalkIbbStreamHandler::~TalkIbbStreamHandler() {
}

void TalkIbbStreamHandler::init(Client * client_) {
        talk_ibbManager = new InBandBytestreamManager(client_);
        talk_ibbManager->setBlockSize(102400);
        //talk_ibbManager->setBlockSize(40960);
        talk_ibbManager->registerInBandBytestreamHandler(this, true);
}

void TalkIbbStreamHandler::newIBBrequest(const std::string & jid_) {
        JID jid(jid_);
        talk_ibbManager->requestInBandBytestream(jid, this);
}

void TalkIbbStreamHandler::newIBBrequest(JID & jid) {
        talk_ibbManager->requestInBandBytestream(jid, this);
}

void TalkIbbStreamHandler::sendIBBData(const std::string & data) {
        IBBSList::iterator iter = ibbsendList.begin();

        for (; iter != ibbsendList.end(); ++iter) {
                JID to((*iter).first);
                Buddy *buddy =
                        Bodies::Get_Bodies().get_buddy_list().find_buddy(to.
                                        bare
                                        ());
                const std::string & customData = buddy->getCustomData();

                if ((*iter).second->sendBlock(customData)) {
                        buddy->cleanIBBstream();
                } else
                        printf("sendBlock error happen\n");
        }
}

bool TalkIbbStreamHandler::handleIncomingInBandBytestream(const JID & from,
                InBandBytestream
                * ibb) {
        talk_ibbManager->acceptInBandBytestream(ibb);
        ibb->registerInBandBytestreamDataHandler(this);
        printf("incomingInbandbytestream by %s\n", from.full().c_str());

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(from.bare());
        assert(buddy != NULL);

        buddy->cleanIBBstream();
        buddy->set_ibbstream(ibb);
        buddy->ibbattachToSession();
        ibbrecvList[from.full()] = ibb;
        return true;
}

/** 在确认能发送的情况下才把InBandBytestream和Session关连*/
void TalkIbbStreamHandler::handleOutgoingInBandBytestream(const JID & to,
                InBandBytestream
                * ibb) {

        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(to.bare());
        assert(buddy != NULL);

        buddy->cleanIBBstream();
        buddy->set_ibbstream(ibb);
        buddy->ibbattachToSession();
        printf("outgoingInbandbytestream by %s\n", to.full().c_str());

        ibbsendList[to.full()] = ibb;
}


void TalkIbbStreamHandler::handleInBandBytestreamError(const JID & remote,
                StanzaError se) {

        printf("字节流传输请求发生错误，实体JID为 %s\n",
               remote.full().c_str());
        printf("也许对方不支持IBB带内字节流传输机制\n");
        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(remote.
                                bare());
        buddy->get_page()->
        showSystemMsg(_
                      ("The remote client does not support InBandBytestream translate now\n"));
}


/**下面为 InBandBytestreamDataHandler的回调函数*/
void TalkIbbStreamHandler::handleInBandData(const std::string & data,
                const std::string & sid) {

        printf("收到字节流数据\n");
        char *random = g_strdup_printf("%x", g_random_int());
        const char *dirname = GUnit::getTempPath();
        char *filename = g_build_filename(dirname, random, NULL);
        std::ofstream fout(filename, std::ios::binary);
        fout.write((const char *) data.c_str(), data.size());
        fout.close();

        IBBSList::iterator iter = ibbrecvList.begin();

        for (; iter != ibbrecvList.end(); ++iter) {
                if (sid == (*iter).second->sid()) {
                        JID from((*iter).first);
                        Buddy *buddy =
                                Bodies::Get_Bodies().get_buddy_list().
                                find_buddy(from.bare());
                        buddy->get_page()->showPicture(filename);
                }
        }


}

void TalkIbbStreamHandler::handleInBandError(const std::string & sid,
                const JID & remote,
                StanzaError se) {
        printf("handleInBandError happen in %d from %s,流id %s\n", se,
               remote.full().c_str(), sid.c_str());
        printf("也许对方不支持IBB带内字节流传输机制\n");

}

/** 这个函数是 InBandBytestream 无端实体关闭时发送过来的报告。在这种情况下
 * 本地也应该关闭这个InBandBytestream了吧~*/
void TalkIbbStreamHandler::handleInBandClose(const std::string & sid,
                const JID & from) {
        printf("字节流传输关闭 sid by %s\n", sid.c_str());
        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(from.bare());

        if (buddy)
                buddy->cleanIBBstream();

}

void TalkIbbStreamHandler::close(const std::string & jid_) {
        JID from(jid_);
        Buddy *buddy =
                Bodies::Get_Bodies().get_buddy_list().find_buddy(from.bare());

        if (buddy)
                buddy->cleanIBBstream();
}

void TalkIbbStreamHandler::closeIBBStream(const std::string & jid) {
        IBBSList::iterator iter = ibbsendList.begin();

        for (; iter != ibbsendList.end(); ++iter) {
                if (jid == (*iter).first) {
                        std::
                        cout << "close send ibbstream for " << (*iter).
                        first << std::endl;
                        talk_ibbManager->dispose((*iter).second);
                        ibbsendList.erase(iter);
                }
        }


        iter = ibbrecvList.begin();

        for (; iter != ibbrecvList.end(); ++iter) {
                if (jid == (*iter).first) {
                        std::
                        cout << "close recvice ibbstream for " <<
                        (*iter).first << std::endl;
                        talk_ibbManager->dispose((*iter).second);
                        ibbrecvList.erase(iter);
                }
        }

}

void TalkIbbStreamHandler::closeRecvIBB(const std::string & sid) {
        IBBSList::iterator iter = ibbrecvList.begin();

        for (; iter != ibbrecvList.end(); ++iter) {
                if (sid == (*iter).second->sid()) {
                        std::
                        cout << "关闭已接收的流 for " <<
                        (*iter).first << std::endl;
                        talk_ibbManager->dispose((*iter).second);
                        ibbrecvList.erase(iter);
                }
        }
}
