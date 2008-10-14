/*
 * =====================================================================================
 *
 *       Filename:  testplugin.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年10月14日 16时25分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#ifndef TEST_PLUGIN_GENERIC_H
#define TEST_PLUGIN_GENERIC_H


#ifdef __cplusplus
extern "C" {
#endif

class GenericPlugin
{
	public:
		virtual int action()=0;
};

extern GenericPlugin *CreatePlug();
typedef GenericPlugin *(*CREATEPLUG_PROC)();

#ifdef __cplusplus
}
#endif

#endif
