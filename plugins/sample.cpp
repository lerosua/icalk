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

int SamplePlugin::action()
{
	std::cout<<"Hello form sample plug-in!"<<std::endl;
	return 0;

}
GenericPlugin * CreatePlug()
{
	return new SamplePlugin;
}
