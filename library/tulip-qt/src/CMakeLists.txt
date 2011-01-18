FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")	

IF(ENABLE_RENDERING_TIME_DISPLAY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_RENDERING_TIME_DISPLAY")
ENDIF(ENABLE_RENDERING_TIME_DISPLAY)

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
QtGlSceneZoomAndPanAnimator.cpp
GlCompositeHierarchyManager.cpp
GraphPropertiesTableWidget.cpp
CSVGraphImport.cpp    
CSVParserConfigurationWidget.cpp
CSVGraphMappingConfigurationWidget.cpp
CSVImportConfigurationWidget.cpp
CSVImportWizard.cpp
SmallMultiplesView.cpp
AbstractSmallMultiplesModel.cpp
SmallMultiplesNavigatorComponent.cpp
GlOffscreenRenderer.cpp
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
     ../designer/QtOpenGlErrorViewerWithAskAgainDialog.ui
     ../designer/CSVGraphMappingConfigurationWidget.ui
     ../designer/CSVImportConfigurationWidget.ui
     ../designer/CSVParserConfigurationWidget.ui
     ../designer/CSVImportWizard.ui
)

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
    ../include/tulip/QtGlSceneZoomAndPanAnimator.h
    ../include/tulip/Morphing.h
    ../include/tulip/GraphPropertiesTableWidget.h
    ../include/tulip/TextEditDialog.h
    ../include/tulip/CSVGraphMappingConfigurationWidget.h
    ../include/tulip/CSVImportConfigurationWidget.h
    ../include/tulip/CSVParserConfigurationWidget.h    
    ../include/tulip/CSVImportWizard.h
    ../include/tulip/AbstractSmallMultiplesModel.h
    ../include/tulip/SmallMultiplesView.h
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
TULIP_QT4_WRAP_CPP(tulip-qt_MOC_SRCS ${tulip-qt_MOC} OPTIONS "-DQT_MINOR_REL=${QT_VERSION_MINOR}")
QT4_ADD_RESOURCES(tulip-qt_RCC_SRCS ${tulip-qt_RCC})

ADD_DEFINITIONS(-DQT_DLL)
ADD_DEFINITIONS(-DQT_THREAD_SUPPORT)

INCLUDE_DIRECTORIES(../include/ ../include/tulip ${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtBuildInclude} ${PROJECT_BINARY_DIR}/library/tulip-qt/include/tulip/)
ADD_LIBRARY(${LibTulipQtName} SHARED ${tulip-qt_UI_SRCS} ${tulip-qt_MOC_SRCS} ${tulip-qt_RCC_SRCS} ${tulip-qt_LIB_SRCS})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${QT_QTOPENGL_LIBRARY})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${LibTulipName})
TARGET_LINK_LIBRARIES(${LibTulipQtName} ${LibTulipOglName})
ADD_DEPENDENCIES(${LibTulipQtName} ${LibTulipName} ${LibTulipOglName})

