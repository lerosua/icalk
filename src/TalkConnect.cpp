/*
* =====================================================================================
* 
*       Filename:  TalkConnect.cpp
* 
*    Description:  连接处理类
* 
*        Version:  1.0
*        Created:  2007年07月1日 16时52分02秒 CST
*       Revision:  none
*       Compiler:  gcc
* 
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
* 
* =====================================================================================
*/

#include "TalkConnect.h"
#include <gtkmm.h>
#include <gtkmm/dialog.h>
#include "TalkCard.h"
#include "Bodies.h"


TalkConnect::TalkConnect()
{}

TalkConnect::~TalkConnect()
{}


void TalkConnect::onConnect()
{
        std::cout << "Talk had connected ..." << std::endl;

        Bodies::Get_Bodies().get_main_window().on_login_finial();
        Bodies::Get_Bodies().get_main_window().on_combo_change();
        Bodies::Get_Bodies().get_main_window().initRoom();
        Bodies::Get_Bodies().fetch_self_vcard();
        //      TalkBookMark& bookmark = Bodies::Get_Bodies().get_bookmark();
        //      bookmark.requestBookmarks();
}

void TalkConnect::onDisconnect(ConnectionError er)
{
        if (ConnNoError == er)
                return ;

        std::cout << "Talk onDisconnect error in " << er << std::endl;

        Gtk::MessageDialog askDialog("连接错误",
                                     false /*use markup */ ,
                                     Gtk::MESSAGE_QUESTION,
                                     Gtk::BUTTONS_OK_CANCEL);

        int result;

        switch (er)
        {
                /*
                   case 0:
                   std::cout<<"ConnNoError "<< er << std::endl;
                   break;
                 */

        case ConnStreamError:
                std::cout << "ConnStreamError" << er << std::endl;
                std::cout << "streamError " << Bodies::Get_Bodies().get_client().streamError() << std::endl;
                askDialog.
                set_secondary_text
                ("连接错误，连接流错误\n重新连接请按确定");
                break;

        case ConnStreamVersionError:
                std::cout << "ConnStreamVersionError" << er << std::endl;
                askDialog.
                set_secondary_text
                ("连接流版本错误\n重新连接请按确定");
                break;

        case ConnStreamClosed:
                std::cout << "ConnStreamClosed " << er << std::endl;
                askDialog.
                set_secondary_text
                ("连接流关闭\n重新连接请按确定");
                break;

        case ConnProxyAuthRequired:
                std::cout << "ConnProxyAuthRequired " << er << std::endl;
                askDialog.
                set_secondary_text
                ("连接代理认证错误\n重新连接请按确定");
                break;

        case ConnProxyAuthFailed:
                std::cout << "ConnProxyAuthFailed " << er << std::endl;
                askDialog.
                set_secondary_text
                ("连接代理认证失败\n重新连接请按确定");
                break;

        case ConnProxyNoSupportedAuth:
                std::cout << "ConnProxyNoSupportedAuth " << er << std::
                endl;
                askDialog.
                set_secondary_text
                ("连接代理不支持认证\n重新连接请按确定");
                break;

        case ConnIoError:
                std::cout << "ConnIoError " << er << std::endl;
                askDialog.
                set_secondary_text
                ("连接IO错误，可能引起CPU占用。\n按确定关闭程序。");
                break;

        case ConnParseError:
                std::cout << "ConnParseError " << er << std::endl;
                askDialog.
                set_secondary_text
                ("ConnParseError\n按确定关闭程序。");
                break;

        case ConnConnectionRefused:
                std::cout << "ConnConnectionRefused " << er << std::endl;
                askDialog.
                set_secondary_text("连接拒绝。请重新连接");
                break;

        case ConnDnsError:
                std::cout << "ConnDnsError " << er << std::endl;
                askDialog.
                set_secondary_text("DNS错误。请重新连接");
                break;

        case ConnOutOfMemory:
                std::cout << "ConnOutOfMemory " << er << std::endl;
                askDialog.
                set_secondary_text("内存错误。请重新连接");
                break;

        case ConnNoSupportedAuth:
                std::cout << "ConnNoSupportedAuth " << er << std::endl;
                askDialog.
                set_secondary_text
                ("连接不支持认证。请重新连接");
                break;

        case ConnTlsFailed:
                std::cout << "ConnTlsFailed " << er << std::endl;
                askDialog.
                set_secondary_text
                ("TLS连接失败。请重新连接");
                break;

        case ConnCompressionFailed:
                std::cout << "ConnCompressionFailed " << er << std::endl;
                askDialog.
                set_secondary_text
                ("CompressionFailed。请重新连接");
                break;

        case ConnAuthenticationFailed:
                std::cout << "ConnAuthenticationFailed " << er << std::
                endl;
                askDialog.
                set_secondary_text
                ("密码错误或者用户名错误\n重新连接请按确定");
                break;
        }

        result = askDialog.run();

        switch (result)
        {
        case (Gtk::RESPONSE_OK):
                {
                        std::cout << "OK clicked" << std::endl;
                        Bodies::Get_Bodies().logout();
                        Bodies::Get_Bodies().get_main_window().
                        on_relogin();
                        break;
                }

        case (Gtk::RESPONSE_CANCEL):
                {
                        std::cout << "Cancel clicked" << std::endl;
                        //Bodies::Get_Bodies().logout();
                        Bodies::Get_Bodies().get_main_window().on_quit();
                        break;
                }

        default:
                {
                        std::cout << "nothing clicked" << std::endl;
                        Bodies::Get_Bodies().logout();
                        break;
                }
        }


        if (ConnUserDisconnected == er)
                std::cout << "ConnUserDisconnected " << er << std::endl;
        else if (ConnNotConnected == er)
                std::cout << "ConnNotConnected " << er << std::endl;
}

void TalkConnect::onResourceBindError(ResourceBindError error)
{
        std::cout << "Talk onResourceBindError ..." << std::endl;
}

void TalkConnect::onSessionCreateError(SessionCreateError error)
{
        std::cout << "Talk onSessionCreateError ..." << std::endl;
}

bool TalkConnect::onTLSConnect(const CertInfo & info)
{
        std::cout << "Talk had TLS connected ..." << std::endl;

        printf
        ("status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
         "from: %s\nto: %s\n", info.status, info.issuer.c_str(),
         info.server.c_str(), info.protocol.c_str(), info.mac.c_str(),
         info.cipher.c_str(), info.compression.c_str(),
         ctime((const time_t *) &info.date_from),
         ctime((const time_t *) &info.date_to));
        return true;


}

void TalkConnect::onStreamEvent(StreamEvent event)
{
        std::cout << "Talk onStreamEvent.." << std::endl;
}

void TalkConnect::handleLog(LogLevel level, LogArea area, const std::string& message)
{
        //printf("log: level: %d, area %d, %s\n",level, area,message.c_str());

}
