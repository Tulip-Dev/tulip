if WIN32 
DLL_FLAGS = -no-undefined
DLL_TULIP_QT = -DDLL_TULIP_QT
DLL_TULIP_QT2 = -DDLL_TULIP_QT2
RELEASE = ${VERSION}
else 
RELEASE = @TULIP_VERSION@ -version-info 0:0:0
endif

if CYGWIN
DLL_FLAGS = -no-undefined
RELEASE = ${VERSION}
endif

lib_LTLIBRARIES = libtulip-qt.la
qtplugin_LTLIBRARIES = libTulipWidgets.la

INCLUDES = ${DLL_TULIP_QT} @GL_INCLUDES@ @TULIP_QT_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@
qtplugindir = $(libdir)/tlp/plugins/designer

BUILT_SOURCES = \
	moc_TulipTableWidget.cpp \
	moc_GlGraphWidget.cpp \
	moc_PropertyWidgets.cpp \
	moc_TulipElementPropertiesData.cpp \
	moc_TulipElementProperties.cpp \
	moc_FindSelection.cpp \
	moc_FindSelectionData.cpp \
	moc_ClusterTreeData.cpp \
	moc_ClusterTree.cpp

libTulipWidgets_la_LDFLAGS = @TULIP_QT_LDFLAGS@ -module -avoid-version
libTulipWidgets_laINCLUDES = ${DLL_TULIP_QT2}

dist_libTulipWidgets_la_SOURCES = \
	TulipWidgetsPlugin.cpp

libtulip_qt_la_LDFLAGS = -release ${RELEASE}  

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

moc_%.cpp: ../include/tulip/%.h 
	$(MOC) -o $@ $<

FindSelectionData.cpp: ../designer/FindSelection.ui ../include/tulip/FindSelectionData.h
	$(UIC) -o $@ -impl ../include/tulip/FindSelectionData.h $<

../include/tulip/FindSelectionData.h: ../designer/FindSelection.ui
	$(UIC) -o $@ $<
