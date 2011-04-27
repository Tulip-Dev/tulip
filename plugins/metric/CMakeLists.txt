INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(Eccentricity_SRCS Eccentricity.h Eccentricity.cpp)
ADD_LIBRARY(Eccentricity-${TulipVersion} SHARED ${Eccentricity_SRCS})
TARGET_LINK_LIBRARIES(Eccentricity-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(DegreeMetric_SRCS DegreeMetric.cpp DegreeMetric.h)
ADD_LIBRARY(DegreeMetric-${TulipVersion} SHARED ${DegreeMetric_SRCS})
TARGET_LINK_LIBRARIES(DegreeMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ClusterMetric_SRCS ClusterMetric.cpp ClusterMetric.h)
ADD_LIBRARY(ClusterMetric-${TulipVersion} SHARED ${ClusterMetric_SRCS})
TARGET_LINK_LIBRARIES(ClusterMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(StrengthMetric_SRCS StrengthMetric.cpp StrengthMetric.h)
ADD_LIBRARY(StrengthMetric-${TulipVersion} SHARED ${StrengthMetric_SRCS})
TARGET_LINK_LIBRARIES(StrengthMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(BiconnectedComponent_SRCS BiconnectedComponent.cpp)
ADD_LIBRARY(BiconnectedComponent-${TulipVersion} SHARED ${BiconnectedComponent_SRCS})
TARGET_LINK_LIBRARIES(BiconnectedComponent-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConnectedComponent_SRCS ConnectedComponent.cpp  ConnectedComponent.h)
ADD_LIBRARY(ConnectedComponent-${TulipVersion} SHARED ${ConnectedComponent_SRCS})
TARGET_LINK_LIBRARIES(ConnectedComponent-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(StrongComponent_SRCS StrongComponent.cpp StrongComponent.h)
ADD_LIBRARY(StrongComponent-${TulipVersion} SHARED ${StrongComponent_SRCS})
TARGET_LINK_LIBRARIES(StrongComponent-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(DagLevelMetric_SRCS DagLevelMetric.cpp DagLevelMetric.h)
ADD_LIBRARY(DagLevelMetric-${TulipVersion} SHARED ${DagLevelMetric_SRCS})
TARGET_LINK_LIBRARIES(DagLevelMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(IdMetric_SRCS IdMetric.cpp IdMetric.h)
ADD_LIBRARY(IdMetric-${TulipVersion} SHARED ${IdMetric_SRCS})
TARGET_LINK_LIBRARIES(IdMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(LeafMetric_SRCS LeafMetric.cpp LeafMetric.h)
ADD_LIBRARY(LeafMetric-${TulipVersion} SHARED ${LeafMetric_SRCS})
TARGET_LINK_LIBRARIES(LeafMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(NodeMetric_SRCS NodeMetric.cpp NodeMetric.h)
ADD_LIBRARY(NodeMetric-${TulipVersion} SHARED ${NodeMetric_SRCS})
TARGET_LINK_LIBRARIES(NodeMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(DepthMetric_SRCS DepthMetric.cpp DepthMetric.h)
ADD_LIBRARY(DepthMetric-${TulipVersion} SHARED ${DepthMetric_SRCS})
TARGET_LINK_LIBRARIES(DepthMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(PathLengthMetric_SRCS PathLengthMetric.cpp PathLengthMetric.h)
ADD_LIBRARY(PathLengthMetric-${TulipVersion} SHARED ${PathLengthMetric_SRCS})
TARGET_LINK_LIBRARIES(PathLengthMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(StrahlerMetric_SRCS StrahlerMetric.cpp StrahlerMetric.h)
ADD_LIBRARY(StrahlerMetric-${TulipVersion} SHARED ${StrahlerMetric_SRCS})
TARGET_LINK_LIBRARIES(StrahlerMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(RandomMetric_SRCS Random.cpp  Random.h)
ADD_LIBRARY(RandomMetric-${TulipVersion} SHARED ${RandomMetric_SRCS})
TARGET_LINK_LIBRARIES(RandomMetric-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(BetweennessCentrality_SRCS BetweennessCentrality.cpp)
ADD_LIBRARY(BetweennessCentrality-${TulipVersion} SHARED ${BetweennessCentrality_SRCS})
TARGET_LINK_LIBRARIES(BetweennessCentrality-${TulipVersion} ${LibTulipName})

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
  DESTINATION ${TulipPluginsInstallDir}/
  COMPONENT libtulip_plugins
  )
