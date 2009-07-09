
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


########### install files ###############




#original Makefile.am contents follow:

#if !MACOSX
#DLL_FLAGS = -no-undefined
#else
#DLL_FLAGS = -Wl,-headerpad_max_install_names
#endif
#
#plugins_LTLIBRARIES = 
#
#pluginsdir = $(libdir)/tlp
#
#AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@
#INCLUDES = @TULIP_INCLUDE@
