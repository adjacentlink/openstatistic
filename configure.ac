%AC_COPYRIGHT([
 Copyright (c) 2016-2018,2020-2023 - Adjacent Link LLC, Bridgewater,
 New Jersey
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.
 * Neither the name of Adjacent Link LLC nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 See toplevel COPYING for more information.
])

AC_INIT([openstatistic],[1.0.10],[labs at adjacent link dot com])

AC_CONFIG_SRCDIR([src])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([])

AM_PATH_PYTHON([3.6])

: ${CXXFLAGS="-g -O3 -Wall -Wextra"}

AC_PROG_CXX

AC_PROG_CXXCPP

AC_USE_SYSTEM_EXTENSIONS

AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

AC_PROG_INSTALL

LT_INIT

AC_LANG([C++])

AC_ARG_WITH(profile,
           [  --with-profile          add profiling support (off)])

AC_ARG_WITH(debug,
           [  --with-debug            add debug support (off)])

PKG_CHECK_MODULES([protobuf],protobuf)

AC_SUBST(otestpoint_CFLAGS,
["${protobuf_CFLAGS}"])

AC_SUBST(otestpoint_LIBS,
["-lopenstatistic ${protobuf_LIBS}"])

# options for use with gprof
if test "$with_profile" = "yes"
then
CPPFLAGS="$CPPFLAGS -g -pg"
LDFLAGS="$LDFLAGS -g -pg"
fi

# options for use with debug
if test "$with_debug" = "yes"
then
CPPFLAGS="$CPPFLAGS -g -O0"
LDFLAGS="$LDFLAGS -g -O0"
fi

if test "$topdir" = ""
then
topdir="`pwd`"
fi

AC_SUBST(TOPDIR,${topdir})

LANG=C
AC_SUBST(LANG)

# check for rpmbuild
AC_CHECK_PROG(HAVE_RPMBUILD, rpmbuild, true, false)
AM_CONDITIONAL(HAVE_RPMBUILD,$HAVE_RPMBUILD)

# check for deb
AC_CHECK_PROG(HAVE_DEB, dh_clean, true, false)
AM_CONDITIONAL(HAVE_DEB,$HAVE_DEB)

AC_CHECK_FILE(/etc/debian_version,[DEBIAN_VERSION=`cat /etc/debian_version | awk -F/ '{print $1}'`],)
AC_SUBST(DEBIAN_VERSION)

AC_CHECK_FILE(/etc/lsb-release,[. /etc/lsb-release],)
AC_SUBST(DISTRIB_CODENAME)

AC_SUBST(DATE_RFC2822,`date -R`)

AC_CONFIG_FILES([Makefile
 include/Makefile
 include/ostatistic/Makefile
 src/Makefile
 src/libostatistic/Makefile
 src/example/Makefile
 src/python/Makefile
 src/libostatistic/libostatistic.pc
])
AC_OUTPUT
