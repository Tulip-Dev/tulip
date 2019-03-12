INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

QTXWEB_SET_INCLUDES_AND_DEFINITIONS()

SET(LIB_SRCS
src/AlgorithmRunner.cpp
src/AlgorithmRunnerItem.cpp
src/ExpandableGroupBox.cpp
src/ExportWizard.cpp
src/GraphPerspectiveLogger.cpp
src/GraphHierarchiesEditor.cpp
src/GraphPerspective.cpp
src/HeaderFrame.cpp
src/ImportWizard.cpp
src/OutputPanelButton.cpp
src/PanelSelectionWizard.cpp
src/PreferencesDialog.cpp
src/SearchWidget.cpp
)

QTX_ADD_RESOURCES(RCC_SRCS
resources/GraphPerspective.qrc
OPTIONS -name GraphPerspective)

QTX_WRAP_UI(UI_SRCS
designer/GraphPerspectiveLogger.ui
designer/AlgorithmRunner.ui
designer/AlgorithmRunnerItem.ui
designer/GraphHierarchiesEditor.ui
designer/GraphPerspectiveMainWindow.ui
designer/HeaderFrame.ui
designer/ImportWizard.ui
designer/ExportWizard.ui
designer/PanelSelectionWizard.ui
designer/PreferencesDialog.ui
designer/SearchWidget.ui
)

IF(TULIP_BUILD_PYTHON_COMPONENTS)
  ADD_DEFINITIONS("-DTULIP_BUILD_PYTHON_COMPONENTS")
ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)
  
QTX_WRAP_CPP(MOC_SRCS
include/AlgorithmRunner.h
include/AlgorithmRunnerItem.h
include/ExpandableGroupBox.h
include/ExportWizard.h
include/GraphHierarchiesEditor.h
include/GraphPerspective.h
include/GraphPerspectiveLogger.h
include/HeaderFrame.h
include/ImportWizard.h
include/OutputPanelButton.h
include/PanelSelectionWizard.h
include/PreferencesDialog.h
include/SearchWidget.h
)

IF(TULIP_BUILD_PYTHON_COMPONENTS)

INCLUDE_DIRECTORIES(${TulipPythonInclude})

SET(LIB_SRCS
${LIB_SRCS}
src/PythonPanel.cpp
)

QTX_WRAP_UI(UI_SRCS
designer/PythonPanel.ui
)

QTX_WRAP_CPP(MOC_SRCS
include/PythonPanel.h
)

ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)

ADD_LIBRARY(GraphPerspective-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})
TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${QT_LIBRARIES})
IF(TULIP_BUILD_PYTHON_COMPONENTS)
TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipPythonName})
ENDIF(TULIP_BUILD_PYTHON_COMPONENTS)

TULIP_INSTALL_PLUGIN(GraphPerspective-${TulipVersion} ${TulipPerspectivePluginsInstallDir})

