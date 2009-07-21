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
    QtCPULODCalculator.cpp 
    InteractorManager.cpp 
    Interactor.cpp 
    InteractorChainOfResponsibility.cpp 
    GlMainViewInteractor.cpp 
    QtProgress.cpp 
    Controller.cpp 
    MainController.cpp 
    View.cpp 
    AbstractView.cpp 
    ControllerPluginsManager.cpp 
    ViewPluginsManager.cpp 
    TulipStatsWidget.cpp 
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
    QtMetaNodeRenderer.cpp 
    QGlPixelBufferManager.cpp
)

set(tulip-qt_UI
     ../designer/TabWidget.ui
     ../designer/PropertyDialog.ui
     ../designer/CopyPropertyDialog.ui
     ../designer/TulipStats.ui
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
    ../include/tulip/TulipStatsWidget.h
    ../include/tulip/TulipStatsData.h
    ../include/tulip/RenderingParametersDialog.h
    ../include/tulip/LayerManagerWidget.h
    ../include/tulip/AugmentedDisplayDialog.h
    ../include/tulip/ElementPropertiesWidgetUI.h
    ../include/tulip/ElementPropertiesWidget.h
    ../include/tulip/FindSelectionWidget.h
    ../include/tulip/SGHierarchyWidget.h
    ../include/tulip/GWOverviewWidget.h
    ../include/tulip/QtCPULODCalculator.h
    ../include/tulip/Interactor.h
    ../include/tulip/PropertyDialogData.h
    ../include/tulip/CopyPropertyDialogData.h
    ../include/tulip/RenderingParametersDialogData.h
    ../include/tulip/GridOptionsWidget.h
    ../include/tulip/GridOptionsData.h
    ../include/tulip/QtProgress.h
    ../include/tulip/Controller.h
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
)

set(tulip-qt_RCC
    ../designer/TulipQtResource.qrc )

TULIP_QT4_WRAP_UI_IN_INCLUDE_TULIP(tulip-qt_UI_SRCS ${tulip-qt_UI})
TULIP_WRAP_QT_PLUGINS_IN_INCLUDE_TULIP(tulip-qt_PLUGINS_SRCS ${tulip-qt_PLUGINS})
TULIP_QT4_WRAP_CPP(tulip-qt_MOC_SRCS ${tulip-qt_MOC})
QT4_ADD_RESOURCES(tulip-qt_RCC_SRCS ${tulip-qt_RCC})

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include/ ${TulipInclude} ${TulipOGLInclude})
ADD_LIBRARY(tulip-qt4 SHARED ${tulip-qt_PLUGINS_SRCS} ${tulip-qt_UI_SRCS} ${tulip-qt_MOC_SRCS} ${tulip-qt_RCC_SRCS} ${tulip-qt_LIB_SRCS})
TARGET_LINK_LIBRARIES(tulip-qt4 ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-qt4 ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-qt4 ${QT_QTOPENGL_LIBRARY})
TARGET_LINK_LIBRARIES(tulip-qt4 tulip)
TARGET_LINK_LIBRARIES(tulip-qt4 tulip-ogl)
ADD_DEPENDENCIES(tulip-qt4 tulip tulip-ogl)

INSTALL_TARGETS(${TulipLibInstallDir} tulip-qt4)
