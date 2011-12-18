#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([utt], [0.1], [babyaries2@gmail.com])
AC_CONFIG_SRCDIR([src/artical.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# gettext
IT_PROG_INTLTOOL([0.40.6])
GETTEXT_PACKAGE=${PACKAGE_NAME}
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
				    [The prefix for our gettext translation domains.])

# libtool
AC_PROG_LIBTOOL
LT_PREREQ([2.2.10])
LT_INIT()

# Checks for libraries.
# Debian 6.0
GTK_MODULES="gtk+-2.0 >= 2.20.1 glib-2.0 >= 2.24.2"
PKG_CHECK_MODULES(GTK,$GTK_MODULES)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		po/Makefile.in
		src/Makefile])

AC_OUTPUT
