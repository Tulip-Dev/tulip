if WIN32
MWINDOWS = -mwindows
ADDLDFLAGS = --lxml2 
else
ADDLDFLAGS = `xml2-config --libs`
ADD_INCLUDES = `xml2-config --cflags` 
endif

bin_PROGRAMS = serverGenerator pluginInstaller

tulip_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT

INCLUDES = -I$(srcdir)/../include -I../include \
	   @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_QT_INCLUDE@/tulip @TULIP_PLUGINSMANAGER_INCLUDE@ ${ADD_INCLUDES}


if MACOSX_7
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ `ls $(top_builddir)/library/tulip/src/*.o` `ls $(top_builddir)/thirdparty/gzstream/*.o` `ls $(top_builddir)/thirdparty/triangle/*.o` -lz  @TULIP_OGL_LDFLAGS@ `ls $(top_builddir)/library/tulip-qt/src/*.o` -framework Carbon -framework AGL
else
if MACOSX
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -framework Carbon -framework AGL -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
else
AM_LDFLAGS = ${MWINDOWS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @TULIP_PLUGINSMANAGER_LDFLAGS@ -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
endif
endif
AM_CPPFLAGS= @QT_CPPFLAGS@

if !WIN32
UIC_MAKE = LD_LIBRARY_PATH=$(top_builddir)/library/tulip/src/.libs/:$(top_builddir)/library/tulip-ogl/src/.libs/:$(top_builddir)/library/tulip-qt/src/.libs/:$$LD_LIBRARY_PATH @UIC@ -L $(top_builddir)/library/tulip-qt/src/.libs/
else
UIC_MAKE = PATH=$(top_builddir)/library/tulip/src/.libs/:$(top_builddir)/library/tulip-ogl/src/.libs/:$(top_builddir)/library/tulip-qt/src/.libs/:$$PATH @UIC@ -L $(top_builddir)/library/tulip-qt/src/.libs/
endif

MOC = @MOC@

dist_serverGenerator_SOURCES = \
	PluginGenerate.cpp \
	ServerGenerator.cpp

dist_pluginInstaller_SOURCES = \
	PluginGenerate.cpp \
	PluginInstaller.cpp

moc_%.cpp: ../include/%.h
	$(MOC) ${AM_CPPFLAGS} -o $@ $<
