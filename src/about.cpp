
#include "about.h"

createAboutDialog::createAboutDialog()
{
        //GlademmXML about_xml = Gnome::Glade::Xml::create(about_ui);
		GBuilderXML about_xml = Gtk::Builder::create_from_file(about_ui);
        Gtk::AboutDialog* aboutDialog = 0;
        about_xml->get_widget("aboutdialog", aboutDialog);
        aboutDialog->set_license("GPL");
        //aboutDialog->show();
        aboutDialog->run();
        aboutDialog->hide();
}
