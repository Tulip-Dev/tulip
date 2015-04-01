ADD_SUBDIRECTORY(FishEye)
ADD_SUBDIRECTORY(MouseMagnifyingGlass)
ADD_SUBDIRECTORY(MouseLassoNodesSelector)
ADD_SUBDIRECTORY(NeighborhoodHighlighter)
ADD_SUBDIRECTORY(PathFinder)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorNavigation-${TulipVersion} SHARED InteractorNavigation.cpp)
TARGET_LINK_LIBRARIES(InteractorNavigation-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorGetInformation-${TulipVersion} SHARED InteractorGetInformation.cpp)
TARGET_LINK_LIBRARIES(InteractorGetInformation-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelection-${TulipVersion} SHARED InteractorSelection.cpp)
TARGET_LINK_LIBRARIES(InteractorSelection-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelectionModifier-${TulipVersion} SHARED InteractorSelectionModifier.cpp)
TARGET_LINK_LIBRARIES(InteractorSelectionModifier-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorRectangleZoom-${TulipVersion} SHARED InteractorRectangleZoom.cpp)
TARGET_LINK_LIBRARIES(InteractorRectangleZoom-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddNodeEdge-${TulipVersion} SHARED InteractorAddEdge.cpp)
TARGET_LINK_LIBRARIES(InteractorAddNodeEdge-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorEditEdgeBends-${TulipVersion} SHARED InteractorEditEdgeBends.cpp)
TARGET_LINK_LIBRARIES(InteractorEditEdgeBends-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorDeleteElement-${TulipVersion} SHARED InteractorDeleteElement.cpp)
TARGET_LINK_LIBRARIES(InteractorDeleteElement-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS InteractorNavigation-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorNavigation)
INSTALL(TARGETS InteractorGetInformation-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorGetInformation)
INSTALL(TARGETS InteractorSelection-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorSelection)
INSTALL(TARGETS InteractorSelectionModifier-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorSelectionModifier)
INSTALL(TARGETS InteractorRectangleZoom-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorRectangleZoom)
INSTALL(TARGETS InteractorAddNodeEdge-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorAddNodeEdge)
INSTALL(TARGETS InteractorEditEdgeBends-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorEditEdgeBends)
INSTALL(TARGETS InteractorDeleteElement-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT InteractorDeleteElement)
