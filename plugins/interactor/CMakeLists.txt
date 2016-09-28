ADD_SUBDIRECTORY(FishEye)
#ADD_SUBDIRECTORY(MouseMagnifyingGlass)
ADD_SUBDIRECTORY(MouseLassoNodesSelector)
ADD_SUBDIRECTORY(NeighborhoodHighlighter)
#ADD_SUBDIRECTORY(PathFinder)

<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude})
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorNavigation-${TulipVersion} SHARED InteractorNavigation.cpp)
TARGET_LINK_LIBRARIES(InteractorNavigation-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorGetInformation-${TulipVersion} SHARED InteractorGetInformation.cpp)
TARGET_LINK_LIBRARIES(InteractorGetInformation-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorSelection-${TulipVersion} SHARED InteractorSelection.cpp)
TARGET_LINK_LIBRARIES(InteractorSelection-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
#ADD_LIBRARY(InteractorSelectionModifier-${TulipVersion} SHARED InteractorSelectionModifier.cpp)
#TARGET_LINK_LIBRARIES(InteractorSelectionModifier-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorRectangleZoom-${TulipVersion} SHARED InteractorRectangleZoom.cpp)
TARGET_LINK_LIBRARIES(InteractorRectangleZoom-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddNodeEdge-${TulipVersion} SHARED InteractorAddEdge.cpp)
TARGET_LINK_LIBRARIES(InteractorAddNodeEdge-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
#ADD_LIBRARY(InteractorEditEdgeBends-${TulipVersion} SHARED InteractorEditEdgeBends.cpp)
#TARGET_LINK_LIBRARIES(InteractorEditEdgeBends-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

###----------------------------------------------------------------------------------------------------------------------------
#ADD_LIBRARY(InteractorDeleteElement-${TulipVersion} SHARED InteractorDeleteElement.cpp)
#TARGET_LINK_LIBRARIES(InteractorDeleteElement-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL_TULIP_PLUGIN(InteractorNavigation-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorGetInformation-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorSelection-${TulipVersion} ${TulipInteractorPluginsInstallDir})
#INSTALL_TULIP_PLUGIN(InteractorSelectionModifier-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorRectangleZoom-${TulipVersion} ${TulipInteractorPluginsInstallDir})
INSTALL_TULIP_PLUGIN(InteractorAddNodeEdge-${TulipVersion} ${TulipInteractorPluginsInstallDir})
#INSTALL_TULIP_PLUGIN(InteractorEditEdgeBends-${TulipVersion} ${TulipInteractorPluginsInstallDir})
#INSTALL_TULIP_PLUGIN(InteractorDeleteElement-${TulipVersion} ${TulipInteractorPluginsInstallDir})
