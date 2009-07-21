INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(MetricColorMapping_SRCS
    MetricMapping.cpp
)

ADD_LIBRARY(MetricColorMapping-${TulipVersion} SHARED ${MetricColorMapping_SRCS})
TARGET_LINK_LIBRARIES(MetricColorMapping-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(MetricColorMapping-${TulipVersion} tulip-ogl)
TARGET_LINK_LIBRARIES(MetricColorMapping-${TulipVersion} tulip-qt4)
ADD_DEPENDENCIES(MetricColorMapping-${TulipVersion} tulip tulip-ogl tulip-qt4)

##----------------------------------------------------------------------------------------------------------------------------
SET(EnumColorValuesMapping_SRCS
    EnumColorValuesMapping.cpp
)

ADD_LIBRARY(EnumColorValuesMapping-${TulipVersion} SHARED ${EnumColorValuesMapping_SRCS})
TARGET_LINK_LIBRARIES(EnumColorValuesMapping-${TulipVersion} tulip)
TARGET_LINK_LIBRARIES(EnumColorValuesMapping-${TulipVersion} tulip-ogl)
TARGET_LINK_LIBRARIES(EnumColorValuesMapping-${TulipVersion} tulip-qt4)
ADD_DEPENDENCIES(EnumColorValuesMapping-${TulipVersion} tulip tulip-ogl tulip-qt4)

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS 
	MetricColorMapping-${TulipVersion}	
	EnumColorValuesMapping-${TulipVersion}
	DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/)
