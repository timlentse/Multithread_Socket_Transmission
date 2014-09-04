#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([SERVER_SOCKET], [2.0], [tinglenxan@gmail.com])
AC_CONFIG_SRCDIR([server.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(SERVER_SOCKET, 2.0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([bzero inet_ntoa socket strerror])

AC_OUTPUT(Makefile)
