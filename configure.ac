AC_INIT([cpp_parser], [0.1], [fzhedu@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_GREP
AC_PROG_NM
AC_PROG_SED
AM_PROG_AR
CPPFLAGS="-w -O2 -DTHERON_XS"
CXXFLAGS=${CXXFLAGS="-g"}
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
#all makefile
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/astnode/Makefile
  src/parser/Makefile
  src/main/Makefile
  test/Makefile
  test/parser/Makefile
  test/main/Makefile
])
AC_OUTPUT

