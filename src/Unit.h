/*
 * =====================================================================================
 * 
 *       Filename:  Unit.h
 * 
 *    Description:  用于初始化工作
 * 
 *        Version:  1.0
 *        Created:  2007年07月12日 22时17分41秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */
/*! @mainpage iCalk API Documentation
 * 
 * @section contents Contents
 * @ref intro_sec <br>
 * @ref test_sec <br>
 * <br>
 *
 * @section intro_sec Introduction
 *
 * iCalk是一个构建在Gloox库上的XMPP通讯软件。运行平台为Linux。使用libglademm,C++技术。目标是提供
 * 一个丰富的适合国人习惯的聊天环境.
 *
 * @note 现在的iCalk还于开发阶段，各种功能有待添加中。
 *
 * @section test_sec 内容
 *
 * 这里其实完全是为了学习Doxygen的文档而写的。呵呵 。有时候写文档也是一种乐趣啊。
 * 再测试一下代码:
 * example:
 * @code
 * class Bodies
 * {
 *
 *	public:
 *		static Bodies& Get_Bodies();
 *
 *	[...]
 * }
 * @endcode
 * class @link Bodies @endlink 是一个用于集合各部件的工厂类，现在来说还兼职了登录处理
 *
 * [...]
 *
 * 有空下回分解
 */
#ifndef  UNIT_FILE_HEADER_INC
#define  UNIT_FILE_HEADER_INC

#include <gtkmm/main.h>
/** 定义Buddy的一些类型*/
#define TYPE_FRIEND  0
#define TYPE_TRANPORT 1
#define TYPE_GROUPCHAT  2
#define TYPE_BOT	3
#define TYPE_MSN	4
#define TYPE_ICQ	5
#define TYPE_YAHOO	6
#define TYPE_OTHER	15
/**定义Buddy的状态的类型，用于扩展组和聊天室*/
#define STATUS_GROUP 9
#define STATUS_ROOM 11

class GUnit
{
	public:
		static void init(const char* user);
		static const char* getImagePath() { return imagePath; }
		static const char* getSoundPath() { return soundPath; }
		static const char* getHomePath()  { return homePath;  }
		static const char* getUserPath()  { return userPath;  }
		static const char* getLogPath()   { return logPath;   }
		static const char* getIconPath()  { return iconPath;  }
		static const char* getTempPath()  { return tempPath;  }

	private:
		static char imagePath[512];
		static char soundPath[512];
		static char homePath[512];
		static char userPath[512];
		static char logPath[512];
		static char iconPath[512];
		static char tempPath[512];
};

#endif   /* ----- #ifndef UNIT_FILE_HEADER_INC  ----- */

