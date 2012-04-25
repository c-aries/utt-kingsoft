AM_CFLAGS = -Werror -Wall

bin_PROGRAMS = utt

utt_SOURCES = \
	main.c main.h \
	englishui.c englishui.h \
	data.c data.h \
	process.c process.h \
	global.c global.h \
	stat.c stat.h \
	commonui.c commonui.h \
	class.c class.h \
	strategy.c strategy.h \
	class_random.c class_random.h

utt_CPPFLAGS = \
	$(GTK_CFLAGS) \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	-DDATADIR=\"$(datadir)/$(PACKAGE)\"

utt_LDADD = \
	$(GTK_LIBS)
