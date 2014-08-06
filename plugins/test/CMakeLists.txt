ADD_CORE_FILES(".")

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${CMAKE_CURRENT_BINARY_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(Planarity_SRCS
    Planarity.cpp
)

ADD_LIBRARY(Planarity-${TulipVersion} SHARED ${Planarity_SRCS})
TARGET_LINK_LIBRARIES(Planarity-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Planarity-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)


##----------------------------------------------------------------------------------------------------------------------------
SET(Simple_SRCS
Simple.cpp
)

ADD_LIBRARY(Simple-${TulipVersion} SHARED ${Simple_SRCS})
TARGET_LINK_LIBRARIES(Simple-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Simple-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)

##----------------------------------------------------------------------------------------------------------------------------
SET(Tree_SRCS
Tree.cpp
)

ADD_LIBRARY(Tree-${TulipVersion} SHARED ${Tree_SRCS})
TARGET_LINK_LIBRARIES(Tree-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Tree-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)

##----------------------------------------------------------------------------------------------------------------------------
SET(Connected_SRCS
Connected.cpp
)

ADD_LIBRARY(Connected-${TulipVersion} SHARED ${Connected_SRCS})
TARGET_LINK_LIBRARIES(Connected-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Connected-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)

##----------------------------------------------------------------------------------------------------------------------------
SET(Biconnected_SRCS
Biconnected.cpp
)

ADD_LIBRARY(Biconnected-${TulipVersion} SHARED ${Biconnected_SRCS})
TARGET_LINK_LIBRARIES(Biconnected-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Biconnected-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)

##----------------------------------------------------------------------------------------------------------------------------
SET(Triconnected_SRCS
Triconnected.cpp
)

ADD_LIBRARY(Triconnected-${TulipVersion} SHARED ${Triconnected_SRCS})
TARGET_LINK_LIBRARIES(Triconnected-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Triconnected-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)

##----------------------------------------------------------------------------------------------------------------------------
SET(Outerplanar_SRCS
Outerplanar.cpp
)

ADD_LIBRARY(Outerplanar-${TulipVersion} SHARED ${Outerplanar_SRCS})
TARGET_LINK_LIBRARIES(Outerplanar-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Outerplanar-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)

##----------------------------------------------------------------------------------------------------------------------------
SET(Acyclic_SRCS
Acyclic.cpp
)

ADD_LIBRARY(Acyclic-${TulipVersion} SHARED ${Acyclic_SRCS})
TARGET_LINK_LIBRARIES(Acyclic-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS Acyclic-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthClustering)