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
QtGlSceneZoomAndPanAnimator.cpp
GlCompositeHierarchyManager.cpp
GraphPropertiesTableWidget.cpp
CSVGraphImport.cpp    
CSVParserConfigurationWidget.cpp
CSVGraphMappingConfigurationWidget.cpp
CSVImportConfigurationWidget.cpp
CSVImportWidget.cpp
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
     ../designer/CSVImportWidget.ui
     ../designer/CSVParserConfigurationWidget.ui
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
    ../include/tulip/CSVImportWidget.h
    ../include/tulip/CSVParserConfigurationWidget.h
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
  FIND_LIBRARY_GLOB(QTCORE_INSTALL_PATH "QtCore4")
  FIND_LIBRARY_GLOB(QTGUI_INSTALL_PATH "QtGui4")
  FIND_LIBRARY_GLOB(QTNETWORK_INSTALL_PATH "QtNetwork4")
  FIND_LIBRARY_GLOB(QTOPENGL_INSTALL_PATH "QtOpenGL4")
  FIND_LIBRARY_GLOB(QTXML_INSTALL_PATH "QtXml4")
  FIND_LIBRARY_GLOB(QTASSISTANT_INSTALL_PATH "QtAssistantClient4")
  FIND_LIBRARY_GLOB(QTSCRIPT_INSTALL_PATH "QtScript4")
  FIND_LIBRARY_GLOB(QTSQL_INSTALL_PATH "QtSql4")
  FIND_LIBRARY_GLOB(QTWEBKIT_INSTALL_PATH "QtWebkit4")
  FIND_LIBRARY_GLOB(QTXMLPATTERNS_INSTALL_PATH "QtXmlPatterns4")
  FIND_LIBRARY_GLOB(PHONON_INSTALL_PATH "phonon4")

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
  FIND_LIBRARY_DEPANDANCIES(TULIP_DEPENDANCIES "${CMAKE_CURRENT_BINARY_DIR}/lib${LibTulipQtName}${CMAKE_SHARED_LIBRARY_SUFFIX}")
  INSTALL_EXTERNAL_LIB(TULIP_DEPENDANCIES libtulip_qt)
  #Copy Qt libs
  FIND_LIBRARY_GLOB_DIR(QTCORE_INSTALL_PATH "QtCore" "/Library/Frameworks/QtCore.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTCORE_DEPS ${QTCORE_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTCORE_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTCORE_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTGUI_INSTALL_PATH "QtGui" "/Library/Frameworks/QtGui.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTGUI_DEPS ${QTGUI_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTGUI_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTGUI_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTNETWORK_INSTALL_PATH "QtNetwork" "/Library/Frameworks/QtNetwork.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTNETWORK_DEPS ${QTNETWORK_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTNETWORK_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTNETWORK_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTOPENGL_INSTALL_PATH "QtOpenGL" "/Library/Frameworks/QtOpenGL.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTOPENGL_DEPS ${QTOPENGL_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTOPENGL_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTOPENGL_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTXML_INSTALL_PATH "QtXml" "/Library/Frameworks/QtXML.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTXML_DEPS ${QTXML_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTXML_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTXML_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTASSISTANT_INSTALL_PATH "QtAssistantClient" "/Library/Frameworks/QtAssistant.framework/")
  IF(QTASSISTANT_INSTALL_PATH)
  FIND_LIBRARY_DEPANDANCIES(QTASSISTANT_DEPS ${QTASSISTANT_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTASSISTANT_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTASSISTANT_DEPS libtulip_qt)
  ENDIF()

  FIND_LIBRARY_GLOB_DIR(QTSCRIPT_INSTALL_PATH "QtScript" "/Library/Frameworks/QtScript.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTSCRIPT_DEPS ${QTSCRIPT_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTSCRIPT_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTSCRIPT_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTSQL_INSTALL_PATH "QtSql" "/Library/Frameworks/QtSql.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTSQL_DEPS ${QTSQL_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTSQL_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTSQL_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTWEBKIT_INSTALL_PATH "QtWebkit" "/Library/Frameworks/QtWebkit.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTWEBKIT_DEPS ${QTWEBKIT_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTWEBKIT_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTWEBKIT_DEPS libtulip_qt)

  FIND_LIBRARY_GLOB_DIR(QTXMLPATTERNS_INSTALL_PATH "QtXmlPatterns" "/Library/Frameworks/QtXmlPatterns.framework/")
  FIND_LIBRARY_DEPANDANCIES(QTXMLPATTERNS_DEPS ${QTXMLPATTERNS_INSTALL_PATH})
  INSTALL_EXTERNAL_LIB(QTXMLPATTERNS_INSTALL_PATH libtulip_qt)
  INSTALL_EXTERNAL_LIB(QTXMLPATTERNS_DEPS libtulip_qt)

  #FIND_LIBRARY_GLOB_DIR(PHONON_INSTALL_PATH "phonon" "/Library/Frameworks/QtXmlPatterns.framework/")
  #FIND_LIBRARY_DEPANDANCIES(PHONON_DEPS QTCORE_INSTALL_PATH)
  #INSTALL_EXTERNAL_LIB(PHONON_INSTALL_PATH libtulip_qt)
  #INSTALL_EXTERNAL_LIB(PHONON_DEPS libtulip_qt)

  #Copy Qt image plugins.
  INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${TulipAppleContentsDir} COMPONENT libtulip_qt)
  #Copy Qt image plugins dependancies
  FILE(GLOB PLUGINS "${QT_PLUGINS_DIR}/imageformats/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  FOREACH(PLUGIN ${PLUGINS})
    FIND_LIBRARY_DEPANDANCIES(QT_PLUGINS_DEPENDANCIES "${PLUGIN}")
    INSTALL_EXTERNAL_LIB(QT_PLUGINS_DEPENDANCIES libtulip_qt)
  ENDFOREACH()
  #Copy Qt menu.
  INSTALL(DIRECTORY "/Library/Frameworks/QtGui.framework/Versions/Current/Resources/qt_menu.nib" DESTINATION "${TulipAppleResourcesDir}" COMPONENT libtulip_qt OPTIONAL)
ENDIF()
