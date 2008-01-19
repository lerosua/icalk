SUBDIRS = images sounds ui smiley

desktopdir       = $(datadir)/applications
desktop_in_files = icalk.desktop.in
desktop_DATA     = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

CLEANFILES = $(desktop_DATA)
