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
#include <glib.h>


/**
 * 插件接口类
 */

#if 0
class Plugin
{
	public:
		Plugin();
		bool load();
		bool unload();
	private:

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


class PluginManager
{
	public:
		PluginManager();
		/**
		 * @brief 探测插件目录里的插件并一一加载
		 * @param ext 插件的后缀
		 */
		void probe(const char* ext);
		//GenericPlugin* plugin_probe(const char* path);
		/**
		 * @brief 探测某插件是否加载
		 * @param f_file 插件名
		 */
		void  plugin_probe(const char* f_file);
		/*
		bool load( Plugin* f_plugin);
		bool unload(Plugin* f_plugin);
		*/
		/**
		 * 卸载插件并从内存中销毁它
		 * @param f_plugin The plugin handle
		 */
		//void destory(Plugin* f_plugin);
		
		/** 添加所探测的插件的目录*/
		void add_search_path(const char* f_path);
		/** 检测文件名是否包含了ext所指向的后缀，比如so,dll*/
		bool has_file_extension(const char* filename,const char* ext);
	private:
		GList* search_paths;
		GList* plugins;
		GList* loaded_plugins;

};

#endif


