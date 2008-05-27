if WIN32
MWINDOWS = -mwindows
endif

bin_PROGRAMS = tulip

tulip_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT -DQT_ASSISTANT='"@QT_ASSISTANT@"'

INCLUDES = -I$(srcdir)/../include -I../include \
	   @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_QT_INCLUDE@/tulip @TULIP_PLUGINSMANAGER_INCLUDE@


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

BUILT_SOURCES = \
	TulipResource.cpp \
	moc_CopyPropertyDialog.cpp \
	moc_AppStartUp.cpp \
	moc_InfoDialog.cpp \
	moc_viewGl.cpp \
	moc_PropertyDialog.cpp \
	moc_QtProgress.cpp \
	moc_TulipStatsWidget.cpp \
	moc_GridOptionsWidget.cpp

nodist_tulip_SOURCES = $(BUILT_SOURCES)

dist_tulip_SOURCES = \
	viewGl.cpp \
	Application.cpp \
	geometrie2D.cpp \
	InfoDialog.cpp \
	AppStartUp.cpp \
	PropertyDialog.cpp \
	CopyPropertyDialog.cpp \
	QtProgress.cpp \
	TulipStatsWidget.cpp \
	GridOptionsWidget.cpp \
	main.cpp

moc_%.cpp: ../include/%.h
	$(MOC) ${AM_CPPFLAGS} -o $@ $<

CLEANFILES = $(BUILT_SOURCES)

../include/%Data.h: ../designer/%.ui
	$(UIC) -o $@ $<

TulipResource.cpp: ../designer/TulipResource.qrc
	$(RCC) -o $@ $<

if MACOSX
install-data-am :
	if test ! -d  $(exec_prefix)/bin/tulip.app/Contents/assistant.app; then \
		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/ ;\
		ln -s @QTDIR@/bin/@QT_ASSISTANT@.app $(exec_prefix)/bin/tulip.app/Contents/@QT_ASSISTANT@.app ;\
	fi;
endif
