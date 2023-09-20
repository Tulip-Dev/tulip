ADD_CORE_FILES(".")

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${CMAKE_CURRENT_BINARY_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(Planarity_SRCS
    Planarity.cpp
)

ADD_LIBRARY(Planarity-${Tulip_VERSION} SHARED ${Planarity_SRCS})
TARGET_LINK_LIBRARIES(Planarity-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Planarity-${Tulip_VERSION} ${TulipPluginsInstallDir})


##----------------------------------------------------------------------------------------------------------------------------
SET(Simple_SRCS
Simple.cpp
)

ADD_LIBRARY(Simple-${Tulip_VERSION} SHARED ${Simple_SRCS})
TARGET_LINK_LIBRARIES(Simple-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Simple-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
SET(Tree_SRCS
Tree.cpp
)

ADD_LIBRARY(Tree-${Tulip_VERSION} SHARED ${Tree_SRCS})
TARGET_LINK_LIBRARIES(Tree-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Tree-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
SET(Connected_SRCS
Connected.cpp
)

ADD_LIBRARY(Connected-${Tulip_VERSION} SHARED ${Connected_SRCS})
TARGET_LINK_LIBRARIES(Connected-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Connected-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
SET(Biconnected_SRCS
Biconnected.cpp
)

ADD_LIBRARY(Biconnected-${Tulip_VERSION} SHARED ${Biconnected_SRCS})
TARGET_LINK_LIBRARIES(Biconnected-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Biconnected-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
SET(Triconnected_SRCS
Triconnected.cpp
)

ADD_LIBRARY(Triconnected-${Tulip_VERSION} SHARED ${Triconnected_SRCS})
TARGET_LINK_LIBRARIES(Triconnected-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Triconnected-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
SET(Outerplanar_SRCS
Outerplanar.cpp
)

ADD_LIBRARY(Outerplanar-${Tulip_VERSION} SHARED ${Outerplanar_SRCS})
TARGET_LINK_LIBRARIES(Outerplanar-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Outerplanar-${Tulip_VERSION} ${TulipPluginsInstallDir})

##----------------------------------------------------------------------------------------------------------------------------
SET(Acyclic_SRCS
Acyclic.cpp
)

ADD_LIBRARY(Acyclic-${Tulip_VERSION} SHARED ${Acyclic_SRCS})
TARGET_LINK_LIBRARIES(Acyclic-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Acyclic-${Tulip_VERSION} ${TulipPluginsInstallDir})
