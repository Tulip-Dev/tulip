INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

SET(MetricColorMapping_SRCS
    MetricMapping.cpp
)

ADD_LIBRARY(MetricColorMapping-${TulipVersion} SHARED ${MetricColorMapping_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ MetricColorMapping-${TulipVersion})

SET(EnumColorValuesMapping_SRCS
    EnumColorValuesMapping.cpp
)

ADD_LIBRARY(EnumColorValuesMapping-${TulipVersion} SHARED ${EnumColorValuesMapping_SRCS})
INSTALL_TARGETS(${TulipLibInstallDir}tlp/ EnumColorValuesMapping-${TulipVersion})
