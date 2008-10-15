/*
 * =====================================================================================
 *
 *       Filename:  sample.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年10月14日 16时29分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */

#include <iostream>
#include "sample.h"

SamplePlugin::SamplePlugin()
{}

int SamplePlugin::action()
{
	std::cout<<"Hello form sample plug-in!"<<std::endl;
	return 0;

}

TalkPluginInfo* SamplePlugin::getPluginInfo()
{
	
	return &info;

}
	
GenericPlugin * CreatePlug()
{
	return new SamplePlugin;
}
