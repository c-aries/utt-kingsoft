AM_CFLAGS = -Werror -Wall

libutt = $(top_builddir)/libutt/libutt.la

gladeutt_LTLIBRARIES = libgladeutt.la
gladeuttdir = $(GLADE_MODULEDIR)

libgladeutt_la_SOURCES = \
	glade-utt.c
libgladeutt_la_CPPFLAGS = \
	$(GLADE_CFLAGS) -I$(top_srcdir)/include
libgladeutt_la_LIBADD = \
	$(GLADE_LIBS) \
	$(libutt)

catalogsdir = $(GLADE_CATALOGDIR)
catalogs_DATA = utt.xml utt.xml.in

@INTLTOOL_XML_NOMERGE_RULE@

CLEANFILES = utt.xml

EXTRA_DIST = utt.xml utt.xml.in
