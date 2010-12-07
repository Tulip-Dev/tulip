FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")	

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${CMAKE_CURRENT_BINARY_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(StrengthClustering_SRCS
    StrengthClustering.h
    StrengthClustering.cpp
)

ADD_LIBRARY(StrengthClustering-${TulipVersion} SHARED ${StrengthClustering_SRCS})
TARGET_LINK_LIBRARIES(StrengthClustering-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(StrengthClustering-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(HierarchicalClustering_SRCS
    HierarchicalClustering.cpp  
    HierarchicalClustering.h
)

ADD_LIBRARY(HierarchicalClustering-${TulipVersion} SHARED ${HierarchicalClustering_SRCS})
TARGET_LINK_LIBRARIES(HierarchicalClustering-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(HierarchicalClustering-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(ConvolutionClustering_UI
    ConvolutionClusteringSetup.ui
)

SET(ConvolutionClustering_MOC
    ConvolutionClusteringSetup.h
)

SET(ConvolutionClustering_SRCS
    ConvolutionClustering.h
    ConvolutionClustering.cpp
    ConvolutionClusteringSetup.cpp
    ConvolutionClusteringSetup.h
)

TULIP_QT4_WRAP_UI(ConvolutionClustering_UI_SRCS ${ConvolutionClustering_UI})
TULIP_QT4_WRAP_CPP(ConvolutionClustering_MOC_SRCS ${ConvolutionClustering_MOC})

ADD_LIBRARY(ConvolutionClustering-${TulipVersion} SHARED ${ConvolutionClustering_SRCS} ${ConvolutionClustering_UI_SRCS} ${ConvolutionClustering_MOC_SRCS})
TARGET_LINK_LIBRARIES(ConvolutionClustering-${TulipVersion} ${LibTulipName} )
TARGET_LINK_LIBRARIES(ConvolutionClustering-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(ConvolutionClustering-${TulipVersion} ${QT_QTGUI_LIBRARY})
ADD_DEPENDENCIES(ConvolutionClustering-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(EqualValueClustering_SRCS
    EqualValueClustering.h
    EqualValueClustering.cpp
)

ADD_LIBRARY(EqualValueClustering-${TulipVersion} SHARED ${EqualValueClustering_SRCS})
TARGET_LINK_LIBRARIES(EqualValueClustering-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(EqualValueClustering-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
SET(QuotientClustering_SRCS
    QuotientClustering.h
    QuotientClustering.cpp
)

ADD_LIBRARY(QuotientClustering-${TulipVersion} SHARED ${QuotientClustering_SRCS})
TARGET_LINK_LIBRARIES(QuotientClustering-${TulipVersion} ${LibTulipName})
ADD_DEPENDENCIES(QuotientClustering-${TulipVersion} ${LibTulipName})

##----------------------------------------------------------------------------------------------------------------------------
INSTALL(TARGETS StrengthClustering-${TulipVersion} 
		HierarchicalClustering-${TulipVersion}
		ConvolutionClustering-${TulipVersion}
		EqualValueClustering-${TulipVersion}
		QuotientClustering-${TulipVersion}
    DESTINATION ${TulipLibInstallDir}tlp/
    COMPONENT libtulip_plugins)
