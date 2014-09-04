#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(client.cpp)
AC_CONFIG_SRCDIR([client.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(client,1.0)

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([bzero gethostbyname socket strerror])

AC_OUTPUT(Makefile)
