INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(LIB_SRCS
src/AbstractFiltersManagerItem.cpp
src/AlgorithmRunner.cpp
src/AlgorithmRunnerItem.cpp
src/ControlFrame.cpp
src/DocumentationNavigator.cpp
src/ExpandableGroupBox.cpp
src/ExportWizard.cpp
src/GraphPerspectiveLogger.cpp
src/FiltersManager.cpp
src/FiltersManagerItem.cpp
src/FiltersManagerCompareItem.cpp
src/FiltersManagerInvertItem.cpp
src/FiltersManagerAlgorithmItem.cpp
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
designer/ControlFrame.ui
designer/FiltersManager.ui
designer/FiltersManagerItem.ui
designer/FiltersManagerCompareItem.ui
designer/FiltersManagerAlgorithmItem.ui
designer/FiltersManagerInvertItem.ui
designer/GraphHierarchiesEditor.ui
designer/GraphPerspectiveMainWindow.ui
designer/HeaderFrame.ui
designer/ImportWizard.ui
designer/ExportWizard.ui
designer/PanelSelectionWizard.ui
designer/PreferencesDialog.ui
designer/SearchWidget.ui
)

QTX_WRAP_CPP(MOC_SRCS
include/AlgorithmRunner.h
include/AlgorithmRunnerItem.h
include/DocumentationNavigator.h
include/ExpandableGroupBox.h
include/ExportWizard.h
include/FiltersManager.h
include/FiltersManagerItem.h
include/AbstractFiltersManagerItem.h
include/FiltersManagerCompareItem.h
include/FiltersManagerInvertItem.h
include/FiltersManagerAlgorithmItem.h
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

IF(BUILD_PYTHON_COMPONENTS)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_PYTHON_COMPONENTS")

INCLUDE_DIRECTORIES(${TulipPythonInclude})

SET(LIB_SRCS
${LIB_SRCS}
src/PythonPanel.cpp
src/PythonPluginCreationDialog.cpp
src/PythonPluginsIDE.cpp
)

QTX_WRAP_UI(UI_SRCS
designer/PythonPanel.ui
designer/PythonPluginCreationDialog.ui
designer/PythonPluginsIDE.ui
)

QTX_WRAP_CPP(MOC_SRCS
include/PythonPanel.h
include/PythonPluginCreationDialog.h
include/PythonPluginsIDE.h
)

ENDIF(BUILD_PYTHON_COMPONENTS)

SET(QT_USE_QTWEBKIT true)

ADD_LIBRARY(GraphPerspective-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})
TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${QT_LIBRARIES})
IF(BUILD_PYTHON_COMPONENTS)
TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipPythonName})
ENDIF(BUILD_PYTHON_COMPONENTS)

INSTALL(TARGETS GraphPerspective-${TulipVersion} DESTINATION ${TulipPerspectivePluginsInstallDir} COMPONENT GraphPerspective)