INSTALL(TARGETS ${LibTulipQtName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT libtulip_qt
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_qt
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT libtulip_qt)
INSTALL(FILES ${tulip-qt_UI_SRCS} DESTINATION ${TulipIncludeInstallDir}/tulip/)

IF(WIN32)
  FIND_LIBRARY_GLOB(QTCORE_INSTALL_PATH "QtCore4.dll")
  FIND_LIBRARY_GLOB(QTGUI_INSTALL_PATH "QtGui4.dll")
  FIND_LIBRARY_GLOB(QTNETWORK_INSTALL_PATH "QtNetwork4.dll")
  FIND_LIBRARY_GLOB(QTOPENGL_INSTALL_PATH "QtOpenGL4.dll")
  FIND_LIBRARY_GLOB(QTXML_INSTALL_PATH "QtXml4.dll")
  FIND_LIBRARY_GLOB(QTASSISTANT_INSTALL_PATH "QtAssistantClient4.dll")
  FIND_LIBRARY_GLOB(QTSCRIPT_INSTALL_PATH "QtScript4.dll")
  FIND_LIBRARY_GLOB(QTSQL_INSTALL_PATH "QtSql4.dll")
  FIND_LIBRARY_GLOB(QTWEBKIT_INSTALL_PATH "QtWebkit4.dll")
  FIND_LIBRARY_GLOB(QTXMLPATTERNS_INSTALL_PATH "QtXmlPatterns4.dll")
  FIND_LIBRARY_GLOB(PHONON_INSTALL_PATH "phonon4.dll")

  INSTALL_EXTERNAL_LIB(QTCORE_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTGUI_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTNETWORK_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTOPENGL_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTXML_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTASSISTANT_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTSCRIPT_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTSQL_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTWEBKIT_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTXMLPATTERNS_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(PHONON_INSTALL_PATH libtulip_qt)
ELSEIF(APPLE)  
  #Copy Qt libs
  FIND_LIBRARY_GLOB(QTCORE_INSTALL_PATH "QtCore.framework/QtCore") 
  INSTALL_EXTERNAL_LIB(QTCORE_INSTALL_PATH libtulip_qt)  

  FIND_LIBRARY_GLOB(QTGUI_INSTALL_PATH "QtGui.framework/QtGui")  
  INSTALL_EXTERNAL_LIB(QTGUI_INSTALL_PATH libtulip_qt)  

  FIND_LIBRARY_GLOB(QTNETWORK_INSTALL_PATH "QtNetwork.framework/QtNetwork")  
  INSTALL_EXTERNAL_LIB(QTNETWORK_INSTALL_PATH libtulip_qt)  

  FIND_LIBRARY_GLOB(QTOPENGL_INSTALL_PATH "QtOpenGL.framework/QtOpenGL")
  INSTALL_EXTERNAL_LIB(QTOPENGL_INSTALL_PATH libtulip_qt)

  FIND_LIBRARY_GLOB(QTXML_INSTALL_PATH "QtXml.framework/QtXml")
  INSTALL_EXTERNAL_LIB(QTXML_INSTALL_PATH libtulip_qt)

  FIND_LIBRARY_GLOB(QTASSISTANT_INSTALL_PATH "QtAssistant.framework/QtAssistant")
  IF(QTASSISTANT_INSTALL_PATH)
  INSTALL_EXTERNAL_LIB(QTASSISTANT_INSTALL_PATH libtulip_qt)
  #Copy QtAssistant_adp apps.
  INSTALL(DIRECTORY "/Developer/Applications/Qt/Assistant_adp.app" DESTINATION ${TulipAppleContentsDir} COMPONENT libtulip_qt OPTIONAL)
  ENDIF()

  FIND_LIBRARY_GLOB(QTSCRIPT_INSTALL_PATH "QtScript.framework/QtScript")
  INSTALL_EXTERNAL_LIB(QTSCRIPT_INSTALL_PATH libtulip_qt)

  FIND_LIBRARY_GLOB(QTSQL_INSTALL_PATH "QtSql.framework/QtSql")
  INSTALL_EXTERNAL_LIB(QTSQL_INSTALL_PATH libtulip_qt)

  FIND_LIBRARY_GLOB(QTWEBKIT_INSTALL_PATH "QtWebKit.framework/QtWebKit")
  INSTALL_EXTERNAL_LIB(QTWEBKIT_INSTALL_PATH libtulip_qt)

  FIND_LIBRARY_GLOB(QTXMLPATTERNS_INSTALL_PATH "QtXmlPatterns.framework/QtXmlPatterns")
  INSTALL_EXTERNAL_LIB(QTXMLPATTERNS_INSTALL_PATH libtulip_qt)

  FIND_LIBRARY_GLOB(PHONON_INSTALL_PATH "phonon.framework/phonon")
  INSTALL_EXTERNAL_LIB(PHONON_INSTALL_PATH libtulip_qt)

  #Copy Qt image plugins.
  INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${TulipAppleContentsDir} COMPONENT libtulip_qt)
  #Copy Qt image plugins dependancies
  FILE(GLOB PLUGINS "${QT_PLUGINS_DIR}/imageformats/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  FOREACH(PLUGIN ${PLUGINS})
    FIND_LIBRARY_DEPENDANCIES(QT_PLUGINS_DEPENDENCIES "${PLUGIN}")
    INSTALL_EXTERNAL_LIB(QT_PLUGINS_DEPENDENCIES libtulip_qt)
  ENDFOREACH()
  #Copy Qt menu.
  INSTALL(DIRECTORY "/Library/Frameworks/QtGui.framework/Versions/Current/Resources/qt_menu.nib" DESTINATION "${TulipAppleResourcesDir}" COMPONENT libtulip_qt OPTIONAL)
ENDIF()
