AM_CFLAGS = -Werror -Wall

lib_LTLIBRARIES = libutt.la

libutt_la_SOURCES = \
	uttarticle.c
libutt_la_CFLAGS = \
	$(GTK_CFLAGS) -I$(top_srcdir)/include
libutt_la_LDFLAGS = --export-dynamic
libutt_la_LIBADD = \
	$(GTK_LIBS)
