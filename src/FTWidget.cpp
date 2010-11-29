/*
* =====================================================================================
*
*       Filename:  FTWidget.cpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  2008年05月13日 17时27分36秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#include <glib/gi18n.h>
#include <sstream>
#include <iomanip>
#include "MainWindow.h"
#include "FTWidget.h"
#include "pixmaps.h"
#include "icalk.h"

FTWidget::FTWidget(MainWindow* f_parent): m_parent(f_parent)
{
        set_title(_("File Transfer"));
        set_border_width(5);
        set_default_size(450, 200);

        //set_transient_for(*m_parent);
        frame = Gtk::manage(new Gtk::Frame());
        //add(*frame);

        add(m_VBox);

        //frame->add(m_VBox);
        m_ScrolledWindow.add(m_TreeView);

        m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

        frame->add(m_ScrolledWindow);

        m_VBox.pack_start(*frame);

        //m_VBox.pack_start(m_ScrolledWindow);
        m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
        m_ButtonBox.set_border_width(5);
        m_ButtonBox.set_layout(Gtk::BUTTONBOX_END);
        m_Button_Continue = Gtk::manage(new Gtk::Button(_("Continue")));
        m_Button_Stop = Gtk::manage(new Gtk::Button(_("Stop")));
        m_Button_Delete = Gtk::manage(new Gtk::Button(_("Delete")));
        m_Button_Quit = Gtk::manage(new Gtk::Button(_("Quit")));

        m_ButtonBox.pack_start(*m_Button_Continue, Gtk::PACK_SHRINK);
        m_ButtonBox.pack_start(*m_Button_Stop, Gtk::PACK_SHRINK);
        m_ButtonBox.pack_start(*m_Button_Delete, Gtk::PACK_SHRINK);
        m_ButtonBox.pack_start(*m_Button_Quit, Gtk::PACK_SHRINK);

        m_Button_Continue->signal_clicked().connect(sigc::mem_fun(*this,
                        &FTWidget::on_button_continue));
        m_Button_Stop->signal_clicked().connect(sigc::mem_fun(*this,
                                                &FTWidget::on_button_stop));
        m_Button_Delete->signal_clicked().connect(sigc::mem_fun(*this,
                        &FTWidget::on_button_del));
        m_Button_Quit->signal_clicked().connect(sigc::mem_fun(*this,
                                                &FTWidget::on_button_quit));

        //设置默认的按钮状态
        m_Button_Continue->set_sensitive(false);
        m_Button_Stop->set_sensitive(false);
        m_Button_Delete->set_sensitive(true);

        //Create the TreeModel;
        m_refTreeModel = Gtk::ListStore::create(m_columns);
        m_TreeView.set_model(m_refTreeModel);

        //Add the TreeView's view columns;
        m_TreeView.append_column("   ", m_columns.m_icons);

        //Display a progress bar instread of a decimal number:
        Gtk::CellRendererProgress* cell = new Gtk::CellRendererProgress;
        int cols_count = m_TreeView.append_column(_("progress"), *cell);
        Gtk::TreeViewColumn* pColumn = m_TreeView.get_column(cols_count - 1);

        if (pColumn) {
#ifdef GLIBMM_PROPERTIES_ENABLED
                pColumn->add_attribute(cell->property_value(), m_columns.m_percent);
#else

                pColumn->add_attribute(*cell, "value", m_columns.m_percent);
#endif

                pColumn->set_fixed_width(80);
                pColumn->set_resizable();
        }

        m_TreeView.append_column(_("Filename"), m_columns.m_filename);
        m_TreeView.append_column(_("size"), m_columns.m_showsize);
        m_TreeView.append_column(_("Type"), m_columns.m_type);
        m_TreeView.append_column(_("Target"), m_columns.m_target);


        show_all();
}

FTWidget::~FTWidget()
{}

Gtk::TreeModel::iterator FTWidget::getListIter(Gtk::TreeModel::
                Children children,
                const Glib::ustring & f_sid)
{
        return find_if(children.begin(),
                       children.end(),
                       bind2nd(CompareColumns(m_columns), f_sid));
}

bool FTWidget::on_delete_event(GdkEventAny*)
{
        m_parent->on_fileXer_close(this);
}

void FTWidget::on_button_quit()
{
        m_parent->on_fileXer_close(this);
}

void FTWidget::on_button_del()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
                m_TreeView.get_selection();
	if (!selection->count_selected_rows())
			return ;
	Gtk::TreeModel::iterator iter = selection->get_selected();
	m_refTreeModel->erase(iter);
}

void FTWidget::on_button_stop()
{}

void FTWidget::on_button_continue()
{}




bool FTWidget::on_key_press_event(GdkEventKey* ev)
{
        if (ev->type != GDK_KEY_PRESS)
                return Gtk::Window::on_key_press_event(ev);

        switch (ev->keyval) {

        case GDK_Escape:
                on_button_quit();
                break;

        case GDK_Return:

        case GDK_KP_Enter:
                break;

        default:
                return Gtk::Window::on_key_press_event(ev);
        }

        //return true;
        return Gtk::Window::on_key_press_event(ev);
}


void FTWidget::addXfer(const Glib::ustring& f_sid, const std::string& f_filename, const std::string& f_target, long f_size, const std::string& f_type)
{

        Gtk::TreeModel::iterator iter = m_refTreeModel->append();
        (*iter)[m_columns.m_sid] = f_sid;
        (*iter)[m_columns.m_filename] = f_filename;
        (*iter)[m_columns.m_totalsize] = f_size;
        (*iter)[m_columns.m_showsize] = filesize_to_string(f_size);
        (*iter)[m_columns.m_size] = 0;
        (*iter)[m_columns.m_target] = f_target;
        (*iter)[m_columns.m_type] = f_type;
        (*iter)[m_columns.m_percent] = 0;
        (*iter)[m_columns.m_icons] = getPix16("ft_request.png");

}

void FTWidget::updateXfer(const Glib::ustring& f_sid, int percent)
{
        Gtk::TreeModel::Children children = m_refTreeModel->children();
        Gtk::TreeModel::iterator iter;
        iter = getListIter(children, f_sid);

        if (iter == children.end())
                return ;

        (*iter)[m_columns.m_percent] = percent;
}

void FTWidget::doneXfer(const Glib::ustring& f_sid, bool error)
{
        Gtk::TreeModel::Children children = m_refTreeModel->children();
        Gtk::TreeModel::iterator iter;
        iter = getListIter(children, f_sid);

        if (iter == children.end())
                return ;

        if (error) {
                (*iter)[m_columns.m_icons] = getPix16("ft_error.png");
                //(*iter)[m_columns.m_type] = _("error");
        } else {
                (*iter)[m_columns.m_percent] = 100;
                (*iter)[m_columns.m_icons] = getPix16("ft_done.png");
                //(*iter)[m_columns.m_type] = _("done");
        }
}

std::string FTWidget::filesize_to_string(long size)
{
        double size_display;
        std::string format;
        std::stringstream ret;

        // the comparisons to 1000 * (2^(n10)) are intentional
        // it's so you don't see something like "1023 bytes",
        // instead you'll see ".9 KB"

        if (size < 1000) {
                format = "Bytes";
                size_display = size;
        } else if (size < 1000 * 1024) {
                format = "KB";
                size_display = (double)size / 1024.0;
        } else if (size < 1000 * 1024 * 1024) {
                format = "MB";
                size_display = (double)size / (1024.0 * 1024.0);
        } else {
                format = "GB";
                size_display = (double)size / (1024.0 * 1024.0 * 1024.0);
        }

        ret << std::setprecision(1) << std::setiosflags(std::ios::fixed )
                << size_display << " " << format;

        return ret.str();
}


