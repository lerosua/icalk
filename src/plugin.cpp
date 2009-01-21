/*
 * =====================================================================================
 *
 *       Filename:  plugin.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年10月15日 15时44分16秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first_name last_name (fl), fl@my-company.com
 *        Company:  my-company
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include "plugin.h"
#include "icalk.h"

PluginManager::PluginManager():search_paths(NULL)
			       ,plugins(NULL)
				,loaded_plugins(NULL)
{
	char buf[512];
	char* homedir = getenv("HOME");
	snprintf(buf,512,"%s/.icalk/plugins",homedir);
	add_search_path(buf);
}

void PluginManager::probe(const char *ext)
{
	GDir *dir;
	const gchar *file;
	gchar *path;
	GenericPlugin* plugin;
	GList *cur;
	const char *search_path;

	//if (!g_module_supported())
	//	return;

	/* Probe plugins */
	for (cur = search_paths; cur != NULL; cur = cur->next)
	{
		search_path = (const char*)cur->data;

		dir = g_dir_open(search_path, 0, NULL);

		if (dir != NULL)
		{
			while ((file = g_dir_read_name(dir)) != NULL)
			{
				path = g_build_filename(search_path, file, NULL);

				if (ext == NULL || has_file_extension(file, ext))
				{
					plugin = plugin_probe(path);
					plugins = g_list_append(plugins,plugin);
				}

				g_free(path);
			}

			g_dir_close(dir);
		}
	}

}

GenericPlugin* PluginManager::plugin_probe(const char* path)
{
	/*just testing for probe plugin*/
	DLOG("loading plugin %s\n",path);
#if 1
     GenericPlugin* (*CreatePlug)();

     Glib::Module *m_module=new Glib::Module(path);
     if(m_module->gobj()!=NULL)
     {
	     if(m_module->get_symbol("CreatePlug", (void*&)CreatePlug))
	     {
		     GenericPlugin *m_plugin=CreatePlug();
		     //TalkPluginInfo* info=m_plugin->getPluginInfo();
		     //DLOG("get plugin homepage  : %s\n",info->homepage);
		     //delete m_module;

			return m_plugin;
	     }
	     else
	     {
		 DLOG("Error: %s\n",m_module->get_last_error().c_str());
		 goto out;
	     }

     }
     else
     {
		 DLOG("Error: %s\n",m_module->get_last_error().c_str());
		 goto out;
     }
	     
out:
     delete m_module;
     return NULL;

#endif
}

//void PluginManager::destory(Plugin* f_plugin)
//{
//}
void PluginManager::add_search_path(const char* path)
{
	g_return_if_fail(path !=NULL);
	if(g_list_find_custom(search_paths,path,(GCompareFunc)strcmp))
		return;
	search_paths = g_list_append(search_paths,strdup(path));
}

bool PluginManager::has_file_extension(const char *filename, const char *ext)
{
	int len, extlen;

	if (filename == NULL || *filename == '\0' || ext == NULL)
		return 0;

	extlen = strlen(ext);
	len = strlen(filename) - extlen;

	if (len < 0)
		return 0;

	return (strncmp(filename + len, ext, extlen) == 0);
}
