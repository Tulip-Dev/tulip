if WIN32
MWINDOWS = -mwindows
else
ADD_INCLUDES = `xml2-config --cflags` 
endif

bin_PROGRAMS = tulip

tulip_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT

INCLUDES = -I$(srcdir)/../include -I../include $(ADD_INCLUDES)\
	   @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_QT_INCLUDE@/tulip


if MACOSX_7
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ `ls $(top_builddir)/library/tulip/src/*.o` `ls $(top_builddir)/thirdparty/gzstream/*.o` `ls $(top_builddir)/thirdparty/triangle/*.o` -lz  @TULIP_OGL_LDFLAGS@ `ls $(top_builddir)/library/tulip-qt/src/*.o` -framework Carbon -framework AGL
else
if MACOSX
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -framework Carbon -framework AGL -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
else
AM_LDFLAGS = ${MWINDOWS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -lfreetype -lxml2 ${ADD_LIB} @LIB_QT_ASSISTANT@ @LIB_QT@ @LIB_IMAGES_LDFLAGS@
endif
endif
AM_CPPFLAGS= @QT_CPPFLAGS@

if !WIN32
UIC_MAKE = LD_LIBRARY_PATH=$(top_builddir)/library/tulip/src/.libs/:$(top_builddir)/library/tulip-ogl/src/.libs/:$(top_builddir)/library/tulip-qt/src/.libs/:$$LD_LIBRARY_PATH @UIC@ -L $(top_builddir)/library/tulip-qt/src/.libs/
else
UIC_MAKE = PATH=$(top_builddir)/library/tulip/src/.libs/:$(top_builddir)/library/tulip-ogl/src/.libs/:$(top_builddir)/library/tulip-qt/src/.libs/:$$PATH @UIC@ -L $(top_builddir)/library/tulip-qt/src/.libs/
endif

MOC = @MOC@

BUILT_SOURCES = \
	QtProgressData.cpp \
	moc_QtProgressData.cpp \
	TulipData.cpp\
	moc_TulipData.cpp\
	TabWidgetData.cpp \
	moc_TabWidgetData.cpp \
	InfoDialogData.cpp \
	AppStartUpData.cpp \
	PropertyDialogData.cpp \
	TulipStatsData.cpp \
	GridOptionsData.cpp \
	CopyPropertyDialogData.cpp \
	moc_TulipStatsData.cpp \
	moc_GridOptionsData.cpp \
	moc_CopyPropertyDialog.cpp \
	moc_CopyPropertyDialogData.cpp \
	moc_AppStartUp.cpp \
	moc_AppStartUpData.cpp \
	moc_InfoDialogData.cpp \
	moc_InfoDialog.cpp \
	moc_viewGl.cpp \
	moc_PropertyDialog.cpp \
	moc_PropertyDialogData.cpp \
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

%Data.cpp: ../designer/%.ui ../include/%Data.h
	$(UIC_MAKE) -o $@ -impl ../include/$*Data.h $<

../include/%Data.h: ../designer/%.ui
	$(UIC_MAKE) -o $@ $<

if MACOSX
install-data-am :
	if test ! -d  $(exec_prefix)/bin/tulip.app/Contents/assistant.app; then \
		mkdir -p $(exec_prefix)/bin/tulip.app/Contents/ ;\
		ln -s @QTDIR@/bin/assistant.app $(exec_prefix)/bin/tulip.app/Contents/assistant.app ;\
	fi;
endif
