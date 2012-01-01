SUBDIRS = icons logo

desktopdir = $(datadir)/applications
desktop_in_files = utt.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

pkgdata_DATA = \
	article-ui.xml

EXTRA_DIST = \
	$(pkgdata_DATA) \
	$(desktop_in_files)

CLEANFILES = \
	$(desktop_DATA)

DISTCLEANFILES = \
	$(desktop_DATA)
