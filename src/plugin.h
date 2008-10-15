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
 * 插件接口类
 */

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

class PluginManager
{
	public:
		PluginManager();
		bool probe(const char* ext);
		GenericPlugin* plugin_probe(const char* path);
		bool load( Plugin* f_plugin);
		bool unload(Plugin* f_plugin);
		/**
		 * 卸载插件并从内存中销毁它
		 * @param f_plugin The plugin handle
		 */
		void destory(Plugin* f_plugin);
		
		void add_search_path(const char* f_path);
		bool has_file_extension(const char* filename,const char* ext);
	private:
		GList* search_paths;
		GList* plugins;
		GList* loaded_plugins;

}

#endif


