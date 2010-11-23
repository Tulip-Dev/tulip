if !MACOSX
DLL_FLAGS = -no-undefined
else
DLL_FLAGS = -Wl,-headerpad_max_install_names
endif

SUBDIRS = MagicWand
EXTRA_DIST = CMakeLists.txt NodeLinkDiagramComponentInteractor.h

if HAVE_TULIP_QT
pluginsqt_LTLIBRARIES = \
	libInteractorNavigation-@VERSION@.la \
	libInteractorGetInformation-@VERSION@.la \
	libInteractorSelection-@VERSION@.la \
	libInteractorSelectionModifier-@VERSION@.la \
	libInteractorRectangleZoom-@VERSION@.la \
	libInteractorDeleteElement-@VERSION@.la \
	libInteractorAddNode-@VERSION@.la \
	libInteractorAddEdge-@VERSION@.la \
	libInteractorEditEdgeBends-@VERSION@.la

pluginsqtdir = $(libdir)/tlp/interactors

libInteractorNavigation_@VERSION@_la_SOURCES = InteractorNavigation.cpp
libInteractorGetInformation_@VERSION@_la_SOURCES = InteractorGetInformation.cpp
libInteractorSelection_@VERSION@_la_SOURCES = InteractorSelection.cpp
libInteractorSelectionModifier_@VERSION@_la_SOURCES = InteractorSelectionModifier.cpp
libInteractorRectangleZoom_@VERSION@_la_SOURCES = InteractorRectangleZoom.cpp
libInteractorDeleteElement_@VERSION@_la_SOURCES = InteractorDeleteElement.cpp
libInteractorAddNode_@VERSION@_la_SOURCES = InteractorAddNode.cpp
libInteractorAddEdge_@VERSION@_la_SOURCES = InteractorAddEdge.cpp
libInteractorEditEdgeBends_@VERSION@_la_SOURCES = InteractorEditEdgeBends.cpp

INCLUDES = @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@ @TULIP_QT_INCLUDE@ @QT_INCLUDES@ @GL_INCLUDES@
AM_LDFLAGS = ${DLL_FLAGS} -avoid-version @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ @QT_LDFLAGS@ @LIB_QT@
AM_CPPFLAGS= @QT_CPPFLAGS@
endif

