AM_CFLAGS = -Werror -Wall

bin_PROGRAMS = utt

utt_SOURCES = \
	main.c

utt_CPPFLAGS = \
	$(GTK_CFLAGS) \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	-DDATADIR=\"$(datadir)/$(PACKAGE)\"

utt_LDADD = \
	$(GTK_LIBS)
