QTX_SET_INCLUDES_AND_DEFINITIONS()
ADD_SUBDIRECTORY(FishEye)
ADD_SUBDIRECTORY(MouseMagnifyingGlass)
ADD_SUBDIRECTORY(MouseLassoNodesSelector)
ADD_SUBDIRECTORY(NeighborhoodHighlighter)
ADD_SUBDIRECTORY(PathFinder)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorGetInformation-${Tulip_VERSION} SHARED InteractorGetInformation.cpp)
TARGET_LINK_LIBRARIES(InteractorGetInformation-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelection-${Tulip_VERSION} SHARED InteractorSelection.cpp)
TARGET_LINK_LIBRARIES(InteractorSelection-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelectionModifier-${Tulip_VERSION} SHARED InteractorSelectionModifier.cpp)
TARGET_LINK_LIBRARIES(InteractorSelectionModifier-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorRectangleZoom-${Tulip_VERSION} SHARED InteractorRectangleZoom.cpp)
TARGET_LINK_LIBRARIES(InteractorRectangleZoom-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddNodeEdge-${Tulip_VERSION} SHARED InteractorAddEdge.cpp)
TARGET_LINK_LIBRARIES(InteractorAddNodeEdge-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorEditEdgeBends-${Tulip_VERSION} SHARED InteractorEditEdgeBends.cpp)
TARGET_LINK_LIBRARIES(InteractorEditEdgeBends-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorDeleteElement-${Tulip_VERSION} SHARED InteractorDeleteElement.cpp)
TARGET_LINK_LIBRARIES(InteractorDeleteElement-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(InteractorGetInformation-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InteractorSelection-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InteractorSelectionModifier-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InteractorRectangleZoom-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InteractorAddNodeEdge-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InteractorEditEdgeBends-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
TULIP_INSTALL_PLUGIN(InteractorDeleteElement-${Tulip_VERSION} ${TulipInteractorPluginsInstallDir})
