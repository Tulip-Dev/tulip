INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIR})

SET(LIB_SRCS
    FishEyeInteractor.cpp 
    FishEyeConfigWidget.cpp
)

QTX_WRAP_UI(LIB_UI_HEADERS
    FishEyeConfigWidget.ui  
)

QTX_WRAP_CPP(LIB_MOC_SRCS
)

QTX_ADD_RESOURCES(LIB_RCC_SRCS
    FishEyeInteractorResource.qrc
)

ADD_LIBRARY(FishEyeInteractor-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(FishEyeInteractor-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

INSTALL(TARGETS FishEyeInteractor-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT FishEyeInteractor)
