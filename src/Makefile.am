INCLUDES = 								\
	   -I$(top_srcdir)                                              \
	   -I$(srcdir)                                                  \
	   -g								\
	   -DDATA_DIR=\""${datadir}/icalk"\" 				\
	   -DITALK_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"	\
	   @PACKAGE_CFLAGS@

bin_PROGRAMS = icalk
icalk_SOURCES = \
	       Bodies.cpp \
	       Buddy.cpp \
	       BuddyList.cpp \
	       BuddyView.cpp \
	       MainWindow.cpp \
	       MsgWindow.cpp \
	       MsgPage.cpp \
	       MsgLog.cpp  \
	       TalkMsg.cpp \
	       TalkConnect.cpp \
	       TalkRoomHandler.cpp  \
	       RoomItem.cpp 	\
	       MemberList.cpp 	\
	       TalkCard.cpp   \
	       TagList.cpp   	\
	       TalkFT.cpp 	\
	       MsgBox.cpp 	\
	       TreeModelDnd.cpp \
	       CommandBar.cpp 	\
	       TrayIcon.cpp   \
	       StatusMsgWidget.cpp \
	       BuddyInfoWindow.cpp \
	       TreeViewTooltips.cpp \
	       ServerDiscoWindow.cpp \
	       TalkDiscoHandler.cpp \
	       SmileyWnd.cpp	\
	       Unit.cpp 	\
	       ConfXml.cpp 	\
	       sounds.cpp 	\
	       about.cpp 	\
	       pixmaps.cpp

noinst_HEADERS = \
		 Bodies.h \
		 Buddy.h \
		 BuddyList.h \
		 BuddyView.h \
		 MainWindow.h \
		 MsgWindow.h \
		 MsgPage.h \
		 MsgLog.h  \
		 TalkMsg.h \
		 TalkConnect.h \
		 TalkRoomHandler.h    \
		 TalkDiscoHandler.h \
		 RoomItem.h 	\
		 MemberList.h 	\
		 TagList.h     \
		 TalkFT.h 	\
	         TalkCard.h 	\
		 MsgBox.h  	\
		 TreeModelDnd.h \
		 CommandBar.h 	\
	         TrayIcon.h   \
		 StatusMsgWidget.h \
		 BuddyInfoWindow.h \
		 TreeViewTooltips.h \
	         ServerDiscoWindow.h \
		 ConfXml.h	\
		 SmileTree.h 	\
		 SmileyWnd.h	\
		 TreeItem.h 	\
		 XPThread.h 	\
	         Unit.h 	\
		 sounds.h 	\
		 about.h 	\
		 icalk.h 	\
	         pixmaps.h
icalk_LDADD = @LIBS@ @PACKAGE_LIBS@ @SEXYMM_LIBS@
