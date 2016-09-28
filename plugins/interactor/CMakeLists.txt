ADD_SUBDIRECTORY(FishEye)
ADD_SUBDIRECTORY(MouseMagnifyingGlass)
ADD_SUBDIRECTORY(MouseLassoNodesSelector)
ADD_SUBDIRECTORY(NeighborhoodHighlighter)
ADD_SUBDIRECTORY(PathFinder)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorNavigation-${TulipVersion} SHARED InteractorNavigation.cpp)
TARGET_LINK_LIBRARIES(InteractorNavigation-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorGetInformation-${TulipVersion} SHARED InteractorGetInformation.cpp)
TARGET_LINK_LIBRARIES(InteractorGetInformation-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelection-${TulipVersion} SHARED InteractorSelection.cpp)
TARGET_LINK_LIBRARIES(InteractorSelection-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelectionModifier-${TulipVersion} SHARED InteractorSelectionModifier.cpp)
TARGET_LINK_LIBRARIES(InteractorSelectionModifier-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorRectangleZoom-${TulipVersion} SHARED InteractorRectangleZoom.cpp)
TARGET_LINK_LIBRARIES(InteractorRectangleZoom-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddNodeEdge-${TulipVersion} SHARED InteractorAddEdge.cpp)
TARGET_LINK_LIBRARIES(InteractorAddNodeEdge-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorEditEdgeBends-${TulipVersion} SHARED InteractorEditEdgeBends.cpp)
TARGET_LINK_LIBRARIES(InteractorEditEdgeBends-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorDeleteElement-${TulipVersion} SHARED InteractorDeleteElement.cpp)
TARGET_LINK_LIBRARIES(InteractorDeleteElement-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL_TULIP_PLUGIN(InteractorNavigation-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorGetInformation-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorSelection-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorSelectionModifier-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorRectangleZoom-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorAddNodeEdge-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorEditEdgeBends-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorDeleteElement-${TulipVersion} ${TulipInteractorPluginsInstallDir})
