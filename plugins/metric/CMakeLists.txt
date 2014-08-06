ADD_CORE_FILES(.)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude})

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
ADD_LIBRARY(RandomMetric-${TulipVersion} SHARED Random.cpp  Random.h)
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
INSTALL(TARGETS Eccentricity-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT Eccentricity)
INSTALL(TARGETS DegreeMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT DegreeMetric)
INSTALL(TARGETS ClusterMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ClusterMetric)
INSTALL(TARGETS StrengthMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrengthMetric)
INSTALL(TARGETS BiconnectedComponent-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT BiconnectedComponent)
INSTALL(TARGETS ConnectedComponent-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT ConnectedComponent)
INSTALL(TARGETS StrongComponent-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrongComponent)
INSTALL(TARGETS DagLevelMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT DagLevelMetric)
INSTALL(TARGETS IdMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT IdMetric)
INSTALL(TARGETS LeafMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT LeafMetric)
INSTALL(TARGETS NodeMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT NodeMetric)
INSTALL(TARGETS DepthMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT DepthMetric)
INSTALL(TARGETS PathLengthMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT PathLengthMetric)
INSTALL(TARGETS StrahlerMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT StrahlerMetric)
INSTALL(TARGETS RandomMetric-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT RandomMetric)
INSTALL(TARGETS BetweennessCentrality-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT BetweennessCentrality)
INSTALL(TARGETS KCores-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT KCores)
INSTALL(TARGETS LouvainClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT LouvainClustering)
INSTALL(TARGETS LinkCommunities-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT LinkCommunities)
INSTALL(TARGETS MCLClustering-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT MCLClustering)
INSTALL(TARGETS PageRank-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT PageRank)
INSTALL(TARGETS WelshPowell-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT WelshPowell)
