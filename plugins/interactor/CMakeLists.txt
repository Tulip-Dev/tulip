INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorNavigation_SRCS
    InteractorNavigation.cpp
)

ADD_LIBRARY(InteractorNavigation-${TulipVersion} SHARED ${InteractorNavigation_SRCS})
TARGET_LINK_LIBRARIES(InteractorNavigation-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorNavigation-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorGetInformation_SRCS
    InteractorGetInformation.cpp
)

ADD_LIBRARY(InteractorGetInformation-${TulipVersion} SHARED ${InteractorGetInformation_SRCS})
TARGET_LINK_LIBRARIES(InteractorGetInformation-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorGetInformation-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorSelection_SRCS
    InteractorSelection.cpp
)

ADD_LIBRARY(InteractorSelection-${TulipVersion} SHARED ${InteractorSelection_SRCS})
TARGET_LINK_LIBRARIES(InteractorSelection-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorSelection-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorSelectionModifier_SRCS
    InteractorSelectionModifier.cpp
)

ADD_LIBRARY(InteractorSelectionModifier-${TulipVersion} SHARED ${InteractorSelectionModifier_SRCS})
TARGET_LINK_LIBRARIES(InteractorSelectionModifier-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorSelectionModifier-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(MagicWand)

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorRectangleZoom_SRCS
    InteractorRectangleZoom.cpp
)

ADD_LIBRARY(InteractorRectangleZoom-${TulipVersion} SHARED ${InteractorRectangleZoom_SRCS})
TARGET_LINK_LIBRARIES(InteractorRectangleZoom-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorRectangleZoom-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorDeleteElement_SRCS
    InteractorDeleteElement.cpp
)

ADD_LIBRARY(InteractorDeleteElement-${TulipVersion} SHARED ${InteractorDeleteElement_SRCS})
TARGET_LINK_LIBRARIES(InteractorDeleteElement-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorDeleteElement-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorAddNode_SRCS
    InteractorAddNode.cpp
)

ADD_LIBRARY(InteractorAddNode-${TulipVersion} SHARED ${InteractorAddNode_SRCS})
TARGET_LINK_LIBRARIES(InteractorAddNode-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorAddNode-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorAddEdge_SRCS
    InteractorAddEdge.cpp
)

ADD_LIBRARY(InteractorAddEdge-${TulipVersion} SHARED ${InteractorAddEdge_SRCS})
TARGET_LINK_LIBRARIES(InteractorAddEdge-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorAddEdge-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(InteractorEditEdgeBends_SRCS
    InteractorEditEdgeBends.cpp
)

ADD_LIBRARY(InteractorEditEdgeBends-${TulipVersion} SHARED ${InteractorEditEdgeBends_SRCS})
TARGET_LINK_LIBRARIES(InteractorEditEdgeBends-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})
ADD_DEPENDENCIES(InteractorEditEdgeBends-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

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
        DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/interactors/)
