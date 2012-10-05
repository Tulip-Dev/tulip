INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${QT_MODELS_TESTS_INCLUDES})

SET(LIB_SRCS
src/AbstractFiltersManagerItem.cpp
src/AlgorithmRunner.cpp
src/AlgorithmRunnerItem.cpp
src/ControlFrame.cpp
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
src/ShadowFilter.cpp
)

QT4_ADD_RESOURCES(RCC_SRCS
resources/GraphPerspective.qrc
OPTIONS -name GraphPerspective)

QT4_WRAP_UI(UI_SRCS
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

QT4_WRAP_CPP(MOC_SRCS
include/AlgorithmRunner.h
include/AlgorithmRunnerItem.h
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

ADD_LIBRARY(GraphPerspective-${TulipVersion} SHARED ${LIB_SRCS} ${RCC_SRCS} ${UI_SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(GraphPerspective-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS GraphPerspective-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT GraphPerspective)

