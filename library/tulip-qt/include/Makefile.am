EXTRA_DIST = CMakeLists.txt

INCLUDES_H = \
	tulip/TulipTableWidget.h \
	tulip/GlMainWidget.h \
	tulip/TabWidget.h \
	tulip/PropertyDialog.h  \
	tulip/CopyPropertyDialog.h \
	tulip/RenderingParametersDialog.h \
	tulip/LayerManagerWidget.h \
	tulip/AugmentedDisplayDialog.h \
	tulip/GridOptionsWidget.h \
	tulip/GlMainView.h \
	tulip/NodeLinkDiagramComponent.h \
	tulip/InteractorManager.h \
	tulip/Interactor.h \
	tulip/InteractorChainOfResponsibility.h \
	tulip/InteractorComponent.h \
	tulip/ForegroundEntity.h \
	tulip/MouseSelector.h \
	tulip/MouseBoxZoomer.h \
	tulip/MouseEdgeBuilder.h \
	tulip/MouseInteractors.h \
	tulip/MouseMagicSelector.h \
	tulip/MouseNodeBuilder.h \
	tulip/MouseSelectionEditor.h \
	tulip/MouseEdgeSelector.h \
	tulip/MouseEdgeBendEditor.h \
	tulip/MouseShowElementInfos.h \
	tulip/PropertyWidget.h \
	tulip/ElementPropertiesWidgetUI.h \
	tulip/ElementPropertiesWidget.h \
	tulip/FindSelectionWidget.h \
	tulip/SGHierarchyWidget.h \
	tulip/GWOverviewWidget.h \
	tulip/TlpQtTools.h \
	tulip/SGHierarchyWidgetPlugin.h \
	tulip/GlMainWidgetPlugin.h \
	tulip/ElementPropertiesWidgetPlugin.h \
	tulip/GWOverviewWidgetPlugin.h \
	tulip/PropertyWidgetPlugin.h \
	tulip/QtProgress.h \
	tulip/Controller.h \
	tulip/ControllerAlgorithmTools.h \
	tulip/ControllerViewsTools.h \
	tulip/ControllerViewsManager.h \
	tulip/MainController.h \
	tulip/ControllerPluginsManager.h \
	tulip/View.h \
	tulip/AbstractView.h \
	tulip/GraphPropertiesSelectionWidget.h \
	tulip/ItemsListWidget.h \
	tulip/StringsListSelectionWidget.h \
	tulip/ViewPluginsManager.h \
	tulip/ColorScaleConfigDialog.h \
	tulip/ColorScaleWidget.h \
	tulip/QtMetaNodeRenderer.h \
	tulip/StringsListSelectionWidgetInterface.h \
	tulip/SimpleStringsListSelectionWidget.h \
	tulip/DoubleStringsListSelectionWidget.h \
	tulip/ListPropertyWidget.h \
	tulip/QGlBufferManager.h \
	tulip/QGlPixelBufferManager.h \
	tulip/BaseGraphicsViewComponentMacro.h \
	tulip/QtOpenGlErrorViewer.h \
	tulip/CSVContentHandler.h \
	tulip/CSVGraphImport.h \
	tulip/CSVGraphMappingConfigurationWidget.h \
	tulip/CSVImportConfigurationWidget.h \
	tulip/CSVImportDialog.h \
	tulip/CSVParser.h \
	tulip/CSVParserConfigurationWidget.h \
	tulip/QtGlSceneZoomAndPanAnimator.h \
	tulip/Morphing.h \
	tulip/GlCompositeHierarchyManager.h \
	tulip/GraphPropertiesTableWidget.h \
	tulip/TextEditDialog.h

QT44_INCLUDES_H = \
	tulip/BaseGraphicsViewComponent.h \
	tulip/GlMainWidgetGraphicsView.h \
	tulip/GlMainWidgetItem.h \
	tulip/TabWidgetHidableMenuGraphicsProxy.h \
	tulip/GraphicsWidgetBase.h

if QT44
nobase_include_HEADERS = $(INCLUDES_H) $(QT44_INCLUDES_H)
else
nobase_include_HEADERS = $(INCLUDES_H)
endif

noinst_HEADERS = \
    tulip/Morphing.h


BUILT_SOURCES = \
	tulip/TabWidgetData.h \
	tulip/PropertyDialogData.h \
	tulip/CopyPropertyDialogData.h \
	tulip/FindSelectionData.h \
	tulip/RenderingParametersDialogData.h \
	tulip/LayerManagerWidgetData.h \
	tulip/AugmentedDisplayDialogData.h \
	tulip/GWOverviewWidgetData.h \
	tulip/GridOptionsData.h \
	tulip/QtProgressData.h \
	tulip/ColorScaleConfigDialogData.h \
	tulip/SimpleStringsListSelectionWidgetData.h \
	tulip/DoubleStringsListSelectionWidgetData.h \
	tulip/ListPropertyWidgetData.h \
	tulip/ChooseFileNameDialogData.h \
        tulip/QtOpenGlErrorViewerWithAskAgainDialogData.h \
	tulip/CSVParserConfigurationWidgetData.h \
	tulip/CSVGraphMappingConfigurationWidgetData.h \
	tulip/CSVImportConfigurationWidgetData.h \
	tulip/CSVImportDialogData.h \
        tulip/TextEditDialogData.h

nobase_nodist_include_HEADERS = $(BUILT_SOURCES)

CLEANFILES = $(BUILT_SOURCES)

tulip/%Data.h: ../designer/%.ui
	mkdir -p tulip
	$(UIC) -o $@ $<
