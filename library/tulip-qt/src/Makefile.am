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
qtplugin_LTLIBRARIES = libPropertyWidgetPlugin.la libElementPropertiesWidgetPlugin.la libGlMainWidgetPlugin.la libSGHierarchyWidgetPlugin.la libGWOverviewWidgetPlugin.la

INCLUDES = ${DLL_TULIP_QT} @GL_INCLUDES@ @TULIP_QT_INCLUDE@ -D_REENTRANT -DQT_THREAD_SUPPORT -DQT_PLUGIN -DQT_SHARED \
	   @QT_INCLUDES@ @TULIP_INCLUDE@ @TULIP_OGL_INCLUDE@
PLUGINS_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ @TULIP_QT_LDFLAGS@ -avoid-version
AM_LDFLAGS = ${DLL_FLAGS} @GL_LDFLAGS@ @LIB_GL@ @QT_LDFLAGS@ @LIB_QT@ @TULIP_LDFLAGS@ @TULIP_OGL_LDFLAGS@ -release @MAJOR_VERSION@.@MINOR_VERSION@

AM_CPPFLAGS= @QT_CPPFLAGS@
qtplugindir = $(libdir)/tlp/designer

libPropertyWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
libElementPropertiesWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
libGlMainWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
libGWOverviewWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}
libSGHierarchyWidgetPlugin_la_LDFLAGS = ${PLUGINS_LDFLAGS}

libPropertyWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libElementPropertiesWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libGlMainWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libGWOverviewWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}
libSGHierarchyWidgetPlugin_laINCLUDES = ${DLL_TULIP_QT2}

libSGHierarchyWidgetPlugin_la_SOURCES = SGHierarchyWidgetPlugin.cpp
nodist_libSGHierarchyWidgetPlugin_la_SOURCES = moc_SGHierarchyWidgetPlugin.cpp
libGlMainWidgetPlugin_la_SOURCES = GlMainWidgetPlugin.cpp
nodist_libGlMainWidgetPlugin_la_SOURCES = moc_GlMainWidgetPlugin.cpp 
libGWOverviewWidgetPlugin_la_SOURCES = GWOverviewWidgetPlugin.cpp
nodist_libGWOverviewWidgetPlugin_la_SOURCES = moc_GWOverviewWidgetPlugin.cpp 
libElementPropertiesWidgetPlugin_la_SOURCES = ElementPropertiesWidgetPlugin.cpp
nodist_libElementPropertiesWidgetPlugin_la_SOURCES = moc_ElementPropertiesWidgetPlugin.cpp
libPropertyWidgetPlugin_la_SOURCES = PropertyWidgetPlugin.cpp
nodist_libPropertyWidgetPlugin_la_SOURCES = moc_PropertyWidgetPlugin.cpp

EXTRA_DIST = CMakeLists.txt

BUILT_SOURCES = \
	TulipQtResource.cpp \
	moc_TulipTableWidget.cpp \
	moc_GlMainWidget.cpp \
	moc_GlMainView.cpp \
	moc_NodeLinkDiagramComponent.cpp \
	moc_PropertyWidget.cpp \
	moc_PropertyDialog.cpp \
	moc_CopyPropertyDialog.cpp \
	moc_RenderingParametersDialog.cpp \
	moc_LayerManagerWidget.cpp \
	moc_AugmentedDisplayDialog.cpp \
	moc_ElementPropertiesWidgetUI.cpp \
	moc_ElementPropertiesWidget.cpp \
	moc_FindSelectionWidget.cpp \
	moc_SGHierarchyWidget.cpp \
	moc_GWOverviewWidget.cpp \
	moc_Interactor.cpp \
	moc_PropertyDialogData.cpp \
	moc_CopyPropertyDialogData.cpp \
	moc_RenderingParametersDialogData.cpp \
	moc_GridOptionsWidget.cpp \
	moc_GridOptionsData.cpp \
        moc_PluginProgressWidget.cpp \
	moc_QtProgress.cpp \
	moc_Controller.cpp \
	moc_ControllerViewsManager.cpp \
	moc_MainController.cpp \
	moc_View.cpp \
	moc_AbstractView.cpp \
	moc_GraphPropertiesSelectionWidget.cpp \
	moc_ItemsListWidget.cpp \
	moc_StringsListSelectionWidgetInterface.cpp \
	moc_StringsListSelectionWidget.cpp  \
	moc_SimpleStringsListSelectionWidget.cpp \
	moc_DoubleStringsListSelectionWidget.cpp \
	moc_ColorScaleConfigDialog.cpp \
	moc_ListPropertyWidget.cpp \
	moc_QtGlSceneZoomAndPanAnimator.cpp \
	moc_Morphing.cpp \
        moc_CSVGraphMappingConfigurationWidget.cpp \
        moc_CSVImportConfigurationWidget.cpp \
        moc_CSVImportWizard.cpp \
	moc_CSVParserConfigurationWidget.cpp \
  moc_GraphPropertiesTableWidget.cpp \
  moc_TextEditDialog.cpp \
  moc_SmallMultiplesView.cpp

