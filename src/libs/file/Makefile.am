yapet_srcdir = $(top_srcdir)/src
yapet_src_libdir = $(yapet_srcdir)/libs
yapet_builddir = $(top_builddir)/src
yapet_libs_builddir = $(yapet_builddir)/libs

AM_CPPFLAGS = -I$(yapet_src_libdir)/pwgen \
    -I$(yapet_src_libdir)/consts \
    -I$(yapet_src_libdir)/metadata \
    -I$(yapet_src_libdir)/exceptions \
    -I$(yapet_src_libdir)/utils \
    -I$(yapet_src_libdir)/interfaces \
    -I$(top_srcdir) \
    -DLOCALEDIR=\"$(localedir)\"

if DISABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
else
AM_CPPFLAGS += -DDEBUG
endif

noinst_LTLIBRARIES = libyapet-file.la
libyapet_file_la_SOURCES = rawfile.cc rawfile.hh \
fileutils.cc fileutils.hh yapetfile.hh \
yapetfile.cc yapet10file.hh yapet10file.cc yapet20file.hh yapet20file.cc header10.cc header10.hh \
headerversion.hh filehelper.hh filehelper.cc
