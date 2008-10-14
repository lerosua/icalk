
#include <iostream>
#include <glibmm/module.h>
#include "testplugin.h"
int main()
{
     GenericPlugin* (*CreatePlug)();
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
         }
         else
             std::cout << "Error: " << plugin.get_last_error() << std::endl;
     }
        return 0;
}
