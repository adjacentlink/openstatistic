noinst_PROGRAMS = example

example_CPPFLAGS = \
 -I@top_srcdir@/include \
 $(AM_CPPFLAGS) \
 $(openstatisic_CFLAGS)

example_LDADD = \
 @top_srcdir@/src/libostatistic/.libs/libostatistic.la

example_SOURCES =  \
 main.cc

