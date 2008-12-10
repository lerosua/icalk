/*
 * =====================================================================================
 *
 *       Filename:  genericplugin.h
 *
 *    Description:  插件基类
 *
 *        Version:  1.0
 *        Created:  2008年10月15日 13时20分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#ifndef GENRIC_PLUGIN_HEADER_INC
#define GENRIC_PLUGIN_HEADER_INC


#define TALK_PLUGIN_MAGIC 0
#define TALK_MAJOR_VERSION 1
#define TALK_MINOR_VERSION 5
#define VERSION "0.15"


typedef struct _TalkPluginInfo  TalkPluginInfo;

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
 * 插件的基本信息
 */
struct _TalkPluginInfo
{
	unsigned int magic;
	unsigned int major_version;
	unsigned int minor_version;
	TalkPluginType type;

	char* id;
	char* name;
	char* version;
	char* summary;
	char* description;
	char* author;
	char* homepage;
};

#ifdef __cplusplus
extern "C" {
#endif

class GenericPlugin
{
	public:
		virtual int action()=0;
		virtual TalkPluginInfo* getPluginInfo()=0;
};



extern GenericPlugin *CreatePlug();




#ifdef __cplusplus
}
#endif


#endif
