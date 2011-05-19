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
INSTALL(TARGETS 
  Eccentricity-${TulipVersion}
  DegreeMetric-${TulipVersion}
  ClusterMetric-${TulipVersion}
  StrengthMetric-${TulipVersion}
  BiconnectedComponent-${TulipVersion}
  ConnectedComponent-${TulipVersion}
  StrongComponent-${TulipVersion}
  DagLevelMetric-${TulipVersion}
  IdMetric-${TulipVersion}
  LeafMetric-${TulipVersion}
  NodeMetric-${TulipVersion}
  DepthMetric-${TulipVersion}
  PathLengthMetric-${TulipVersion}
  StrahlerMetric-${TulipVersion}
  RandomMetric-${TulipVersion}
  BetweennessCentrality-${TulipVersion}
DESTINATION ${TulipPluginsInstallDir}
COMPONENT tulip_core_plugins)
