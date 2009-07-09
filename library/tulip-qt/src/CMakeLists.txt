FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

MACRO (TULIP_QT4_WRAP_UI outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/${outfile}Data.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (TULIP_QT4_WRAP_UI)

MACRO (TULIP_WRAP_QT_PLUGINS outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    STRING(TOLOWER ${outfile} outfile_lower)
    SET(outfile_lower ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/${outfile_lower}.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile_lower}
      COMMAND cp
      ARGS ${infile} ${outfile_lower}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile_lower})
  ENDFOREACH (it)
ENDMACRO (TULIP_WRAP_QT_PLUGINS)

MACRO (TULIP_QT4_WRAP_CPP outfiles )
  QT4_GET_MOC_FLAGS(moc_flags)
  QT4_EXTRACT_OPTIONS(moc_files moc_options ${ARGN})

  FOREACH (it ${moc_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(it ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/moc_${outfile}.cpp)
    QT4_CREATE_MOC_COMMAND(${it} ${outfile} "${moc_flags}" "${moc_options}")
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)

ENDMACRO (TULIP_QT4_WRAP_CPP)		

set(tulip-qt_LIB_SRCS 
     TulipTableWidget.cpp
     GlMainWidget.cpp
     GlMainView.cpp
     NodeLinkDiagramComponent.cpp
     PropertyDialog.cpp
     CopyPropertyDialog.cpp
     PropertyWidget.cpp
     ElementPropertiesWidgetUI.cpp
     ElementPropertiesWidget.cpp
     FindSelectionWidget.cpp
     SGHierarchyWidget.cpp
     GWOverviewWidget.cpp
     Morphing.cpp
     GraphState.cpp
     MouseSelector.cpp
     MouseBoxZoomer.cpp
     MouseEdgeBuilder.cpp
     MouseInteractors.cpp
     MouseMagicSelector.cpp
     MouseNodeBuilder.cpp
     MouseSelectionEditor.cpp
     MouseEdgeSelector.cpp
     MouseEdgeBendEditor.cpp
     MouseShowElementInfos.cpp
     DataSetDialog.cpp
     TlpQtTools.cpp
     QtCPULODCalculator.cpp
     GWInteractor.cpp
     InteractorManager.cpp
     QtProgress.cpp
     Controller.cpp
     MainController.cpp
     View.cpp
     AbstractView.cpp
     ControllerPluginsManager.cpp
     ViewPluginsManager.cpp
     TulipStatsWidget.cpp
     RenderingParametersDialog.cpp
     AugmentedDisplayDialog.cpp
     GridOptionsWidget.cpp)

set(tulip-qt_UI
     ../designer/TabWidget.ui
     ../designer/PropertyDialog.ui
     ../designer/CopyPropertyDialog.ui
     ../designer/TulipStats.ui
     ../designer/FindSelection.ui
     ../designer/RenderingParametersDialog.ui
     ../designer/AugmentedDisplayDialog.ui
     ../designer/GWOverviewWidget.ui
     ../designer/GridOptions.ui
     ../designer/QtProgress.ui)

set(tulip-qt_PLUGINS
     ../include/tulip/SGHierarchyWidget.h
     ../include/tulip/GlMainWidget.h
     ../include/tulip/ElementPropertiesWidget.h
     ../include/tulip/GWOverviewWidget.h
     ../include/tulip/PropertyWidget.h)

set(tulip-qt_MOC
     ../include/tulip/TulipTableWidget.h
     ../include/tulip/GlMainWidget.h
     ../include/tulip/GlMainView.h
     ../include/tulip/NodeLinkDiagramComponent.h
     ../include/tulip/PropertyWidget.h
     ../include/tulip/PropertyDialog.h
     ../include/tulip/CopyPropertyDialog.h
     ../include/tulip/TulipStatsWidget.h
     ../include/tulip/TulipStatsData.h
     ../include/tulip/RenderingParametersDialog.h
     ../include/tulip/AugmentedDisplayDialog.h
     ../include/tulip/ElementPropertiesWidgetUI.h
     ../include/tulip/ElementPropertiesWidget.h
     ../include/tulip/FindSelectionWidget.h
     ../include/tulip/SGHierarchyWidget.h
     ../include/tulip/GWOverviewWidget.h
     ../include/tulip/QtCPULODCalculator.h
     ../include/tulip/PropertyDialogData.h
     ../include/tulip/CopyPropertyDialogData.h
     ../include/tulip/TulipStatsData.h
     ../include/tulip/RenderingParametersDialogData.h
     ../include/tulip/GridOptionsWidget.h
     ../include/tulip/GridOptionsData.h
     ../include/tulip/QtProgress.h
     ../include/tulip/Controller.h
     ../include/tulip/MainController.h
     ../include/tulip/View.h
     ../include/tulip/AbstractView.h)

set(tulip-qt_RCC
  ../designer/TulipQtResource.qrc )

TULIP_QT4_WRAP_UI(tulip-qt_UI_SRCS ${tulip-qt_UI})
TULIP_WRAP_QT_PLUGINS(tulip-qt_PLUGINS_SRCS ${tulip-qt_PLUGINS})
TULIP_QT4_WRAP_CPP(tulip-qt_MOC_SRCS ${tulip-qt_MOC})
QT4_ADD_RESOURCES(tulip-qt_RCC_SRCS ${tulip-qt_RCC})

INCLUDE_DIRECTORIES(../include/ ${TULIP_INCLUDE} ${TULIP_OGL_INCLUDE})
ADD_LIBRARY(tulip-qt SHARED ${tulip-qt_PLUGINS_SRCS} ${tulip-qt_UI_SRCS} ${tulip-qt_MOC_SRCS} ${tulip-qt_RCC_SRCS} ${tulip-qt_LIB_SRCS})
ADD_DEPENDENCIES(tulip-qt ../include)


########### install files ###############

INSTALL_TARGETS(${TULIP_LIB_INSTALL_DIR} tulip-qt)


#original Makefile.am contents follow:

#if WIN32 
#DLL_FLAGS = -no-undefined
#DLL_TULIP_QT = -DDLL_TULIP_QT
#DLL_TULIP_QT2 = -DDLL_TULIP_QT2
#else 
#if MACOSX
#DLL_FLAGS = -Wl,-headerpad_max_install_names
#endif
#endif
#
#lib_LTLIBRARIES = libtulip-qt4.la
#qtplugin_LTLIBRARIES = libPropertyWidgetPlugin.la libElementPropertiesWidgetPlugin.la libGlMainWidgetPlugin.la libSGHierarchyWidgetPlugin.la libGWOverviewWidgetPlugin.la
#
#INCLUDES = ${DLL_TULIP_QT} @GL_INCLUDES@ @TULIP_QT_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
#	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
#PLUGINS_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -avoid-version
#AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ -release @MAJOR_VERSION@.@MINOR_VERSION@
#
#AM_CPPFLAGS= @QT_CPPFLAGS@
#qtplugindir = $(libdir)/tlp/designer
#
#libPropertyWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
#libElementPropertiesWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
#libGlMainWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
#libGWOverviewWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
#libSGHierarchyWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
#
#libPropertyWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
#libElementPropertiesWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
#libGlMainWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
#libGWOverviewWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
#libSGHierarchyWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
#
#libSGHierarchyWidgetPlugin_la_SOURCES = SGHierarchyWidgetPlugin.cpp
#nodist_libSGHierarchyWidgetPlugin_la_SOURCES = moc_SGHierarchyWidgetPlugin.cpp
#libGlMainWidgetPlugin_la_SOURCES = GlMainWidgetPlugin.cpp
#nodist_libGlMainWidgetPlugin_la_SOURCES = moc_GlMainWidgetPlugin.cpp 
#libGWOverviewWidgetPlugin_la_SOURCES = GWOverviewWidgetPlugin.cpp
#nodist_libGWOverviewWidgetPlugin_la_SOURCES = moc_GWOverviewWidgetPlugin.cpp 
#libElementPropertiesWidgetPlugin_la_SOURCES = ElementPropertiesWidgetPlugin.cpp
#nodist_libElementPropertiesWidgetPlugin_la_SOURCES = moc_ElementPropertiesWidgetPlugin.cpp
#libPropertyWidgetPlugin_la_SOURCES = PropertyWidgetPlugin.cpp
#nodist_libPropertyWidgetPlugin_la_SOURCES = moc_PropertyWidgetPlugin.cpp
#
#BUILT_SOURCES = \
#	TulipQtResource.cpp \
#	moc_TulipTableWidget.cpp \
#	moc_GlMainWidget.cpp \
#	moc_GlMainView.cpp \
#	moc_NodeLinkDiagramComponent.cpp \
#	moc_PropertyWidget.cpp \
#	moc_PropertyDialog.cpp \
#	moc_CopyPropertyDialog.cpp \
#	moc_TulipStatsWidget.cpp \
#	moc_TulipStatsData.cpp \
#	moc_RenderingParametersDialog.cpp \
#	moc_AugmentedDisplayDialog.cpp \
#	moc_ElementPropertiesWidgetUI.cpp \
#	moc_ElementPropertiesWidget.cpp \
#	moc_FindSelectionWidget.cpp \
#	moc_SGHierarchyWidget.cpp \
#	moc_GWOverviewWidget.cpp \
#	moc_QtCPULODCalculator.cpp \
#	moc_PropertyDialogData.cpp \
#	moc_CopyPropertyDialogData.cpp \
#	moc_TulipStatsData.cpp \
#	moc_RenderingParametersDialogData.cpp \
#	moc_GridOptionsWidget.cpp \
#	moc_GridOptionsData.cpp \
#	moc_QtProgress.cpp \
#	moc_Controller.cpp \
#	moc_MainController.cpp \
#	moc_View.cpp \
#	moc_AbstractView.cpp
#
#nodist_libtulip_qt4_la_SOURCES = $(BUILT_SOURCES)
#
#dist_libtulip_qt4_la_SOURCES = \
#	TulipTableWidget.cpp \
#	GlMainWidget.cpp \
#	GlMainView.cpp \
#	NodeLinkDiagramComponent.cpp \
#	PropertyDialog.cpp \
#	CopyPropertyDialog.cpp \
#	PropertyWidget.cpp \
#	ElementPropertiesWidgetUI.cpp \
#	ElementPropertiesWidget.cpp \
#	FindSelectionWidget.cpp \
#	SGHierarchyWidget.cpp \
#	GWOverviewWidget.cpp \
#	Morphing.cpp \
#	GraphState.cpp \
#	MouseSelector.cpp \
#	MouseBoxZoomer.cpp \
#	MouseEdgeBuilder.cpp \
#	MouseInteractors.cpp \
#	MouseMagicSelector.cpp \
#	MouseNodeBuilder.cpp \
#	MouseSelectionEditor.cpp \
#	MouseEdgeSelector.cpp \
#	MouseEdgeBendEditor.cpp \
#	MouseShowElementInfos.cpp \
#	DataSetDialog.cpp \
#	TlpQtTools.cpp \
#	QtCPULODCalculator.cpp \
#	GWInteractor.cpp \
#	InteractorManager.cpp \
#	QtProgress.cpp \
#	Controller.cpp \
#	MainController.cpp \
#	View.cpp \
#	AbstractView.cpp \
#	ControllerPluginsManager.cpp \
#	ViewPluginsManager.cpp \
#	TulipStatsWidget.cpp \
#	RenderingParametersDialog.cpp \
#	AugmentedDisplayDialog.cpp \
#	GridOptionsWidget.cpp
#
#CLEANFILES = $(BUILT_SOURCES)
#
#moc_%.cpp: $(srcdir)/../include/tulip/%.h 
#	$(MOC) ${AM_CPPFLAGS} ${INCLUDES} -o $@ $<
#
#../include/%Data.h: ../designer/%.ui
#	$(UIC) -o $@ $<
#
#moc_SGHierarchyWidget.cpp: $(srcdir)/../include/tulip/SGHierarchyWidget.h 
#	$(MOC) ${AM_CPPFLAGS} -o $@ $<
#
#TulipQtResource.cpp: ../designer/TulipQtResource.qrc
#	$(RCC) -name TulipQt -o $@ $<
#
#
