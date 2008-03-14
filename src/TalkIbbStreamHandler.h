/*
 * =====================================================================================
 * 
 *       Filename:  IbbStreamHandler.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  2007年10月21日 15时39分04秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  IBBSTREAMHANDLER_FILE_HEADER_INC
#define  IBBSTREAMHANDLER_FILE_HEADER_INC

#include <gloox/inbandbytestreamhandler.h>
#include <gloox/inbandbytestreamdatahandler.h>
#include <gloox/inbandbytestreammanager.h>
#include <gloox/inbandbytestream.h>
#include <map>

using namespace gloox;

/**
 * @brief 带内数据传输。在xml流里传输base64编码的数据以达到传输小文件的目的。
 * 这里主要是用于扩展来发达小图片，达到有发送自定义表情的功能。
 * 继承自
 * @link InBandBytestreamHandler gloox::InBandBytestreamHandler @endlink 类.
 * @link InBandBytestreamDataHandler gloox::InBandBytestreamDataHandler @endlink 类.
 * @author lerosua <lerosua@gmail.com>
 */

class TalkIbbStreamHandler: public InBandBytestreamHandler, public InBandBytestreamDataHandler {

public:
        typedef std::map<std::string, InBandBytestream*> IBBSList;
        TalkIbbStreamHandler();
        ~TalkIbbStreamHandler();

        /** 用于初始化talk_ibbManager类*/
        void init(Client* client_);

        /** 对远端实体发起一个IBB请求*/
        void newIBBrequest(const std::string& jid_);
        void newIBBrequest(JID& jid);
        /** 发送字节流函数*/
        void sendIBBData(const std::string& data);

        /**
         * @brief 当有新的IBB请求时的通知。使用 @link InBandBytestream::attachTo()@endlink将IBB
         * 绑定到一个 @link MessageSession @endlink中。
         * 
         * @li 对于同步的 <br>InBandBytestreamHandler:
         * 如果返回值为True，则参数中的bytestream将被接收，而此InBandBytestreamHandler处理类
         * 将变成此InBandBytestream对象的主管。如果返回False，参数中的 InBandBytestream将
         * 会被删除，并且拒绝对方的bytestream请求。
         *
         * @li 对于异步的 <br>InBandBytestreamHandler:
         * 返回值将会被忽略。您必须分别调用 InBandBytestreamManager::accptInBandBytestream()
         * 或者 InBandBytestreamManager::rejectInBandBytestream()来处理情况。
         *
         * @param from 远端实体ID，bytestream的请求者。
         * @param ibb  bytestream流。
         * @return 返回真则接受字节流，返回假则拒绝字节流。
         *
         * @note 您不应该在这个函数中使用这个字节流(bytestream)来发送任何数据。字节流将
         * 会被接收只有当这个函数返回后。
         */
        bool  handleIncomingInBandBytestream(const JID& from, InBandBytestream* ibb);

        /**
         * @brief 当发布一个外送的IBB请求成功时调用此函数。使用InBandBytestream::attachTo()
         * 将此IBB绑定到一个MessageSession中。此流已经被远端实体接受并且准备好发送数据。
         * 此 InBandBytestreamHandler(此处为IbbStreamHandler)将变成此InBandBytestream对
         * 象的主管。
         *
         * @param to 远端实体的JID
         * @param ibb 新的字节流
         */
        void handleOutgoingInBandBytestream(const JID& to, InBandBytestream* ibb);

        /**
         * @brief 当字节流请求遇到错误时回调此函数。例如，远端实体并未实现IBB时会调用。
         *
         * @param remote 远端实体的JID
         * @param se  错误号
         */
        void handleInBandBytestreamError(const JID& remote, StanzaError se);

        /**
         * @brief 此函数前所未有从bytestream发过来的数据.
         * 接收到的数据可能只是所有数据的一个块（一部分）（取决于你想你所发
         * 数据的数量），但任何情况下，它的大小不超过之前你用bytestream协商
         * 的块大小(blocksize)
         * @param data 实现的数据流，并未用base64加密的.
         * @param sid 流id号
         */

        /**下面为 InBandBytestreamDataHandler的回调函数*/
        void handleInBandData(const std::string& data, const std::string& sid);

        /**
         * @brief 当使用bytestream时出错的回调，当此handler被调用时，流已经被关闭。
         * @param sid 流ID号
         * @param remote 远端实体。
         * @param se 错误代号。
         */
        void handleInBandError(const std::string& sid, const JID& remote, StanzaError se);
        /**
         * @brief 当给定的JID关闭流时回调此函数
         * @param sid 关闭流的ID
         * @param from 关闭流的远端实体的JID
         */
        void handleInBandClose(const std::string& sid, const JID& from);
        /** 关闭所有的InBandBytestream 流*/
        void closeIBBStream(const std::string& jid);
        /** 关闭sid号的接收流*/
        void closeRecvIBB(const std::string& sid);
        void close(const std::string& jid);

private:
        InBandBytestreamManager* talk_ibbManager;
        IBBSList ibbsendList;
        IBBSList ibbrecvList;
};

#endif   /* ----- #ifndef IBBSTREAMHANDLER_FILE_HEADER_INC  ----- */

