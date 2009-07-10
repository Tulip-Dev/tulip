INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

SET(Eccentricity_SRCS Eccentricity.h Eccentricity.cpp)
ADD_LIBRARY(Eccentricity-${TulipVersion} SHARED ${Eccentricity_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ Eccentricity-${TulipVersion})

SET(ArityMetric_SRCS ArityMetric.cpp ArityMetric.h)
ADD_LIBRARY(ArityMetric-${TulipVersion} SHARED ${ArityMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ArityMetric-${TulipVersion})

SET(ClusterMetric_SRCS ClusterMetric.cpp ClusterMetric.h)
ADD_LIBRARY(ClusterMetric-${TulipVersion} SHARED ${ClusterMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ClusterMetric-${TulipVersion})

SET(StrengthMetric_SRCS StrengthMetric.cpp StrengthMetric.h)
ADD_LIBRARY(StrengthMetric-${TulipVersion} SHARED ${StrengthMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ StrengthMetric-${TulipVersion})

SET(BiconnectedComponent_SRCS BiconnectedComponent.cpp)
ADD_LIBRARY(BiconnectedComponent-${TulipVersion} SHARED ${BiconnectedComponent_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ BiconnectedComponent-${TulipVersion})

SET(ConnectedComponent_SRCS ConnectedComponent.cpp  ConnectedComponent.h)
ADD_LIBRARY(ConnectedComponent-${TulipVersion} SHARED ${ConnectedComponent_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ ConnectedComponent-${TulipVersion})

SET(StrongComponent_SRCS StrongComponent.cpp StrongComponent.h)
ADD_LIBRARY(StrongComponent-${TulipVersion} SHARED ${StrongComponent_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ StrongComponent-${TulipVersion})

SET(DagLevelMetric_SRCS DagLevelMetric.cpp DagLevelMetric.h)
ADD_LIBRARY(DagLevelMetric-${TulipVersion} SHARED ${DagLevelMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ DagLevelMetric-${TulipVersion})

SET(IdMetric_SRCS IdMetric.cpp IdMetric.h)
ADD_LIBRARY(IdMetric-${TulipVersion} SHARED ${IdMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ IdMetric-${TulipVersion})

SET(LeafMetric_SRCS LeafMetric.cpp LeafMetric.h)
ADD_LIBRARY(LeafMetric-${TulipVersion} SHARED ${LeafMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ LeafMetric-${TulipVersion})

SET(NodeMetric_SRCS NodeMetric.cpp NodeMetric.h)
ADD_LIBRARY(NodeMetric-${TulipVersion} SHARED ${NodeMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ NodeMetric-${TulipVersion})

SET(DepthMetric_SRCS DepthMetric.cpp DepthMetric.h)
ADD_LIBRARY(DepthMetric-${TulipVersion} SHARED ${DepthMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ DepthMetric-${TulipVersion})

SET(PathLengthMetric_SRCS PathLengthMetric.cpp PathLengthMetric.h)
ADD_LIBRARY(PathLengthMetric-${TulipVersion} SHARED ${PathLengthMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ PathLengthMetric-${TulipVersion})

SET(StrahlerMetric_SRCS StrahlerMetric.cpp StrahlerMetric.h)
ADD_LIBRARY(StrahlerMetric-${TulipVersion} SHARED ${StrahlerMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ StrahlerMetric-${TulipVersion})

SET(RandomMetric_SRCS Random.cpp  Random.h)
ADD_LIBRARY(RandomMetric-${TulipVersion} SHARED ${RandomMetric_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ RandomMetric-${TulipVersion})

SET(BetweennessCentrality_SRCS BetweennessCentrality.cpp)
ADD_LIBRARY(BetweennessCentrality-${TulipVersion} SHARED ${BetweennessCentrality_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ BetweennessCentrality-${TulipVersion})


