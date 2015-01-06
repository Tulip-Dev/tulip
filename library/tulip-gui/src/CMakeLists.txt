
SET(SRCS
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
  DragHandle.cpp
  GlCompositeHierarchyManager.cpp
  GlMainView.cpp
  GlMainWidget.cpp
  GlOffscreenRenderer.cpp
  GraphPropertiesSelectionWidget.cpp
  ItemsListWidget.cpp
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
  NodeLinkDiagramComponentInteractor.cpp
  Perspective.cpp
  PluginManager.cpp
  PropertyCreationDialog.cpp
  QGlBufferManager.cpp
  QtGlSceneZoomAndPanAnimator.cpp
  QuaZIPFacade.cpp
  RenamePropertyDialog.cpp
  SimplePluginProgressWidget.cpp
  SimpleStringsListSelectionWidget.cpp
  StringsListSelectionWidget.cpp
  StringsListSelectionDialog.cpp
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
  ColorScaleButton.cpp
  View.cpp
  GlMainWidgetGraphicsItem.cpp
  ViewWidget.cpp
  InteractorComposite.cpp
  GLInteractor.cpp
  TulipWorkspace.cpp
  WorkspacePanel.cpp
  Interactor.cpp
  ProcessingAnimationItem.cpp
  Workspace.cpp
  PlaceHolderWidget.cpp
  TulipFileDescriptorWidget.cpp
  GraphHierarchiesModel.cpp
  CoordEditor.cpp
  GlOverviewGraphicsItem.cpp
  SceneConfigWidget.cpp
  QuickAccessBar.cpp
  TulipFont.cpp
  TulipFontDialog.cpp
  ScrollPopupButton.cpp
  WorkspaceExposeWidget.cpp
  GraphTableItemDelegate.cpp
  StringEditor.cpp
  VectorEditor.cpp
  GlyphRenderer.cpp
  TulipFontWidget.cpp
  ClearableLineEdit.cpp
  CaptionItem.cpp
  CaptionGraphicsItem.cpp
  CaptionGraphicsSubItems.cpp
  GraphModel.cpp
  GraphElementModel.cpp
  SnapshotDialog.cpp
  TulipMimes.cpp
  SceneLayersModel.cpp
  SceneLayersConfigWidget.cpp
  TreeViewComboBox.cpp
  GlSimpleEntityItemModel.cpp
  PluginModel.cpp
  GraphNeedsSavingObserver.cpp
)

IF(BUILD_PYTHON_COMPONENTS)
  SET(SRCS ${SRCS} PythonVersionChecker.cpp)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PYTHON_COMPONENTS")
  SET_SOURCE_FILES_PROPERTIES(PythonVersionChecker.cpp
                              PROPERTIES COMPILE_DEFINITIONS TLP_PYTHON="${PYTHON_VERSION}")
ENDIF(BUILD_PYTHON_COMPONENTS)

QTX_WRAP_UI(UI_HEADERS
  ../designer/ColorScaleConfigDialog.ui
  ../designer/CopyPropertyDialog.ui
  ../designer/CSVGraphMappingConfigurationWidget.ui
  ../designer/CSVImportConfigurationWidget.ui
  ../designer/CSVImportWizard.ui
  ../designer/CSVParserConfigurationWidget.ui
  ../designer/DoubleStringsListSelectionWidget.ui
  ../designer/PropertyCreationDialog.ui
  ../designer/RenamePropertyDialog.ui
  ../designer/SimplePluginProgressWidget.ui
  ../designer/SimpleStringsListSelectionWidget.ui
  ../designer/StringsListSelectionDialog.ui
  ../designer/WorkspacePanel.ui
  ../designer/Workspace.ui
  ../designer/CoordEditor.ui
  ../designer/SceneConfigWidget.ui
  ../designer/QuickAccessBar.ui
  ../designer/TulipFontDialog.ui
  ../designer/VectorEditor.ui
  ../designer/TulipFontWidget.ui
  ../designer/GridOptionsWidget.ui
  ../designer/ElementInformationsWidget.ui
  ../designer/SnapshotDialog.ui
  ../designer/SceneLayersConfigWidget.ui
)

QTX_WRAP_CPP(MOC_SRCS
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
  ../include/tulip/ItemsListWidget.h
  ../include/tulip/NodeLinkDiagramComponent.h
  ../include/tulip/Perspective.h
  ../include/tulip/PropertyCreationDialog.h
  ../include/tulip/QtGlSceneZoomAndPanAnimator.h
  ../include/tulip/RenamePropertyDialog.h
  ../include/tulip/SimplePluginProgressWidget.h
  ../include/tulip/SimpleStringsListSelectionWidget.h
  ../include/tulip/StringsListSelectionDialog.h
  ../include/tulip/TulipProject.h
  ../include/tulip/Animation.h
  ../include/tulip/ColorButton.h
  ../include/tulip/TulipModel.h
  ../include/tulip/ColorScaleButton.h
  ../include/tulip/TulipSettings.h
  ../include/tulip/View.h
  ../include/tulip/ViewWidget.h
  ../include/tulip/GlMainWidgetGraphicsItem.h
  ../include/tulip/Interactor.h
  ../include/tulip/InteractorComposite.h
  ../include/tulip/GLInteractor.h
  ../include/tulip/WorkspacePanel.h
  ../include/tulip/TulipWorkspace.h
  ../include/tulip/ProcessingAnimationItem.h
  ../include/tulip/Workspace.h
  ../include/tulip/TulipFileDescriptorWidget.h
  ../include/tulip/PlaceHolderWidget.h
  ../include/tulip/GraphHierarchiesModel.h
  ../include/tulip/CoordEditor.h
  ../include/tulip/GlOverviewGraphicsItem.h
  ../include/tulip/SceneConfigWidget.h
  ../include/tulip/QuickAccessBar.h
  ../include/tulip/TulipFont.h
  ../include/tulip/TulipFontDialog.h
  ../include/tulip/ScrollPopupButton.h
  ../include/tulip/WorkspaceExposeWidget.h  
  ../include/tulip/GraphTableItemDelegate.h
  ../include/tulip/VectorEditor.h
  ../include/tulip/TulipFontWidget.h
  ../include/tulip/CaptionItem.h
  ../include/tulip/CaptionGraphicsItem.h
  ../include/tulip/CaptionGraphicsSubItems.h
  ../include/tulip/GraphNeedsSavingObserver.h
  ../include/tulip/DragHandle.h
  ../include/tulip/SnapshotDialog.h
  ../include/tulip/SceneLayersModel.h
  ../include/tulip/SceneLayersConfigWidget.h
  ../include/tulip/TulipMimes.h
  ../include/tulip/TreeViewComboBox.h
  ../include/tulip/TulipItemDelegate.h
  ../include/tulip/GlSimpleEntityItemModel.h
)

QTX_ADD_RESOURCES(RCC_SRCS ../resources/TulipGUIResource.qrc)

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIBuildInclude} ${CMAKE_CURRENT_BINARY_DIR} ${ZLIB_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${QuazipInclude} ${QxtInclude})
ADD_LIBRARY(${LibTulipGUIName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(${LibTulipGUIName} ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOglName} ${QuazipLibrary} ${QxtLibrary})

INSTALL(TARGETS ${LibTulipGUIName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_gui
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui)

INSTALL(FILES ../resources/icons/tulip.ico DESTINATION ${TulipBitmapInstallDir})
