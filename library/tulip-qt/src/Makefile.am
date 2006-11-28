if WIN32 
DLL_FLAGS = -no-undefined
DLL_TULIP_QT = -DDLL_TULIP_QT
DLL_TULIP_QT2 = -DDLL_TULIP_QT2
RELEASE = ${VERSION}
else 
RELEASE = @TULIP_VERSION@ -version-info 0:0:0
if MACOSX
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif
endif

lib_LTLIBRARIES = libtulip-qt@QT_VERSION@.la
if QT3
qtplugin_LTLIBRARIES = libTulipWidgets.la
else
qtplugin_LTLIBRARIES = libPropertyWidgetPlugin.la libElementPropertiesWidgetPlugin.la libGlGraphWidgetPlugin.la libSGHierarchyWidgetPlugin.la libGWOverviewWidgetPlugin.la
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

EXTRA_DIST = SGHierarchyWidgetPlugin.cpp GlGraphWidgetPlugin.cpp\
	     ElementPropertiesWidgetPlugin.cpp PropertyWidgetPlugin.cpp GWOverviewWidgetPlugin.cpp
else
libPropertyWidgetPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libElementPropertiesWidgetPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libGlGraphWidgetPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libGWOverviewWidgetPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libSGHierarchyWidgetPlugin_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version

libPropertyWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libElementPropertiesWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libGlGraphWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libGWOverviewWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libSGHierarchyWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}

libSGHierarchyWidgetPlugin_la_SOURCES = SGHierarchyWidgetPlugin.cpp
nodist_libSGHierarchyWidgetPlugin_la_SOURCES = moc_SGHierarchyWidgetPlugin.cpp
libGlGraphWidgetPlugin_la_SOURCES = GlGraphWidgetPlugin.cpp
nodist_libGlGraphWidgetPlugin_la_SOURCES = moc_GlGraphWidgetPlugin.cpp 
libGWOverviewWidgetPlugin_la_SOURCES = GWOverviewWidgetPlugin.cpp
nodist_libGWOverviewWidgetPlugin_la_SOURCES = moc_GWOverviewWidgetPlugin.cpp 
libElementPropertiesWidgetPlugin_la_SOURCES = ElementPropertiesWidgetPlugin.cpp
nodist_libElementPropertiesWidgetPlugin_la_SOURCES = moc_ElementPropertiesWidgetPlugin.cpp
libPropertyWidgetPlugin_la_SOURCES = PropertyWidgetPlugin.cpp
nodist_libPropertyWidgetPlugin_la_SOURCES = moc_PropertyWidgetPlugin.cpp

EXTRA_DIST = TulipWidgetsPlugin.cpp
endif

libtulip_qt@QT_VERSION@_la_LDFLAGS = -release ${RELEASE}  


dist_libtulip_qt@QT_VERSION@_la_SOURCES = \
	TulipTableWidget.cpp \
	GlGraphWidget.cpp \
	PropertyWidget.cpp \
	ElementPropertiesWidgetData.cpp \
	ElementPropertiesWidget.cpp \
	FindSelection.cpp \
	SGHierarchyWidgetData.cpp \
	SGHierarchyWidget.cpp \
	GWOverviewWidget.cpp \
	Morphing.cpp \
	GraphState.cpp \
	MouseBoxZoomer.cpp \
	MouseEdgeBuilder.cpp \
	MouseInteractors.cpp \
	MouseMagicSelector.cpp \
	MouseNodeBuilder.cpp \
	MouseSelectionEditor.cpp \
	MouseSelector.cpp \
	DataSetDialog.cpp

CLEANFILES = $(BUILT_SOURCES)

nodist_libtulip_qt@QT_VERSION@_la_SOURCES = \
	moc_TulipTableWidget.cpp \
	moc_GlGraphWidget.cpp \
	moc_PropertyWidget.cpp \
	moc_ElementPropertiesWidgetData.cpp \
	moc_ElementPropertiesWidget.cpp \
	moc_FindSelection.cpp \
	moc_FindSelectionData.cpp \
	moc_SGHierarchyWidgetData.cpp \
	FindSelectionData.cpp \
	GWOverviewWidgetData.cpp \
	moc_SGHierarchyWidget.cpp \
	moc_GWOverviewWidget.cpp \
	moc_GWOverviewWidgetData.cpp

if QT3
moc_%.cpp: $(srcdir)/../include/tulip/%.h 
	$(MOC) -o $@ $<
moc_%.cpp: ../include/tulip/%.h
	$(MOC) -o $@ $<
else
moc_%.cpp: $(srcdir)/../include/tulip/%.h 
	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<
moc_%.cpp: ../include/tulip/%.h
	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<

moc_SGHierarchyWidget.cpp: $(srcdir)/../include/tulip/SGHierarchyWidgetQt3.h 
	$(MOC) ${AM_CPPFLAGS} -o $@ $<
endif

FindSelectionData.cpp: $(srcdir)/../designer/FindSelection.ui ../include/tulip/FindSelectionData.h
	$(UIC) -o $@ -impl ../include/tulip/FindSelectionData.h $<

GWOverviewWidgetData.cpp: $(srcdir)/../designer/GWOverviewWidget.ui ../include/tulip/GWOverviewWidgetData.h
	$(UIC) -o $@ -impl ../include/tulip/GWOverviewWidgetData.h $<
