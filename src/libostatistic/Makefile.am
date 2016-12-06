lib_LTLIBRARIES = libostatistic.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libostatistic.pc

libostatistic_la_CPPFLAGS= \
 -I@top_srcdir@/include \
 $(protobuf_CFLAGS)

nodist_libostatistic_la_SOURCES = \
 openstatisticapi.pb.cc \
 openstatisticapi.pb.h

BUILT_SOURCES = \
 $(nodist_libostatistic_la_SOURCES)

libostatistic_la_SOURCES = \
 any.cc \
 anyutils.h \
 clearupdatehandler.cc \
 clearupdatehandler.h \
 errorresponse.cc \
 errorresponse.h \
 inetaddr.cc \
 inetaddr.h \
 parameterconvert.h \
 parameterconvert.inl \
 queryhandler.cc \
 queryhandler.h \
 registrarimpl.cc \
 registrarimpl.h \
 requestmessagehandler.cc \
 requestmessagehandler.h \
 service.cc \
 session.cc \
 session.h \
 socket.cc \
 socket.h \
 statisticinfo.h \
 statisticinfo.inl \
 tableclearupdatehandler.cc \
 tableclearupdatehandler.h \
 tableinfo.h \
 tableinfo.inl \
 tablequeryhandler.cc \
 tablequeryhandler.h

EXTRA_DIST = \
 openstatisticapi.proto

libostatistic_la_LIBADD= \
 $(protobuf_LIBS)        

libostatistic_la_LDFLAGS= \
 -avoid-version

clean-local:
	rm -f $(BUILT_SOURCES)

openstatisticapi.pb.cc openstatisticapi.pb.h: openstatisticapi.proto
	protoc -I=. --cpp_out=. $<
