
#ifndef _ABOUT_GLADE_H
#define _ABOUT_GLADE_H

#include <gtkmm.h>
#include <libglademm/xml.h>
#define about_ui DATA_DIR"/ui/about.glade"


class createAboutDialog
{

public:
        createAboutDialog();
private:
		typedef Glib::RefPtr < Gnome::Glade::Xml > GlademmXML;

};


#endif
