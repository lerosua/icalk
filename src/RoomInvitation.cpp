/*
* =====================================================================================
*
*       Filename:  RoomInvitation.cpp
*
*    Description:  
*
*        Version:  1.0
*        Created:  2008年04月25日 09时59分15秒
*       Revision:  none
*       Compiler:  gcc
*
*         Author:  first_name last_name (fl), fl@my-company.com
*        Company:  my-company
*
* =====================================================================================
*/

#include <glib/gi18n.h>
#include <gtkmm/messagedialog.h>
#include "RoomInvitation.h"
#include "Bodies.h"
#include "TalkRoomHandler.h"
#include "RoomItem.h"
#include "MsgPage.h"

RoomInvitation::RoomInvitation(Client* f_client):MUCInvitationHandler(f_client)
{}
//RoomInvitation::RoomInvitation(Client* f_client)
//{}

RoomInvitation::~RoomInvitation()
{}

void RoomInvitation::handleMUCInvitation(const JID& room, const JID& from
                , const std::string& reason, const std::string& body, const std::string & password, bool cont)
{
}
void RoomInvitation::handleMUCInvitation(const JID& room, const JID& from
                , const std::string& reason, const std::string& body, const std::string & password, bool cont,const std::string& thread)
{

        Gtk::MessageDialog dialog( _("Room Invitation"), false,
                                   Gtk::MESSAGE_QUESTION,
                                   Gtk::BUTTONS_OK_CANCEL);

        Glib::ustring msg =
                from.bare() + _(" have send a invitation from ") + room.bare()
                + _("\nMessage: ") + reason + _("\nDo you want to join the room?");

        dialog.set_secondary_text(msg);

        int result = dialog.run();
        dialog.show();

        switch (result) {
        case (Gtk::RESPONSE_OK): {
                        ConferenceListItem ci;

                        Bodies& bodies = Bodies::Get_Bodies();
                        ci.jid = room.bare();
                        ci.nick = bodies.get_jid().username();
                        ci.name = bodies.get_jid().username();
                        ci.password = password;
                        RoomItem *item = new RoomItem(ci);

                        bodies.getRoomHandler().addRoomItem(ci.jid,
                                                            item);
                        item->join();

                        MsgPage *page = item->getPage();

                        if (NULL == page) {
                                const std::string label =
                                        item->getRoomJID();
                                MsgPage *page_ =
                                        new MsgPage(label, item, 1);
                                item->setPage(page_);
                                page = item->getPage();
                                page->setSubject();
                                page->refreshMember();
                                bodies.get_msg_window().
                                add_page(*page);
                        }

                        bodies.get_msg_window().show();
                        bodies.get_msg_window().setCurrentPage(page);


                        break;

                }

        case (Gtk::RESPONSE_CANCEL):
                                        break;

        default:
                break;
        }




}
