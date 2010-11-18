
#include <stdio.h>
#include <iostream>
#include <glibmm/module.h>
#include "genericplugin.h"
int main()
{
     GenericPlugin* (*CreatePlug)();
     TalkPluginInfo* (*getPluginInfo)();
     Glib::Module plugin("./libsample.so");
     if(!plugin)
         std::cout << "Error: " << plugin.get_last_error() << std::endl;
     else
     {
         std::cout << "Name of the plugin: " << plugin.get_name() <<
std::endl;
         if(plugin.get_symbol("CreatePlug", (void*&)CreatePlug))
         {
          GenericPlugin *loaded_plugin = CreatePlug();
             std::cout << "Function action() returns: " <<
loaded_plugin->action() << std::endl;
		 TalkPluginInfo* info=loaded_plugin->getPluginInfo();
	     printf("get plugin homepage  : %s\n",info->homepage);
         }
         else
             std::cout << "Error: " << plugin.get_last_error() << std::endl;

	 /*
	 if(plugin.get_symbol("getPluginInfo",(void*&)getPluginInfo))
	 {
		 TalkPluginInfo* info=getPluginInfo();
		 printf("plugin author is %s\n",info->author);

	 }
	 else
             std::cout << "Error: " << plugin.get_last_error() << std::endl;
	     */
     }
        return 0;
}
