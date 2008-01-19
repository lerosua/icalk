
#ifndef _ABOUT_GLADE_H
#define _ABOUT_GLADE_H

#include <gtkmm.h>
#include <libglademm/xml.h>
#define about_ui DATA_DIR"/ui/about.glade"

typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

class createAboutDialog
{
	public:
		createAboutDialog();

};


#endif
