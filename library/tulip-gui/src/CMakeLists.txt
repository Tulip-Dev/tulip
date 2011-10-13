IF(ENABLE_RENDERING_TIME_DISPLAY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_RENDERING_TIME_DISPLAY")
ENDIF(ENABLE_RENDERING_TIME_DISPLAY)

SET(SRCS
  AugmentedDisplayDialog.cpp
  ColorScaleConfigDialog.cpp
  ColorScaleWidget.cpp
  CopyPropertyDialog.cpp
  CSVGraphImport.cpp
  CSVGraphMappingConfigurationWidget.cpp
  CSVImportConfigurationWidget.cpp
  CSVImportWizard.cpp
  CSVParserConfigurationWidget.cpp
  CSVParser.cpp
  DoubleStringsListSelectionWidget.cpp
  DownloadManager.cpp
  GlCompositeHierarchyManager.cpp
  GlMainView.cpp
  GlMainWidget.cpp
  GlOffscreenRenderer.cpp
  GraphicsWidgetBase.cpp
  GraphPropertiesSelectionWidget.cpp
  GraphPropertiesTableWidget.cpp
  GraphState.cpp
  GridOptionsWidget.cpp
  GWOverviewWidget.cpp
  InteractorChainOfResponsibility.cpp
  Interactor.cpp
  InteractorManager.cpp
  ItemsListWidget.cpp
  LayerManagerWidget.cpp
  Morphing.cpp
  MouseBoxZoomer.cpp
  MouseEdgeBendEditor.cpp
  MouseEdgeBuilder.cpp
  MouseEdgeSelector.cpp
  MouseInteractors.cpp
  MouseNodeBuilder.cpp
  MouseSelectionEditor.cpp
  MouseSelector.cpp
  MouseShowElementInfos.cpp
  NodeLinkDiagramComponent.cpp
  Perspective.cpp
  PluginInformations.cpp
  PluginManager.cpp
  PropertyCreationDialog.cpp
  QGlBufferManager.cpp
  QtGlSceneZoomAndPanAnimator.cpp
  QtOpenGlErrorViewer.cpp
  QuaZIPFacade.cpp
  RenderingParametersDialog.cpp
  SGHierarchyWidget.cpp
  SimplePluginProgressWidget.cpp
  SimpleStringsListSelectionWidget.cpp
  SmallMultiplesNavigatorComponent.cpp
  SmallMultiplesView.cpp
  StringsListSelectionWidget.cpp
  TabWidgetHidableMenuGraphicsProxy.cpp
  ThreadedComputeProperty.cpp
  TlpQtTools.cpp
  TulipProject.cpp
  TulipSettings.cpp
  ParameterListModel.cpp
  TulipItemDelegate.cpp
  TulipMetaTypes.cpp
  TulipItemEditorCreators.cpp
  Animation.cpp
  DoublePropertyAnimation.cpp
  LayoutPropertyAnimation.cpp
  PropertyAnimation.cpp
  TulipParallelAnimationGroup.cpp
  ColorButton.cpp
  TulipModel.cpp
  SizeEditor.cpp
  ColorScaleButton.cpp
  View.cpp
  GlMainWidgetGraphicsItem.cpp
ViewWidget.cpp
)

QT4_WRAP_UI(UI_HEADERS
  ../designer/AugmentedDisplayDialog.ui
  ../designer/ColorScaleConfigDialog.ui
  ../designer/CopyPropertyDialog.ui
  ../designer/CSVGraphMappingConfigurationWidget.ui
  ../designer/CSVImportConfigurationWidget.ui
  ../designer/CSVImportWizard.ui
  ../designer/CSVParserConfigurationWidget.ui
  ../designer/DoubleStringsListSelectionWidget.ui
  ../designer/GridOptions.ui
  ../designer/GWOverviewWidget.ui
  ../designer/LayerManagerWidget.ui
  ../designer/PropertyCreationDialog.ui
  ../designer/QtOpenGlErrorViewerWithAskAgainDialog.ui
  ../designer/RenderingParametersDialog.ui
  ../designer/SimplePluginProgressWidget.ui
  ../designer/SimpleStringsListSelectionWidget.ui
  ../designer/SizeEditor.ui
)

QT4_WRAP_CPP(MOC_SRCS
  ../include/tulip/AugmentedDisplayDialog.h
  ../include/tulip/ColorScaleConfigDialog.h
  ../include/tulip/CopyPropertyDialog.h
  ../include/tulip/CSVGraphMappingConfigurationWidget.h
  ../include/tulip/CSVImportConfigurationWidget.h
  ../include/tulip/CSVImportWizard.h
  ../include/tulip/CSVParserConfigurationWidget.h
  ../include/tulip/DoubleStringsListSelectionWidget.h
  ../include/tulip/DownloadManager.h
  ../include/tulip/GlMainView.h
  ../include/tulip/GlMainWidget.h
  ../include/tulip/GraphPropertiesTableWidget.h
  ../include/tulip/GridOptionsWidget.h
  ../include/tulip/GWOverviewWidget.h
  ../include/tulip/Interactor.h
  ../include/tulip/ItemsListWidget.h
  ../include/tulip/LayerManagerWidget.h
  ../include/tulip/Morphing.h
  ../include/tulip/NodeLinkDiagramComponent.h
  ../include/tulip/Perspective.h
  ../include/tulip/PluginInformations.h
  ../include/tulip/PluginManager.h
  ../include/tulip/PropertyCreationDialog.h
  ../include/tulip/QtGlSceneZoomAndPanAnimator.h
  ../include/tulip/RenderingParametersDialog.h
  ../include/tulip/SGHierarchyWidget.h
  ../include/tulip/SimplePluginProgressWidget.h
  ../include/tulip/SimpleStringsListSelectionWidget.h
  ../include/tulip/SmallMultiplesView.h
  ../include/tulip/TabWidgetHidableMenuGraphicsProxy.h
  ../include/tulip/ThreadedComputeProperty.h
  ../include/tulip/TulipProject.h
  ../include/tulip/Animation.h
  ../include/tulip/ColorButton.h
  ../include/tulip/TulipModel.h
  ../include/tulip/SizeEditor.h
  ../include/tulip/ColorScaleButton.h
  ../include/tulip/TulipSettings.h
  ../include/tulip/View.h
  ../include/tulip/ViewWidget.h
  ../include/tulip/GlMainWidgetGraphicsItem.h
)

QT4_ADD_RESOURCES(RCC_SRCS ../resources/TulipGUIResource.qrc)

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIBuildInclude} ${CMAKE_CURRENT_BINARY_DIR} ${ZLIB_INCLUDE_DIR} ${QuazipInclude} ${QT_INCLUDE_DIR})
ADD_LIBRARY(${LibTulipGUIName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(${LibTulipGUIName} ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOglName} quazip)

INSTALL(TARGETS ${LibTulipGUIName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_gui
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui)
