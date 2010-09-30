FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")	

SET(tulip-qt_LIB_SRCS
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
    InteractorManager.cpp 
    Interactor.cpp 
    InteractorChainOfResponsibility.cpp
    QtProgress.cpp 
    Controller.cpp 
    ControllerAlgorithmTools.cpp
    ControllerViewsTools.cpp
    ControllerViewsManager.cpp
    MainController.cpp 
    View.cpp 
    AbstractView.cpp 
    ControllerPluginsManager.cpp 
    ViewPluginsManager.cpp 
    RenderingParametersDialog.cpp 
    LayerManagerWidget.cpp 
    AugmentedDisplayDialog.cpp 
    GridOptionsWidget.cpp 
    GraphPropertiesSelectionWidget.cpp 
    ItemsListWidget.cpp 
    StringsListSelectionWidget.cpp 
    SimpleStringsListSelectionWidget.cpp 
    DoubleStringsListSelectionWidget.cpp 
    ColorScaleConfigDialog.cpp 
    ColorScaleWidget.cpp
    QtMetaNodeRenderer.cpp 
    QGlBufferManager.cpp
    ListPropertyWidget.cpp
    QtOpenGlErrorViewer.cpp
    CSVParser.cpp
    ImportCSVDataDialog.cpp
    ImportCSVDataWidget.cpp
    ImportCSVDataConfigurationWidget.cpp
    QtGlSceneZoomAndPanAnimator.cpp
    GlCompositeHierarchyManager.cpp
    GraphPropertiesTableWidget.cpp
)

set(tulip-qt_UI
     ../designer/TabWidget.ui
     ../designer/PropertyDialog.ui
     ../designer/CopyPropertyDialog.ui
     ../designer/FindSelection.ui
     ../designer/RenderingParametersDialog.ui
     ../designer/LayerManagerWidget.ui
     ../designer/AugmentedDisplayDialog.ui
     ../designer/GWOverviewWidget.ui
     ../designer/GridOptions.ui
     ../designer/QtProgress.ui
     ../designer/ColorScaleConfigDialog.ui
     ../designer/SimpleStringsListSelectionWidget.ui
     ../designer/DoubleStringsListSelectionWidget.ui
     ../designer/ListPropertyWidget.ui
     ../designer/ChooseFileNameDialog.ui
     ../designer/TextEditDialog.ui
     ../designer/ImportCSVDataConfigurationWidget.ui
     ../designer/QtOpenGlErrorViewerWithAskAgainDialog.ui     
)

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
    ../include/tulip/RenderingParametersDialog.h
    ../include/tulip/LayerManagerWidget.h
    ../include/tulip/AugmentedDisplayDialog.h
    ../include/tulip/ElementPropertiesWidgetUI.h
    ../include/tulip/ElementPropertiesWidget.h
    ../include/tulip/FindSelectionWidget.h
    ../include/tulip/SGHierarchyWidget.h
    ../include/tulip/GWOverviewWidget.h
    ../include/tulip/Interactor.h
    ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/PropertyDialogData.h
    ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/CopyPropertyDialogData.h
    ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/RenderingParametersDialogData.h
    ../include/tulip/GridOptionsWidget.h
    ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/GridOptionsData.h
    ../include/tulip/QtProgress.h
    ../include/tulip/Controller.h
    ../include/tulip/ControllerViewsManager.h
    ../include/tulip/MainController.h
    ../include/tulip/View.h
    ../include/tulip/AbstractView.h
    ../include/tulip/GraphPropertiesSelectionWidget.h
    ../include/tulip/ItemsListWidget.h
    ../include/tulip/StringsListSelectionWidgetInterface.h
    ../include/tulip/StringsListSelectionWidget.h
    ../include/tulip/SimpleStringsListSelectionWidget.h
    ../include/tulip/DoubleStringsListSelectionWidget.h
    ../include/tulip/ColorScaleConfigDialog.h
    ../include/tulip/ListPropertyWidget.h
    ../include/tulip/ImportCSVDataDialog.h
    ../include/tulip/ImportCSVDataWidget.h
    ../include/tulip/ImportCSVDataConfigurationWidget.h
    ../include/tulip/QtGlSceneZoomAndPanAnimator.h
    ../include/tulip/Morphing.h
    ../include/tulip/GraphPropertiesTableWidget.h
    ../include/tulip/TextEditDialog.h
)

IF(QT_VERSION_MINOR GREATER 3)
SET(tulip-qt_LIB_SRCS
    ${tulip-qt_LIB_SRCS}
    GlMainWidgetGraphicsView.cpp
    BaseGraphicsViewComponent.cpp
    TabWidgetHidableMenuGraphicsProxy.cpp
    GraphicsWidgetBase.cpp
    GlMainWidgetItem.cpp
)
set(tulip-qt_MOC
    ${tulip-qt_MOC}
    ../include/tulip/GlMainWidgetGraphicsView.h
    ../include/tulip/BaseGraphicsViewComponent.h
    ../include/tulip/GlMainWidgetItem.h
    ../include/tulip/TabWidgetHidableMenuGraphicsProxy.h
)
ENDIF(QT_VERSION_MINOR GREATER 3)

set(tulip-qt_RCC
    ../designer/TulipQtResource.qrc )

TULIP_QT4_WRAP_UI_IN_INCLUDE_TULIP(tulip-qt_UI_SRCS ${tulip-qt_UI})
TULIP_WRAP_QT_PLUGINS_IN_INCLUDE_TULIP(tulip-qt_PLUGINS_SRCS ${tulip-qt_PLUGINS})
TULIP_QT4_WRAP_CPP(tulip-qt_MOC_SRCS ${tulip-qt_MOC} OPTIONS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")
QT4_ADD_RESOURCES(tulip-qt_RCC_SRCS ${tulip-qt_RCC})

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include/ ../include/tulip ${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtBuildInclude} ${PROJECT_BINARY_DIR}/library/tulip-qt/include/tulip/)
ADD_LIBRARY(${LibTulipQtName} SHARED ${tulip-qt_PLUGINS_SRCS} ${tulip-qt_UI_SRCS} ${tulip-qt_MOC_SRCS} ${tulip-qt_RCC_SRCS} ${tulip-qt_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_QTOPENGL_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${LibTulipOglName})
ADD_DEPENDENCIES(${LibTulipQtName} ${LibTulipName} ${LibTulipOglName})

INSTALL_TARGETS(${TulipLibInstallDir} ${LibTulipQtName})
INSTALL_FILES(${TulipIncludeInstallDir}/tulip/ FILES ${tulip-qt_UI_SRCS})
