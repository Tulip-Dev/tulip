if WIN32 
DLL_FLAGS = -no-undefined
DLL_TULIP_QT = -DDLL_TULIP_QT
DLL_TULIP_QT2 = -DDLL_TULIP_QT2
RELEASE = ${VERSION}
else 
RELEASE = @TULIP_VERSION@ -version-info 0:0:0
endif

lib_LTLIBRARIES = libtulip-qt.la
if QT3
qtplugin_LTLIBRARIES = libTulipWidgets.la
else
qtplugin_LTLIBRARIES = libTulipPropertyTablePlugin.la libTulipElementPropertiesPlugin.la libGlGraphWidgetPlugin.la libClusterTreePlugin.la
endif

INCLUDES = ${DLL_TULIP_QT} @GL_INCLUDES@ @TULIP_QT_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
if MACOSX_7
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
else
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
endif

AM_CPPFLAGS= @QT_CPPFLAGS@
qtplugindir = $(libdir)/tlp/plugins/designer

if QT3
libTulipWidgets_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libTulipWidgets_laINCLUDES = ${DLL_TULIP_QT2}

libTulipWidgets_la_SOURCES = \
	TulipWidgetsPlugin.cpp

EXTRA_DIST = ClusterTreePlugin.cpp GlGraphWidgetPlugin.cpp\
	     TulipElementPropertiesPlugin.cpp TulipPropertyTablePlugin.cpp
else
libTulipPropertyTablePlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libTulipElementPropertiesPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libGlGraphWidgetPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libClusterTreePlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version

libTulipPropertyTablePlugin_laINCLUDES = ${DLL_TULIP_QT2}
libTulipElementPropertiesPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libGlGraphWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libClusterTreePlugin_laINCLUDES = ${DLL_TULIP_QT2}

libClusterTreePlugin_la_SOURCES = ClusterTreePlugin.cpp
nodist_libClusterTreePlugin_la_SOURCES = moc_ClusterTreePlugin.cpp
libGlGraphWidgetPlugin_la_SOURCES = GlGraphWidgetPlugin.cpp
nodist_libGlGraphWidgetPlugin_la_SOURCES = moc_GlGraphWidgetPlugin.cpp 
libTulipElementPropertiesPlugin_la_SOURCES = TulipElementPropertiesPlugin.cpp
nodist_libTulipElementPropertiesPlugin_la_SOURCES = moc_TulipElementPropertiesPlugin.cpp
libTulipPropertyTablePlugin_la_SOURCES = TulipPropertyTablePlugin.cpp
nodist_libTulipPropertyTablePlugin_la_SOURCES = moc_TulipPropertyTablePlugin.cpp

EXTRA_DIST = TulipWidgetsPlugin.cpp
endif

libtulip_qt_la_LDFLAGS = -release ${RELEASE}  


dist_libtulip_qt_la_SOURCES = \
	TulipTableWidget.cpp \
	GlGraphWidget.cpp \
	PropertyWidgets.cpp \
	TulipElementPropertiesData.cpp \
	TulipElementProperties.cpp \
	FindSelection.cpp \
	MouseObject.cpp \
	MouseZoomBox.cpp \
	ClusterTreeData.cpp \
	Morphing.cpp \
	ClusterTree.cpp \
	GraphState.cpp \
	MouseMoveSelection.cpp \
	MouseAddNode.cpp \
	MouseAddEdge.cpp \
	MouseSelection.cpp \
	MouseMagicSelection.cpp \
	DataSetDialog.cpp

CLEANFILES = $(BUILT_SOURCES)

if QT3
nodist_libtulip_qt_la_SOURCES = \
	moc_TulipTableWidget.cpp \
	moc_GlGraphWidget.cpp \
	moc_PropertyWidgets.cpp \
	moc_TulipElementPropertiesData.cpp \
	moc_TulipElementProperties.cpp \
	moc_FindSelection.cpp \
	moc_FindSelectionData.cpp \
	moc_ClusterTreeData.cpp \
	FindSelectionData.cpp \
	moc_ClusterTree.cpp

moc_%.cpp: $(srcdir)/../include/tulip/%.h 
	$(MOC) -o $@ $<
else
nodist_libtulip_qt_la_SOURCES = \
	moc_TulipTableWidget.cpp \
	moc_GlGraphWidget.cpp \
	moc_PropertyWidgets.cpp \
	moc_TulipElementPropertiesData.cpp \
	moc_TulipElementProperties.cpp \
	moc_FindSelection.cpp \
	moc_FindSelectionData.cpp \
	moc_ClusterTreeData.cpp \
	FindSelectionData.cpp \
	moc_ClusterTree.cpp \
	$(srcdir)/../include/tulip/clustertree.h \
	$(srcdir)/../include/tulip/glgraphwidget.h \
	$(srcdir)/../include/tulip/tulipelementproperties.h \
	$(srcdir)/../include/tulip/tulippropertytable.h

moc_%.cpp: $(srcdir)/../include/tulip/%.h 
	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<

moc_ClusterTree.cpp: $(srcdir)/../include/tulip/ClusterTreeQt3.h 
	$(MOC) ${AM_CPPFLAGS} -o $@ $<

../include/tulip/clustertree.h : $(srcdir)/../include/tulip/ClusterTree.h
	cp -f $< $@

../include/tulip/glgraphwidget.h : $(srcdir)/../include/tulip/GlGraphWidget.h
	cp -f $< $@

../include/tulip/tulipelementproperties.h : $(srcdir)/../include/tulip/TulipElementProperties.h
	cp -f $< $@

../include/tulip/tulippropertytable.h : $(srcdir)/../include/tulip/PropertyWidgets.h
	cp -f $< $@
endif


FindSelectionData.cpp: $(srcdir)/../designer/FindSelection.ui $(srcdir)/../include/tulip/FindSelectionData.h
	$(UIC) -o $@ -impl $(srcdir)/../include/tulip/FindSelectionData.h $<

../include/tulip/FindSelectionData.h: $(srcdir)/../designer/FindSelection.ui
	$(UIC) -o $@ $<
