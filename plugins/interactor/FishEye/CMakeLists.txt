<<<<<<< HEAD
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIR})
=======
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${GLEW_INCLUDE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

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
TARGET_LINK_LIBRARIES(FishEyeInteractor-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

INSTALL_TULIP_PLUGIN(FishEyeInteractor-${TulipVersion} ${TulipInteractorPluginsInstallDir})
