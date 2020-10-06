ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(Eccentricity-${TulipVersion} SHARED Eccentricity.h Eccentricity.cpp)
TARGET_LINK_LIBRARIES(Eccentricity-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(DegreeMetric-${TulipVersion} SHARED DegreeMetric.cpp DegreeMetric.h)
TARGET_LINK_LIBRARIES(DegreeMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ClusterMetric-${TulipVersion} SHARED ClusterMetric.cpp ClusterMetric.h)
TARGET_LINK_LIBRARIES(ClusterMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(StrengthMetric-${TulipVersion} SHARED StrengthMetric.cpp StrengthMetric.h)
TARGET_LINK_LIBRARIES(StrengthMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BiconnectedComponent-${TulipVersion} SHARED BiconnectedComponent.cpp)
TARGET_LINK_LIBRARIES(BiconnectedComponent-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(ConnectedComponent-${TulipVersion} SHARED ConnectedComponent.cpp  ConnectedComponent.h)
TARGET_LINK_LIBRARIES(ConnectedComponent-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(StrongComponent-${TulipVersion} SHARED StrongComponent.cpp StrongComponent.h)
TARGET_LINK_LIBRARIES(StrongComponent-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(DagLevelMetric-${TulipVersion} SHARED DagLevelMetric.cpp DagLevelMetric.h)
TARGET_LINK_LIBRARIES(DagLevelMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(IdMetric-${TulipVersion} SHARED IdMetric.cpp IdMetric.h)
TARGET_LINK_LIBRARIES(IdMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LeafMetric-${TulipVersion} SHARED LeafMetric.cpp LeafMetric.h)
TARGET_LINK_LIBRARIES(LeafMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(NodeMetric-${TulipVersion} SHARED NodeMetric.cpp NodeMetric.h)
TARGET_LINK_LIBRARIES(NodeMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(DepthMetric-${TulipVersion} SHARED DepthMetric.cpp DepthMetric.h)
TARGET_LINK_LIBRARIES(DepthMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PathLengthMetric-${TulipVersion} SHARED PathLengthMetric.cpp PathLengthMetric.h)
TARGET_LINK_LIBRARIES(PathLengthMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(StrahlerMetric-${TulipVersion} SHARED StrahlerMetric.cpp StrahlerMetric.h)
TARGET_LINK_LIBRARIES(StrahlerMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(RandomMetric-${TulipVersion} SHARED Random.cpp Random.h)
TARGET_LINK_LIBRARIES(RandomMetric-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(BetweennessCentrality-${TulipVersion} SHARED BetweennessCentrality.cpp)
TARGET_LINK_LIBRARIES(BetweennessCentrality-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(KCores-${TulipVersion} SHARED KCores.cpp)
TARGET_LINK_LIBRARIES(KCores-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LouvainClustering-${TulipVersion} SHARED LouvainClustering.cpp)
TARGET_LINK_LIBRARIES(LouvainClustering-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(SecondOrderCentrality-${TulipVersion} SHARED SecondOrderCentrality.cpp)
TARGET_LINK_LIBRARIES(SecondOrderCentrality-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(LinkCommunities-${TulipVersion} SHARED LinkCommunities.cpp)
TARGET_LINK_LIBRARIES(LinkCommunities-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(MCLClustering-${TulipVersion} SHARED MCLClustering.cpp)
TARGET_LINK_LIBRARIES(MCLClustering-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(PageRank-${TulipVersion} SHARED PageRank.cpp)
TARGET_LINK_LIBRARIES(PageRank-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
ADD_LIBRARY(WelshPowell-${TulipVersion} SHARED WelshPowell.cpp)
TARGET_LINK_LIBRARIES(WelshPowell-${TulipVersion} ${LibTulipCoreName})

##----------------------------------------------------------------------------------------------------------------------------
TULIP_INSTALL_PLUGIN(Eccentricity-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DegreeMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ClusterMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(StrengthMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(BiconnectedComponent-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(ConnectedComponent-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(StrongComponent-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DagLevelMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(IdMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LeafMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(NodeMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(DepthMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(PathLengthMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(StrahlerMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(RandomMetric-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(BetweennessCentrality-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(KCores-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LouvainClustering-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(LinkCommunities-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(MCLClustering-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(PageRank-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(WelshPowell-${TulipVersion} ${TulipPluginsInstallDir})
TULIP_INSTALL_PLUGIN(SecondOrderCentrality-${TulipVersion} ${TulipPluginsInstallDir})
