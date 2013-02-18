INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude})

SET(LIB_SRCS
    FishEyeInteractor.cpp 
    FishEyeConfigWidget.cpp
)

QT4_WRAP_UI(LIB_UI_HEADERS
    FishEyeConfigWidget.ui  
)

QT4_WRAP_CPP(LIB_MOC_SRCS
)

QT4_ADD_RESOURCES(LIB_RCC_SRCS
    FishEyeInteractorResource.qrc
)

ADD_LIBRARY(FishEyeInteractor-${TulipVersion} SHARED ${LIB_SRCS} ${LIB_UI_HEADERS} ${LIB_MOC_SRCS} ${LIB_RCC_SRCS})
TARGET_LINK_LIBRARIES(FishEyeInteractor-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName})

INSTALL(TARGETS FishEyeInteractor-${TulipVersion} DESTINATION ${TulipInteractorPluginsInstallDir} COMPONENT FishEyeInteractor)
