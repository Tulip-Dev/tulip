if ! WIN32
if ! MACOSX
ADD_LIB = -lXext -lXi -lpthread
endif
else
MWINDOWS = -mwindows
endif

bin_PROGRAMS = tulip

tulip_CXXFLAGS = -DQT_THREAD_SUPPORT -D_REENTRANT

INCLUDES = -I$(srcdir)/../include -I../include \
	   @GL_INCLUDES@ @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @TULIP_QT_INCLUDE@/tulip

if MACOSX
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -framework Carbon -framework AGL
else
AM_LDFLAGS = ${MWINDOWS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@
endif
AM_CPPFLAGS= @QT_CPPFLAGS@

tulip_LDFLAGS = -L.
if QT3
tulip_LDADD = -lfreetype -lxml2 ${ADD_LIB} @QT_LDFLAGS@ -lqassistantclient @LIB_QT@ @LIB_IMAGES_LDFLAGS@
else
tulip_LDADD = -lfreetype -lxml2 ${ADD_LIB} @QT_LDFLAGS@ -lQtAssistantClient @LIB_QT@ @LIB_IMAGES_LDFLAGS@
endif

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
	OverviewData.cpp\
	moc_OverviewData.cpp\
	TabWidgetData.cpp \
	moc_TabWidgetData.cpp \
	InfoDialogData.cpp \
	AppStartUpData.cpp \
	ToolBarData.cpp \
	PropertyDialogData.cpp \
	TulipStatsData.cpp \
	GridOptionsData.cpp \
	moc_TulipStatsData.cpp \
	moc_GridOptionsData.cpp \
	moc_Overview.cpp \
	moc_AppStartUp.cpp \
	moc_AppStartUpData.cpp \
	moc_InfoDialogData.cpp \
	moc_InfoDialog.cpp \
	moc_viewGl.cpp \
	moc_ToolBar.cpp \
	moc_ToolBarData.cpp \
	moc_PropertyDialog.cpp \
	moc_PropertyDialogData.cpp \
	moc_NavigateButton.cpp \
	moc_QtProgress.cpp \
	moc_TulipStatsWidget.cpp \
	moc_GridOptionsWidget.cpp

nodist_tulip_SOURCES =  \
	QtProgressData.cpp \
	moc_QtProgressData.cpp \
	moc_Overview.cpp \
	TulipData.cpp\
	moc_TulipData.cpp\
	OverviewData.cpp\
	moc_OverviewData.cpp\
	TabWidgetData.cpp \
	moc_TabWidgetData.cpp \
	InfoDialogData.cpp \
	AppStartUpData.cpp \
	ToolBarData.cpp \
	PropertyDialogData.cpp \
	TulipStatsData.cpp \
	GridOptionsData.cpp \
	moc_TulipStatsData.cpp \
	moc_GridOptionsData.cpp \
	moc_AppStartUp.cpp \
	moc_AppStartUpData.cpp \
	moc_InfoDialogData.cpp \
	moc_InfoDialog.cpp \
	moc_viewGl.cpp \
	moc_ToolBar.cpp \
	moc_ToolBarData.cpp \
	moc_PropertyDialog.cpp \
	moc_PropertyDialogData.cpp \
	moc_NavigateButton.cpp \
	moc_QtProgress.cpp \
	moc_TulipStatsWidget.cpp \
	moc_GridOptionsWidget.cpp


if QT3
dist_tulip_SOURCES = \
	Overview.cpp \
	ElementInfoToolTip.cpp \
	viewGl.cpp \
	Application.cpp \
	geometrie2D.cpp \
	NavigateButton.cpp \
	InfoDialog.cpp \
	AppStartUp.cpp \
	ToolBar.cpp \
	PropertyDialog.cpp \
	QtProgress.cpp \
	TulipStatsWidget.cpp \
	GridOptionsWidget.cpp \
	main.cpp

moc_%.cpp: ../include/%.h 
	$(MOC) -o $@ $<

else
dist_tulip_SOURCES = \
	Overview.cpp \
	viewGl.cpp \
	Application.cpp \
	geometrie2D.cpp \
	NavigateButton.cpp \
	InfoDialog.cpp \
	AppStartUp.cpp \
	ToolBar.cpp \
	PropertyDialog.cpp \
	QtProgress.cpp \
	TulipStatsWidget.cpp \
	GridOptionsWidget.cpp \
	main.cpp

moc_%.cpp: ../include/%.h 
	$(MOC) ${AM_CPPFLAGS} -o $@ $<

endif


CLEANFILES = $(BUILT_SOURCES)

if !QT3
moc_PropertyDialog.cpp: ../include/PropertyDialogQt3.h 
	$(MOC) ${AM_CPPFLAGS} -o $@ $<
endif

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
