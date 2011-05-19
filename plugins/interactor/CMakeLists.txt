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
ADD_SUBDIRECTORY(MagicWand)

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorRectangleZoom-${TulipVersion} SHARED InteractorRectangleZoom.cpp)
TARGET_LINK_LIBRARIES(InteractorRectangleZoom-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorDeleteElement-${TulipVersion} SHARED InteractorDeleteElement.cpp)
TARGET_LINK_LIBRARIES(InteractorDeleteElement-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddNode-${TulipVersion} SHARED InteractorAddNode.cpp)
TARGET_LINK_LIBRARIES(InteractorAddNode-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorAddEdge-${TulipVersion} SHARED InteractorAddEdge.cpp)
TARGET_LINK_LIBRARIES(InteractorAddEdge-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(InteractorEditEdgeBends-${TulipVersion} SHARED InteractorEditEdgeBends.cpp)
TARGET_LINK_LIBRARIES(InteractorEditEdgeBends-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
  InteractorNavigation-${TulipVersion}
  InteractorGetInformation-${TulipVersion}
  InteractorSelection-${TulipVersion}
  InteractorSelectionModifier-${TulipVersion}
  InteractorRectangleZoom-${TulipVersion}
  InteractorDeleteElement-${TulipVersion}
  InteractorAddNode-${TulipVersion}
  InteractorAddEdge-${TulipVersion}
  InteractorEditEdgeBends-${TulipVersion}
DESTINATION ${TulipPluginsInstallDir}/interactors/
COMPONENT tulip_gui_plugins)
