/*
 * =====================================================================================
 *
 *       Filename:  plugin.h
 *
 *    Description:  icalk的插件系统接口
 *
 *        Version:  1.0
 *        Created:  2008年10月14日 09时58分57秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  lerosua (lerosua@gmail.com)
 *
 * =====================================================================================
 */

#ifndef ICALK_PLUGIN_HEADER_INC
#define ICALK_PLUGIN_HEADER_INC

#include <glibmm/module.h>

/**
 * 插件类型.
 */
typedef enum
{
	TALK_PLUGIN_UNKONW = -1,  /**<未知类型 */
	TALK_PLUGIN_STANDARD = 0, /**<独立插件 */
	TALK_PLUGIN_LOADER,	  /**<可加载插件 */
	TALK_PLUGIN_PROTOCOL	  /**<协议插件   */

} TalkPluginType;


/**
 * 插件接口类
 */

class Plugin
{
	public:
		Plugin();

		bool load();
		bool unload();


	private:
		unsigned int magic;
		unsigned int major_version;
		unsigned int minor_version;
		TalkPluginType type;
		
		std::string id;
		std::string name;
		std::string version;
		std::string summary;
		std::string description;
		std::string author;
		std::string homepage;


};

class TalkPlugin
{
	public:
		TalkPlugin();
		bool probe(Plugin* f_plugin);
		bool load( Plugin* f_plugin);
		bool unload(Plugin* f_plugin);
		/**
		 * 卸载插件并从内存中销毁它
		 * @param f_plugin The plugin handle
		 */
		void destory(Plugin* f_plugin);




};

#endif


