/*
* =====================================================================================
*
*       Filename:  TalkFT.h
*
*    Description:  gloox的文件传输模块
*
*        Version:  1.0
*        Created:  2007年08月10日 19时01分46秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  TALKFT_FILE_HEADER_INC
#define  TALKFT_FILE_HEADER_INC

#include <gloox/disco.h>
#include <gloox/client.h>
#include <gloox/siprofileft.h>
#include <gloox/bytestreamdatahandler.h>
#include <gloox/siprofilefthandler.h>
#include <gloox/socks5bytestreamserver.h>

using namespace gloox;

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <ios>
#include <map>
#include "XPThread.h"

#if defined( WIN32 ) || defined( _WIN32 )
# include <windows.h>
#endif

class Bodies;
/**
 * 文件传输的类，负责接收与发送文件
 */

class TalkFT: public SIProfileFTHandler, public BytestreamDataHandler
{

public:
        TalkFT(Client * client_);
        ~TalkFT();

        typedef std::map<std::string , std::fstream*> FILELIST;
        /** 初始化SIProfileFT类和接收/发送所需要的proxy服务器*/
        void initFT();
        /** 循环接收流函数 */
        void *loopRecv(void *);

        /** 发送循环发送流 */
        void* loopSend(void* );

        /** 是否发送的流*/
        bool isSend(Bytestream* bs);

        /** 自定义的发送文件的处理*/
        void handleFTSend(const JID& to, const std::string& m_file);
        /** 此函数在对方要发送一个文件给你时调用。*/
        void handleFTRequest(const JID & from,
                             const std::string & sid,
                             const std::string & name, long size,
                             const std::string & hash,
                             const std::string & date,
                             const std::string & mimetype,
                             const std::string & desc, int /*stypes */ ,
                             long /*offset */ , long /*length */ );
        /** 此函数处理文件发送请求错误或者被拒绝时调用*/
        void handleFTRequestError(const IQ & iq, const std::string & sid);
        /** 传输一个协商好的SOCKS5流，此流尚打开也没准备好发送/接收数据*/
        void handleFTBytestream(Bytestream * bs);
        /** 如果协商好用OOB策略传输则调用此函数*/
        virtual const std::string handleOOBRequestResult(const JID & from,
                        const std::
                        string & sid)
        {
                return std::string();
        };


        /**
         * @brief 实际接收流数据的函数。
         * @param bs 传输中的流.
         * @param data 实际接收的数据.
         */
        void handleBytestreamData(Bytestream * bs,
                                  const std::string & data);
        /**
         * @brief 使用流发生错误时调用。当流调用这个函数时流已经是关闭的了。
         * @param bs 流
         * @param stanza 错误描述的iq
         */
        void handleBytestreamError(Bytestream * bs, const IQ & stanza);
        void handleBytestreamOpen(Bytestream * bs);
        void handleBytestreamClose(Bytestream * bs);

private:
        Client* m_client;
        SIProfileFT * m_ft;
        SOCKS5BytestreamServer *m_server;
        std::list < Bytestream * >bs_recvList;
        std::list < Bytestream * >bs_sendList;
        //Bytestream* m_bs_send;
        //std::ifstream sendfile;
        //std::ofstream recvfile;
        FILELIST rfilelist;
        FILELIST sfilelist;
        XPThread < TalkFT > recvThread;
        XPThread < TalkFT > sendThread;
        volatile int recvCount; //接收文件计数
        volatile int sendCount; //发送文件计数
        //XPthreadMutex mutex;
        volatile int R_RUNNING; //接收线程标志
        volatile int S_RUNNING; //发送线程标志

private:
        TalkFT(const TalkFT & rhs); //不用
        TalkFT & operator =(const TalkFT & rh); // 不用
};



#endif    /* ----- #ifndef TALKFT_FILE_HEADER_INC  ----- */
