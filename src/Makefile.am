AM_CFLAGS = -Werror -Wall

bin_PROGRAMS = article

article_SOURCES = \
	article.c
article_CPPFLAGS = \
	$(GTK_CFLAGS) \
	-I$(top_srcdir)/include \
	-DPKGDATADIR=\"$(pkgdatadir)\"
article_LDFLAGS = --export-dynamic
article_LDADD = \
	$(GTK_LIBS) \
	$(top_builddir)/libutt/libutt.la
