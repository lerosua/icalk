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

#include "testplugin.h"
class SamplePlugin:public GenericPlugin
{
	public:
		virtual int action();
};




#endif