QT44_BUILT_SOURCES = \
	moc_GlMainWidgetGraphicsView.cpp \
	moc_BaseGraphicsViewComponent.cpp \
	moc_GlMainWidgetItem.cpp \
	moc_TabWidgetHidableMenuGraphicsProxy.cpp

TODIST_SOURCES = \
	TulipTableWidget.cpp \
	GlMainWidget.cpp \
	GlMainView.cpp \
	NodeLinkDiagramComponent.cpp \
	PropertyDialog.cpp \
	CopyPropertyDialog.cpp \
	PropertyWidget.cpp \
	ElementPropertiesWidgetUI.cpp \
	ElementPropertiesWidget.cpp \
	FindSelectionWidget.cpp \
	SGHierarchyWidget.cpp \
	GWOverviewWidget.cpp \
	Morphing.cpp \
	GraphState.cpp \
	MouseSelector.cpp \
	MouseBoxZoomer.cpp \
	MouseEdgeBuilder.cpp \
	MouseInteractors.cpp \
	MouseMagicSelector.cpp \
	MouseNodeBuilder.cpp \
	MouseSelectionEditor.cpp \
	MouseEdgeSelector.cpp \
	MouseEdgeBendEditor.cpp \
	MouseShowElementInfos.cpp \
	DataSetDialog.cpp \
	TlpQtTools.cpp \
	InteractorManager.cpp \
	Interactor.cpp \
	InteractorChainOfResponsibility.cpp \
        PluginProgressWidget.cpp \
	QtProgress.cpp \
	Controller.cpp \
	ControllerAlgorithmTools.cpp \
	ControllerViewsTools.cpp \
	ControllerViewsManager.cpp \
	MainController.cpp \
	View.cpp \
	AbstractView.cpp \
	ControllerPluginsManager.cpp \
	ViewPluginsManager.cpp \
	RenderingParametersDialog.cpp \
	LayerManagerWidget.cpp \
	AugmentedDisplayDialog.cpp \
	GridOptionsWidget.cpp \
	GraphPropertiesSelectionWidget.cpp \
	ItemsListWidget.cpp \
	StringsListSelectionWidget.cpp \
	SimpleStringsListSelectionWidget.cpp \
	DoubleStringsListSelectionWidget.cpp \
	ColorScaleConfigDialog.cpp \
  ColorScaleWidget.cpp \
	QtMetaNodeRenderer.cpp \
  QGlBufferManager.cpp \
	ListPropertyWidget.cpp \
    QtOpenGlErrorViewer.cpp \
	CSVParser.cpp \
	CSVGraphImport.cpp \
	CSVParserConfigurationWidget.cpp \
	CSVGraphMappingConfigurationWidget.cpp \
	CSVImportConfigurationWidget.cpp \
        CSVImportWizard.cpp \
	QtGlSceneZoomAndPanAnimator.cpp \
	GlCompositeHierarchyManager.cpp \
  GraphPropertiesTableWidget.cpp \
  SmallMultiplesView.cpp \
  SmallMultiplesNavigatorComponent.cpp \
  GlOffscreenRenderer.cpp

QT44_SOURCES = \
	GlMainWidgetGraphicsView.cpp \
	BaseGraphicsViewComponent.cpp \
	TabWidgetHidableMenuGraphicsProxy.cpp \
	GraphicsWidgetBase.cpp \
	GlMainWidgetItem.cpp

if QT44
nodist_libtulip_qt4_la_SOURCES = $(BUILT_SOURCES) $(QT44_BUILT_SOURCES)
dist_libtulip_qt4_la_SOURCES = $(TODIST_SOURCES) $(QT44_SOURCES)
else
nodist_libtulip_qt4_la_SOURCES = $(BUILT_SOURCES)
dist_libtulip_qt4_la_SOURCES = $(TODIST_SOURCES)
endif

CLEANFILES = $(BUILT_SOURCES) moc_GlMainWidgetPlugin.cpp moc_PropertyWidgetPlugin.cpp moc_ElementPropertiesWidgetPlugin.cpp moc_SGHierarchyWidgetPlugin.cpp moc_GWOverviewWidgetPlugin.cpp $(QT44_BUILT_SOURCES)



moc_%.cpp: $(srcdir)/../include/tulip/%.h 
	$(MOC) @QT_CPPFLAGS@ ${INCLUDES} -o $@ $<

../include/%Data.h: ../designer/%.ui
	$(UIC) -o $@ $<

moc_SGHierarchyWidget.cpp: $(srcdir)/../include/tulip/SGHierarchyWidget.h 
	$(MOC) @QT_CPPFLAGS@ -o $@ $<

TulipQtResource.cpp: ../designer/TulipQtResource.qrc
	$(RCC) -name TulipQt -o $@ $<
