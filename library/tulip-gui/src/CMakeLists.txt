
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
    GlMainView.cpp
    GlMainWidget.cpp
    GlOffscreenRenderer.cpp
    GraphPropertiesSelectionWidget.cpp
    ItemsListWidget.cpp
    MouseBoxZoomer.cpp
  #  MouseEdgeBendEditor.cpp
    MouseEdgeBuilder.cpp
    MouseInteractors.cpp
    MouseNodeBuilder.cpp
  #  MouseSelectionEditor.cpp
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
    GlyphPreviewRenderer.cpp
    TulipFontWidget.cpp
    ClearableLineEdit.cpp
    GraphModel.cpp
    GraphElementModel.cpp
    SnapshotDialog.cpp
    TulipMimes.cpp
    SceneLayersModel.cpp
    SceneLayersConfigWidget.cpp
    TreeViewComboBox.cpp
    GlEntityItemModel.cpp
    PluginModel.cpp
    GraphNeedsSavingObserver.cpp
    FileDownloader.cpp
    TextureFileDialog.cpp
    FontIconManager.cpp
    ColorScalesManager.cpp
    RangeSlider.cpp
    AboutTulipPage.cpp
)

IF(BUILD_PYTHON_COMPONENTS)
  SET(PYTHON_PART_SRCS PythonVersionChecker.cpp)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PYTHON_COMPONENTS")
  SET_SOURCE_FILES_PROPERTIES(PythonVersionChecker.cpp
                              PROPERTIES COMPILE_DEFINITIONS TLP_PYTHON="${PYTHON_VERSION}")
  ADD_LIBRARY(tulip-gui-python STATIC ${PYTHON_PART_SRCS})
  TARGET_LINK_LIBRARIES(tulip-gui-python ${QT_LIBRARIES})
  IF(UNIX)
    SET_TARGET_PROPERTIES(tulip-gui-python
                          PROPERTIES COMPILE_OPTIONS "-fPIC")
  ENDIF(UNIX)
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
  ../designer/TextureFileDialog.ui
  ../designer/TulipFontDialog.ui
  ../designer/VectorEditor.ui
  ../designer/TulipFontWidget.ui
  ../designer/GridOptionsWidget.ui
  ../designer/ElementInformationsWidget.ui
  ../designer/SnapshotDialog.ui
  ../designer/SceneLayersConfigWidget.ui
  ../designer/AboutTulipPage.ui
)

QTX_WRAP_CPP(MOC_SRCS
  ../include/tulip/Animation.h
  ../include/tulip/ColorButton.h
  ../include/tulip/ColorScaleButton.h
  ../include/tulip/ColorScaleConfigDialog.h
  ../include/tulip/CoordEditor.h
  ../include/tulip/CopyPropertyDialog.h
  ../include/tulip/CSVGraphMappingConfigurationWidget.h
  ../include/tulip/CSVImportConfigurationWidget.h
  ../include/tulip/CSVImportWizard.h
  ../include/tulip/CSVParserConfigurationWidget.h
  ../include/tulip/DoubleStringsListSelectionWidget.h
  ../include/tulip/DownloadManager.h
  ../include/tulip/DragHandle.h
  ../include/tulip/FileDownloader.h
  ../include/tulip/GLInteractor.h
  ../include/tulip/GlMainView.h
  ../include/tulip/GlMainWidget.h
  ../include/tulip/GlMainWidgetGraphicsItem.h
  ../include/tulip/GlOverviewGraphicsItem.h
  ../include/tulip/GraphHierarchiesModel.h
  ../include/tulip/GraphNeedsSavingObserver.h
  ../include/tulip/GraphTableItemDelegate.h
  ../include/tulip/Interactor.h
  ../include/tulip/InteractorComposite.h
  ../include/tulip/ItemsListWidget.h
  ../include/tulip/NodeLinkDiagramComponent.h
  ../include/tulip/Perspective.h
  ../include/tulip/PlaceHolderWidget.h
  ../include/tulip/ProcessingAnimationItem.h
  ../include/tulip/PropertyCreationDialog.h
  ../include/tulip/QtGlSceneZoomAndPanAnimator.h
  ../include/tulip/QuickAccessBar.h
  ../include/tulip/SceneConfigWidget.h
  ../include/tulip/SceneLayersModel.h
  ../include/tulip/SceneLayersConfigWidget.h
  ../include/tulip/ScrollPopupButton.h
  ../include/tulip/SimplePluginProgressWidget.h
  ../include/tulip/SimpleStringsListSelectionWidget.h
  ../include/tulip/SnapshotDialog.h
  ../include/tulip/RangeSlider.h
  ../include/tulip/StringsListSelectionDialog.h
  ../include/tulip/TextureFileDialog.h
  ../include/tulip/TreeViewComboBox.h
  ../include/tulip/TulipFont.h
  ../include/tulip/TulipFontDialog.h
  ../include/tulip/TulipFontWidget.h
  ../include/tulip/TulipItemDelegate.h
  ../include/tulip/TulipMimes.h
  ../include/tulip/TulipModel.h
  ../include/tulip/TulipProject.h
  ../include/tulip/TulipSettings.h
  ../include/tulip/TulipWorkspace.h
  ../include/tulip/VectorEditor.h
  ../include/tulip/View.h
  ../include/tulip/ViewWidget.h
  ../include/tulip/WorkspacePanel.h
  ../include/tulip/Workspace.h
  ../include/tulip/PlaceHolderWidget.h
  ../include/tulip/GraphHierarchiesModel.h
  ../include/tulip/CoordEditor.h
  ../include/tulip/SceneConfigWidget.h
  ../include/tulip/QuickAccessBar.h
  ../include/tulip/TulipFont.h
  ../include/tulip/TulipFontDialog.h
  ../include/tulip/ScrollPopupButton.h
  ../include/tulip/WorkspaceExposeWidget.h
  ../include/tulip/GraphTableItemDelegate.h
  ../include/tulip/VectorEditor.h
  ../include/tulip/TulipFontWidget.h
  ../include/tulip/GraphNeedsSavingObserver.h
  ../include/tulip/DragHandle.h
  ../include/tulip/SnapshotDialog.h
  ../include/tulip/SceneLayersModel.h
  ../include/tulip/SceneLayersConfigWidget.h
  ../include/tulip/TulipMimes.h
  ../include/tulip/TreeViewComboBox.h
  ../include/tulip/TulipItemDelegate.h
  ../include/tulip/GlEntityItemModel.h
  ../include/tulip/FileDownloader.h
)

QTX_ADD_RESOURCES(RCC_SRCS ../resources/TulipGUIResource.qrc)

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(${TulipGUIBuildInclude} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${ZLIB_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${QuazipInclude} ${QtAwesomeInclude})
ADD_LIBRARY(${LibTulipGUIName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(${LibTulipGUIName} ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOGLESName} ${QuazipLibrary} ${QtAwesomeLibrary})

IF(BUILD_PYTHON_COMPONENTS)
TARGET_LINK_LIBRARIES(${LibTulipGUIName} tulip-gui-python)
ENDIF(BUILD_PYTHON_COMPONENTS)

INSTALL(TARGETS ${LibTulipGUIName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_gui
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_gui)

INSTALL(FILES ../resources/icons/tulip.ico DESTINATION ${TulipBitmapInstallDir})
INSTALL(FILES ../resources/icons/tulip-file-icon.ico DESTINATION ${TulipBitmapInstallDir})

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
COPY_TARGET_LIBRARY_POST_BUILD(${LibTulipGUIName} ${TULIPGUI_PYTHON_NATIVE_FOLDER} wheels)
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)
