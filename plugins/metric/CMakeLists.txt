ADD_CORE_FILES(.)

ADD_SUBDIRECTORY(Leiden)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Eccentricity-${Tulip_VERSION} SHARED Eccentricity.h Eccentricity.cpp)
TARGET_LINK_LIBRARIES(Eccentricity-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(DegreeMetric-${Tulip_VERSION} SHARED DegreeMetric.cpp DegreeMetric.h)
TARGET_LINK_LIBRARIES(DegreeMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ClusterMetric-${Tulip_VERSION} SHARED ClusterMetric.cpp ClusterMetric.h)
TARGET_LINK_LIBRARIES(ClusterMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(StrengthMetric-${Tulip_VERSION} SHARED StrengthMetric.cpp StrengthMetric.h)
TARGET_LINK_LIBRARIES(StrengthMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BiconnectedComponent-${Tulip_VERSION} SHARED BiconnectedComponent.cpp)
TARGET_LINK_LIBRARIES(BiconnectedComponent-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ConnectedComponent-${Tulip_VERSION} SHARED ConnectedComponent.cpp  ConnectedComponent.h)
TARGET_LINK_LIBRARIES(ConnectedComponent-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(StrongComponent-${Tulip_VERSION} SHARED StrongComponent.cpp StrongComponent.h)
TARGET_LINK_LIBRARIES(StrongComponent-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(DagLevelMetric-${Tulip_VERSION} SHARED DagLevelMetric.cpp DagLevelMetric.h)
TARGET_LINK_LIBRARIES(DagLevelMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(IdMetric-${Tulip_VERSION} SHARED IdMetric.cpp IdMetric.h)
TARGET_LINK_LIBRARIES(IdMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LeafMetric-${Tulip_VERSION} SHARED LeafMetric.cpp LeafMetric.h)
TARGET_LINK_LIBRARIES(LeafMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(NodeMetric-${Tulip_VERSION} SHARED NodeMetric.cpp NodeMetric.h)
TARGET_LINK_LIBRARIES(NodeMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(DepthMetric-${Tulip_VERSION} SHARED DepthMetric.cpp DepthMetric.h)
TARGET_LINK_LIBRARIES(DepthMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PathLengthMetric-${Tulip_VERSION} SHARED PathLengthMetric.cpp PathLengthMetric.h)
TARGET_LINK_LIBRARIES(PathLengthMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(StrahlerMetric-${Tulip_VERSION} SHARED StrahlerMetric.cpp StrahlerMetric.h)
TARGET_LINK_LIBRARIES(StrahlerMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(RandomMetric-${Tulip_VERSION} SHARED Random.cpp Random.h)
TARGET_LINK_LIBRARIES(RandomMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BetweennessCentrality-${Tulip_VERSION} SHARED BetweennessCentrality.cpp)
TARGET_LINK_LIBRARIES(BetweennessCentrality-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(KCores-${Tulip_VERSION} SHARED KCores.cpp)
TARGET_LINK_LIBRARIES(KCores-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LouvainClustering-${Tulip_VERSION} SHARED LouvainClustering.cpp)
TARGET_LINK_LIBRARIES(LouvainClustering-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SecondOrderCentrality-${Tulip_VERSION} SHARED SecondOrderCentrality.cpp)
TARGET_LINK_LIBRARIES(SecondOrderCentrality-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LinkCommunities-${Tulip_VERSION} SHARED LinkCommunities.cpp)
TARGET_LINK_LIBRARIES(LinkCommunities-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MCLClustering-${Tulip_VERSION} SHARED MCLClustering.cpp)
TARGET_LINK_LIBRARIES(MCLClustering-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PageRank-${Tulip_VERSION} SHARED PageRank.cpp)
TARGET_LINK_LIBRARIES(PageRank-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(WelshPowell-${Tulip_VERSION} SHARED WelshPowell.cpp)
TARGET_LINK_LIBRARIES(WelshPowell-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(NeighborhoodMetric-${Tulip_VERSION} SHARED NeighborhoodMetric.cpp)
TARGET_LINK_LIBRARIES(NeighborhoodMetric-${Tulip_VERSION} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Eccentricity-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DegreeMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ClusterMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(StrengthMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(BiconnectedComponent-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ConnectedComponent-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(StrongComponent-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DagLevelMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(IdMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LeafMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(NodeMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DepthMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(PathLengthMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(StrahlerMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(BetweennessCentrality-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(KCores-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LouvainClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LinkCommunities-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(MCLClustering-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(PageRank-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(WelshPowell-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(SecondOrderCentrality-${Tulip_VERSION} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(NeighborhoodMetric-${Tulip_VERSION} ${TulipPluginsInstallDir})
