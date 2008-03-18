/*
* =====================================================================================
*
*       Filename:  BuddyInfoWindow.h
*
*    Description:  用户信息面板
*
*        Version:  1.0
*        Created:  2007年09月27日 20时12分38秒 CST
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  lerosua (), lerosua@gmail.com
*        Company:  Cyclone
*
* =====================================================================================
*/

#ifndef  BUDDYINFOWINDOW_FILE_HEADER_INC
#define  BUDDYINFOWINDOW_FILE_HEADER_INC

#include <gtkmm.h>
#include <libglademm/xml.h>
#include "MainWindow.h"
#include "Buddy.h"

class BuddyInfoWindow: public Gtk::Window
{

public:
        BuddyInfoWindow(Buddy* buddy_);
        ~BuddyInfoWindow();

private:
        bool on_key_press_event(GdkEventKey* ev);
        void on_btclose_clicked();

private:
        Buddy* buddy;
        MsgBox *infoBox;
};


#endif   /* ----- #ifndef BUDDYINFOWINDOW_FILE_HEADER_INC  ----- */

