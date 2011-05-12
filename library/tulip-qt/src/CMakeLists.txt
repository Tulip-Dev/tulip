IF(ENABLE_RENDERING_TIME_DISPLAY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_RENDERING_TIME_DISPLAY")
ENDIF(ENABLE_RENDERING_TIME_DISPLAY)

IF (CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-deprecated-declarations")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

SET(SRCS
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
  Controller.cpp
  ControllerAlgorithmTools.cpp
  ControllerViewsTools.cpp
  ControllerViewsManager.cpp
  MainController.cpp
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
  ListPropertyWidget.cpp
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
  SnapshotDialog.cpp
  PropertyCreationDialog.cpp
  TulipSettings.cpp
  GlMainWidgetGraphicsView.cpp
  BaseGraphicsViewComponent.cpp
  TabWidgetHidableMenuGraphicsProxy.cpp
  GraphicsWidgetBase.cpp
  GlMainWidgetItem.cpp
  PluginInformations.cpp
  PluginManager.cpp
)

QT4_WRAP_UI(UI_HEADERS
  ../designer/TabWidget.ui
  ../designer/PropertyDialog.ui
  ../designer/CopyPropertyDialog.ui
  ../designer/FindSelection.ui
  ../designer/RenderingParametersDialog.ui
  ../designer/LayerManagerWidget.ui
  ../designer/AugmentedDisplayDialog.ui
  ../designer/GWOverviewWidget.ui
  ../designer/GridOptions.ui
  ../designer/PluginProgressWidget.ui
  ../designer/ColorScaleConfigDialog.ui
  ../designer/SimpleStringsListSelectionWidget.ui
  ../designer/DoubleStringsListSelectionWidget.ui
  ../designer/ListPropertyWidget.ui
  ../designer/ChooseFileNameDialog.ui
  ../designer/TextEditDialog.ui
  ../designer/QtOpenGlErrorViewerWithAskAgainDialog.ui
  ../designer/CSVGraphMappingConfigurationWidget.ui
  ../designer/CSVImportConfigurationWidget.ui
  ../designer/CSVParserConfigurationWidget.ui
  ../designer/CSVImportWizard.ui
  ../designer/SnapshotDialog.ui
  ../designer/PropertyCreationDialog.ui
)

QT4_WRAP_CPP(MOC_SRCS
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
  ../include/tulip/GridOptionsWidget.h
  ../include/tulip/PluginProgressWidget.h
  ../include/tulip/QtProgress.h
  ../include/tulip/Controller.h
  ../include/tulip/ControllerViewsManager.h
  ../include/tulip/MainController.h
  ../include/tulip/View.h
  ../include/tulip/AbstractView.h
  ../include/tulip/ItemsListWidget.h
  ../include/tulip/SimpleStringsListSelectionWidget.h
  ../include/tulip/DoubleStringsListSelectionWidget.h
  ../include/tulip/ColorScaleConfigDialog.h
  ../include/tulip/ListPropertyWidget.h
  ../include/tulip/QtGlSceneZoomAndPanAnimator.h
  ../include/tulip/Morphing.h
  ../include/tulip/GraphPropertiesTableWidget.h
  ../include/tulip/TextEditDialog.h
  ../include/tulip/CSVGraphMappingConfigurationWidget.h
  ../include/tulip/CSVImportConfigurationWidget.h
  ../include/tulip/CSVParserConfigurationWidget.h
  ../include/tulip/CSVImportWizard.h
  ../include/tulip/SmallMultiplesView.h
  ../include/tulip/SnapshotDialog.h
  ../include/tulip/PropertyCreationDialog.h
  ../include/tulip/GlMainWidgetGraphicsView.h
  ../include/tulip/BaseGraphicsViewComponent.h
  ../include/tulip/GlMainWidgetItem.h
  ../include/tulip/TabWidgetHidableMenuGraphicsProxy.h
  ../include/tulip/ThreadedComputeProperty.h
)

QT4_ADD_RESOURCES(RCC_SRCS ../resources/TulipQtResource.qrc)

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include ${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtBuildInclude} ${CMAKE_CURRENT_BINARY_DIR})
ADD_LIBRARY(${LibTulipQtName} SHARED ${SRCS} ${MOC_SRCS} ${RCC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_LIBRARIES} ${LibTulipName} ${LibTulipOglName})

INSTALL(TARGETS ${LibTulipQtName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT libtulip_qt
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_qt
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_qt)
INSTALL(FILES ${UI_SRCS} DESTINATION ${TulipIncludeInstallDir}/tulip/)

IF(WIN32)
  #Copy Qt libs
  set(QT_LIBS phonon QtCLucene QtCore QtHelp QtGui QtNetwork QtOpenGL QtScript QtSql QtSvg QtWebKit QtXml QtXmlPatterns)
  FOREACH(QT_LIB ${QT_LIBS})
    FIND_LIBRARY_GLOB(QT_LIB_FILE "${QT_LIB}4.dll")
    INSTALL_EXTERNAL_LIB(QT_LIB_FILE libtulip_qt)
  ENDFOREACH()
ELSEIF(APPLE)
  #Copy Qt libs
  set(QT_LIBS phonon QtAssistant QtCore QtDBus QtHelp QtGui QtNetwork QtOpenGL QtScript QtSql QtSvg QtWebKit QtXml QtXmlPatterns)
  FOREACH(QT_LIB ${QT_LIBS})
    FIND_LIBRARY_GLOB_DIR(QT_LIB_FILE "${QT_LIB}.framework/${QT_LIB}" "/Library/Frameworks")
    INSTALL_EXTERNAL_LIB(QT_LIB_FILE libtulip_qt)
  ENDFOREACH()

  FIND_LIBRARY_GLOB_DIR(QTLUCENE_INSTALL_PATH "libQtCLucene.4.dylib" "/usr/lib")
  INSTALL_EXTERNAL_LIB(QTLUCENE_INSTALL_PATH libtulip_qt)  

  #Copy Qt image plugins.
  INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${TulipAppleContentsDir} COMPONENT libtulip_qt)
  #Copy Qt image plugins dependancies
  FILE(GLOB PLUGINS "${QT_PLUGINS_DIR}/imageformats/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  FOREACH(PLUGIN ${PLUGINS})
    FIND_LIBRARY_DEPENDANCIES(QT_PLUGINS_DEPENDENCIES "${PLUGIN}")
    INSTALL_EXTERNAL_LIB(QT_PLUGINS_DEPENDENCIES libtulip_qt)
  ENDFOREACH()
  #Copy Qt menu.
  INSTALL(DIRECTORY "${QT_LIBRARY_DIR}/QtGui.framework/Versions/Current/Resources/qt_menu.nib" DESTINATION "${TulipAppleResourcesDir}" COMPONENT libtulip_qt OPTIONAL)
ENDIF()
