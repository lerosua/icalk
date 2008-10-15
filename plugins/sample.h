/*
 * =====================================================================================
 *
 *       Filename:  sample.h
 *
 *    Description:  插件示例
 *
 *        Version:  1.0
 *        Created:  2008年10月14日 16时24分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#ifndef SAMPLE_PLUGIN_INC
#define SAMPLE_PLUGIN_INC

#include <iostream>
#include "genericplugin.h"

#define _(x)   x

static TalkPluginInfo info=
{
	TALK_PLUGIN_MAGIC,
	TALK_MAJOR_VERSION,
	TALK_MINOR_VERSION,
	TALK_PLUGIN_LOADER,
	"plugin-sample",	/**< id */
	_("Plugin Sample"),	/**< name */
	VERSION,		/**< version */
	_("just plugin sample"), /**< summary */
	_("just plugin sample"), /**< description */
	"lerosua (http://lerosua.at.3322.org)", /**< author */
	"http://code.google.com/p/icalk"	/**< homepage */
};


class SamplePlugin:public GenericPlugin
{
	public:
		SamplePlugin();
		virtual int action();
		virtual TalkPluginInfo* getPluginInfo();
};




#endif
