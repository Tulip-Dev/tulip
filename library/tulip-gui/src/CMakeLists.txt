IF(ENABLE_RENDERING_TIME_DISPLAY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_RENDERING_TIME_DISPLAY")
ENDIF(ENABLE_RENDERING_TIME_DISPLAY)

IF (CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-deprecated-declarations")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

SET(SRCS
  GlMainWidget.cpp
  GlMainView.cpp
  NodeLinkDiagramComponent.cpp
  CopyPropertyDialog.cpp
  SGHierarchyWidget.cpp
  GWOverviewWidget.cpp
  Morphing.cpp
  GraphState.cpp
  MouseSelector.cpp
  MouseBoxZoomer.cpp
  MouseEdgeBuilder.cpp
  MouseInteractors.cpp
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
  PluginProgressWidget.cpp
  QtProgress.cpp
  View.cpp
  AbstractView.cpp
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
  QGlBufferManager.cpp
  QtOpenGlErrorViewer.cpp
  CSVParser.cpp
  QtGlSceneZoomAndPanAnimator.cpp
  GlCompositeHierarchyManager.cpp
  GraphPropertiesTableWidget.cpp
  CSVGraphImport.cpp
  CSVParserConfigurationWidget.cpp
  CSVGraphMappingConfigurationWidget.cpp
  CSVImportConfigurationWidget.cpp
  CSVImportWizard.cpp
  SmallMultiplesView.cpp
  SmallMultiplesNavigatorComponent.cpp
  GlOffscreenRenderer.cpp
  ThreadedComputeProperty.cpp
  PropertyCreationDialog.cpp
  TulipSettings.cpp
  GlMainWidgetGraphicsView.cpp
  BaseGraphicsViewComponent.cpp
  TabWidgetHidableMenuGraphicsProxy.cpp
  GraphicsWidgetBase.cpp
  GlMainWidgetItem.cpp
  PluginInformations.cpp
  PluginManager.cpp
  TulipProject.cpp
  QuaZIPFacade.cpp
)

QT4_WRAP_UI(UI_HEADERS
  ../designer/CopyPropertyDialog.ui
  ../designer/RenderingParametersDialog.ui
  ../designer/LayerManagerWidget.ui
  ../designer/AugmentedDisplayDialog.ui
  ../designer/GWOverviewWidget.ui
  ../designer/GridOptions.ui
  ../designer/PluginProgressWidget.ui
  ../designer/ColorScaleConfigDialog.ui
  ../designer/SimpleStringsListSelectionWidget.ui
  ../designer/DoubleStringsListSelectionWidget.ui
  ../designer/QtOpenGlErrorViewerWithAskAgainDialog.ui
  ../designer/CSVGraphMappingConfigurationWidget.ui
  ../designer/CSVImportConfigurationWidget.ui
  ../designer/CSVParserConfigurationWidget.ui
  ../designer/CSVImportWizard.ui
  ../designer/PropertyCreationDialog.ui
)

QT4_WRAP_CPP(MOC_SRCS
  ../include/tulip/GlMainWidget.h
  ../include/tulip/GlMainView.h
  ../include/tulip/NodeLinkDiagramComponent.h
  ../include/tulip/CopyPropertyDialog.h
  ../include/tulip/RenderingParametersDialog.h
  ../include/tulip/LayerManagerWidget.h
  ../include/tulip/AugmentedDisplayDialog.h
  ../include/tulip/SGHierarchyWidget.h
  ../include/tulip/GWOverviewWidget.h
  ../include/tulip/Interactor.h
  ../include/tulip/GridOptionsWidget.h
  ../include/tulip/PluginProgressWidget.h
  ../include/tulip/QtProgress.h
  ../include/tulip/View.h
  ../include/tulip/AbstractView.h
  ../include/tulip/ItemsListWidget.h
  ../include/tulip/SimpleStringsListSelectionWidget.h
  ../include/tulip/DoubleStringsListSelectionWidget.h
  ../include/tulip/ColorScaleConfigDialog.h
  ../include/tulip/QtGlSceneZoomAndPanAnimator.h
  ../include/tulip/Morphing.h
  ../include/tulip/GraphPropertiesTableWidget.h
  ../include/tulip/CSVGraphMappingConfigurationWidget.h
  ../include/tulip/CSVImportConfigurationWidget.h
  ../include/tulip/CSVParserConfigurationWidget.h
  ../include/tulip/CSVImportWizard.h
  ../include/tulip/SmallMultiplesView.h
  ../include/tulip/PropertyCreationDialog.h
  ../include/tulip/GlMainWidgetGraphicsView.h
  ../include/tulip/BaseGraphicsViewComponent.h
  ../include/tulip/GlMainWidgetItem.h
  ../include/tulip/TabWidgetHidableMenuGraphicsProxy.h
  ../include/tulip/ThreadedComputeProperty.h
  ../include/tulip/TulipProject.h
  ../include/tulip/PluginManager.h
)

QT4_ADD_RESOURCES(RCC_SRCS ../resources/TulipQtResource.qrc)

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIBuildInclude} ${CMAKE_CURRENT_BINARY_DIR} ${QuazipInclude} ${QT_INCLUDE_DIR})
ADD_LIBRARY(${LibTulipGUIName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(${LibTulipGUIName} ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOglName} quazip)

INSTALL(TARGETS ${LibTulipGUIName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_gui
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui)
