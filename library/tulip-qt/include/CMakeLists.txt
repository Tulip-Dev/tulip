file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tulip)

INSTALL_FILES(${TulipIncludeInstallDir}/tulip/ FILES
	tulip/TulipTableWidget.h 
	tulip/GlMainWidget.h 
	tulip/TabWidget.h 
	tulip/PropertyDialog.h  
	tulip/CopyPropertyDialog.h 
	tulip/TulipStatsWidget.h 
	tulip/RenderingParametersDialog.h 
	tulip/LayerManagerWidget.h 
	tulip/AugmentedDisplayDialog.h 
	tulip/GridOptionsWidget.h 
	tulip/GlMainView.h 
	tulip/NodeLinkDiagramComponent.h 
	tulip/InteractorManager.h 
	tulip/Interactor.h 
	tulip/InteractorChainOfResponsibility.h 
	tulip/InteractorComponent.h 
	tulip/GlMainViewInteractor.h 
	tulip/ForegroundEntity.h 
	tulip/MouseSelector.h 
	tulip/MouseBoxZoomer.h 
	tulip/MouseEdgeBuilder.h 
	tulip/MouseInteractors.h 
	tulip/MouseMagicSelector.h 
	tulip/MouseNodeBuilder.h 
	tulip/MouseSelectionEditor.h 
	tulip/MouseEdgeSelector.h 
	tulip/MouseEdgeBendEditor.h 
	tulip/MouseShowElementInfos.h 
	tulip/PropertyWidget.h 
	tulip/ElementPropertiesWidgetUI.h 
	tulip/ElementPropertiesWidget.h 
	tulip/FindSelectionWidget.h 
	tulip/SGHierarchyWidget.h 
	tulip/GWOverviewWidget.h 
	tulip/TlpQtTools.h 
	tulip/SGHierarchyWidgetPlugin.h 
	tulip/GlMainWidgetPlugin.h 
	tulip/ElementPropertiesWidgetPlugin.h 
	tulip/GWOverviewWidgetPlugin.h 
	tulip/PropertyWidgetPlugin.h 
	tulip/QtCPULODCalculator.h 
	tulip/QtProgress.h 
	tulip/Controller.h 
	tulip/MainController.h 
	tulip/ControllerPluginsManager.h 
	tulip/View.h 
	tulip/AbstractView.h 
	tulip/GraphPropertiesSelectionWidget.h 
	tulip/ItemsListWidget.h 
	tulip/StringsListSelectionWidget.h 
	tulip/ViewPluginsManager.h 
	tulip/ColorScaleConfigDialog.h 
	tulip/QtMetaNodeRenderer.h 
	tulip/StringsListSelectionWidgetInterface.h 
	tulip/SimpleStringsListSelectionWidget.h 
	tulip/DoubleStringsListSelectionWidget.h 
	tulip/QGlPixelBufferManager.h
	tulip/QtQuadTreeLODCalculator.h
	tulip/BaseGraphicsViewComponentMacro.h
)

IF(QT_VERSION_MINOR GREATER 3)
INSTALL_FILES(${TulipIncludeInstallDir}/tulip/ FILES
        tulip/BaseGraphicsViewComponent.h
	tulip/GlMainWidgetGraphicsView.h
	tulip/GlMainWidgetItem.h
	tulip/TabWidgetHidableMenuGraphicsProxy.h	
	tulip/GraphicsWidgetBase.h
)
ENDIF(QT_VERSION_MINOR GREATER 3)
