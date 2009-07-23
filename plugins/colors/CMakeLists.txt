INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(MetricColorMapping_SRCS
    MetricMapping.cpp
)

ADD_LIBRARY(MetricColorMapping-${TulipVersion} SHARED ${MetricColorMapping_SRCS})
TARGET_LINK_LIBRARIES(MetricColorMapping-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(MetricColorMapping-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(MetricColorMapping-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(MetricColorMapping-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
SET(EnumColorValuesMapping_SRCS
    EnumColorValuesMapping.cpp
)

ADD_LIBRARY(EnumColorValuesMapping-${TulipVersion} SHARED ${EnumColorValuesMapping_SRCS})
TARGET_LINK_LIBRARIES(EnumColorValuesMapping-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(EnumColorValuesMapping-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(EnumColorValuesMapping-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(EnumColorValuesMapping-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	MetricColorMapping-${TulipVersion}	
	EnumColorValuesMapping-${TulipVersion}
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
