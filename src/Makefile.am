AM_CFLAGS = -Werror -Wall

bin_PROGRAMS = utt

utt_SOURCES = \
	main.c \
	-DPKGDATADIR=\"$(pkgdatadir)\"

utt_CPPFLAGS = \
	$(GTK_CFLAGS)

utt_LDADD = \
	$(GTK_LIBS)
