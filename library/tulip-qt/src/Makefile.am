RELEASE = ${VERSION}

if WIN32 
DLL_FLAGS = -no-undefined
DLL_TULIP_QT = -DDLL_TULIP_QT
DLL_TULIP_QT2 = -DDLL_TULIP_QT2
else 
if MACOSX
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif
endif

lib_LTLIBRARIES = libtulip-qt4.la
qtplugin_LTLIBRARIES = libPropertyWidgetPlugin.la libElementPropertiesWidgetPlugin.la libGlGraphWidgetPlugin.la libSGHierarchyWidgetPlugin.la libGWOverviewWidgetPlugin.la

INCLUDES = ${DLL_TULIP_QT} @GL_INCLUDES@ @TULIP_QT_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
if MACOSX_7
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
else
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ -release ${RELEASE}
endif

AM_CPPFLAGS= @QT_CPPFLAGS@
qtplugindir = $(libdir)/tlp/designer

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

BUILT_SOURCES = \
	moc_TulipTableWidget.cpp \
	moc_GlGraphWidget.cpp \
	moc_PropertyWidget.cpp \
	moc_ElementPropertiesWidgetUI.cpp \
	moc_ElementPropertiesWidget.cpp \
	moc_FindSelectionWidget.cpp \
	moc_FindSelectionData.cpp \
	moc_RenderingParametersDialogData.cpp \
	moc_SGHierarchyWidgetUI.cpp \
	FindSelectionData.cpp \
	RenderingParametersDialogData.cpp \
	GWOverviewWidgetData.cpp \
	LayerManagerWidgetData.cpp \
	moc_SGHierarchyWidget.cpp \
	moc_GWOverviewWidget.cpp \
	moc_GWOverviewWidgetData.cpp \
	moc_LayerManagerWidget.cpp \
	moc_LayerManagerWidgetData.cpp 

nodist_libtulip_qt4_la_SOURCES = $(BUILT_SOURCES)

dist_libtulip_qt4_la_SOURCES = \
	TulipTableWidget.cpp \
	GlGraphWidget.cpp \
	PropertyWidget.cpp \
	ElementPropertiesWidgetUI.cpp \
	ElementPropertiesWidget.cpp \
	FindSelectionWidget.cpp \
	SGHierarchyWidgetUI.cpp \
	SGHierarchyWidget.cpp \
	GWOverviewWidget.cpp \
	LayerManagerWidget.cpp \
	Morphing.cpp \
	GraphState.cpp \
	MouseBoxZoomer.cpp \
	MouseEdgeBuilder.cpp \
	MouseInteractors.cpp \
	MouseMagicSelector.cpp \
	MouseNodeBuilder.cpp \
	MouseSelectionEditor.cpp \
	MouseSelector.cpp \
	MouseEdgeSelector.cpp \
	MouseEdgeBendEditor.cpp \
	DataSetDialog.cpp

CLEANFILES = $(BUILT_SOURCES)

moc_%.cpp: $(srcdir)/../include/tulip/%.h 
	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<
moc_%.cpp: ../include/tulip/%.h
	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<

moc_SGHierarchyWidget.cpp: $(srcdir)/../include/tulip/SGHierarchyWidgetQt3.h 
	$(MOC) ${AM_CPPFLAGS} -o $@ $<

FindSelectionData.cpp: $(srcdir)/../designer/FindSelection.ui ../include/tulip/FindSelectionData.h
	$(UIC) -o $@ -impl ../include/tulip/FindSelectionData.h $<

RenderingParametersDialogData.cpp: $(srcdir)/../designer/RenderingParametersDialog.ui ../include/tulip/RenderingParametersDialogData.h
	$(UIC) -o $@ -impl ../include/tulip/RenderingParametersDialogData.h $<

GWOverviewWidgetData.cpp: $(srcdir)/../designer/GWOverviewWidget.ui ../include/tulip/GWOverviewWidgetData.h
	$(UIC) -o $@ -impl ../include/tulip/GWOverviewWidgetData.h $<
