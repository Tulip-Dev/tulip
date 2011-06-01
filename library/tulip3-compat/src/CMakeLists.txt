SET(SRCS
  ControllerAlgorithmTools.cpp
  Controller.cpp
  ControllerViewsManager.cpp
  ControllerViewsTools.cpp
  ListPropertyWidget.cpp
  PropertyWidget.cpp
  MainController.cpp
  PropertyDialog.cpp
  TulipTableWidget.cpp
  ElementPropertiesWidget.cpp
  ElementPropertiesWidgetUI.cpp
  FindSelectionWidget.cpp
  SnapshotDialog.cpp
)

QT4_WRAP_CPP(MOC_SRCS
  ../include/tulip3/Controller.h
  ../include/tulip3/ControllerViewsManager.h
  ../include/tulip3/PropertyWidget.h
  ../include/tulip3/ListPropertyWidget.h
  ../include/tulip3/MainController.h
  ../include/tulip3/PropertyDialog.h
  ../include/tulip3/TulipTableWidget.h
  ../include/tulip3/ElementPropertiesWidget.h
  ../include/tulip3/ElementPropertiesWidgetUI.h
  ../include/tulip3/FindSelectionWidget.h
  ../include/tulip3/SnapshotDialog.h
  ../include/tulip3/TextEditDialog.h
)

QT4_WRAP_UI(UI_HEADERS
  ../designer/ListPropertyWidget.ui
  ../designer/PropertyDialog.ui
  ../designer/TabWidget.ui
  ../designer/ChooseFileNameDialog.ui
  ../designer/TextEditDialog.ui
  ../designer/FindSelection.ui
  ../designer/SnapshotDialog.ui
)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipGUIBuildInclude} ${Tulip3CompatInclude} ${Tulip3CompatBuildInclude} ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(${LibTulip3CompatName} SHARED ${SRCS} ${MOC_SRCS} ${UI_HEADERS})

TARGET_LINK_LIBRARIES(${LibTulip3CompatName} ${QT_LIBRARIES} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

INSTALL(TARGETS ${LibTulip3CompatName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip3_compat
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip3_compat
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip3_compat)
