// generated 2005/8/8 17:10:19 CST by xihels@163.com
//


#ifndef _COMMANDBAR_HH
#  define _COMMANDBAR_HH
#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include "SmileyWnd.h"

class MsgPage;
/**
 * @brief 聊天窗口中的工具栏
 */
class CommandBar : public Gtk::HBox
{  
    public:
	CommandBar(MsgPage* _parent); 
	~CommandBar();
    private:
	MsgPage* parent;

	void on_fonts_clicked();
	void on_link_clicked();
	void on_smiley_clicked();
	void on_image_clicked();
    private:
	SmileyWnd* smileyWnd;
	Gtk::Button * buttonFonts;
	Gtk::Button * buttonLink;
	Gtk::Button * buttonSmiley;
	Gtk::Button * buttonImage;
};

#endif
