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
#include "genericplugin.h"


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


/**
 *@brief  插件管理
 * 查找所有插件并加载需要加载的插件，保存于一GList中
 */
class PluginManager
{
	public:
		PluginManager();
		/**
		 * @brief 探测插件目录里的插件并一一加载
		 * @param ext 插件的后缀
		 */
		void probe(const char* ext);
		/**
		 * @brief 探测某插件是否加载
		 * @param f_file 插件名
		 * @return 返回值是插件类指针
		 */
		GenericPlugin* plugin_probe(const char* f_file);
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
		const GList* get_plugins_list()const { return plugins;};
	private:
		GList* search_paths;   //插件所找路径
		GList* plugins;
		GList* loaded_plugins;  //加载的插件列表

};

#endif


