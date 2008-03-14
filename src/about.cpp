
#include "about.h"

createAboutDialog::createAboutDialog()
{
        GlademmXML about_xml = Gnome::Glade::Xml::create(about_ui);
        Gtk::AboutDialog* aboutDialog = 0;
        about_xml->get_widget("aboutdialog", aboutDialog);
        aboutDialog->set_license("GPL");
        aboutDialog->show();
}
