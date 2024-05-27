INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipPythonInclude} ${OPENGL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

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
src/PanelSelectionWizard.cpp
src/PluginDocDialog.cpp
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
designer/PluginDocDialog.ui
)

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
include/PanelSelectionWizard.h
include/PreferencesDialog.h
include/SearchWidget.h
)

ADD_LIBRARY(GraphPerspective-${Tulip_VERSION} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})
TARGET_LINK_LIBRARIES(GraphPerspective-${Tulip_VERSION} ${QT_LIBRARIES})

TULIP_INSTALL_PLUGIN(GraphPerspective-${Tulip_VERSION} ${TulipPerspectivePluginsInstallDir})
