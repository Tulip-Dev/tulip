<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder

SET(LIB_SRCS
TableView.cpp
PropertiesEditor.cpp
navigabletableview.cpp
)

QTX_WRAP_UI(UI_SRCS
TableView.ui
PropertiesEditor.ui
)

QTX_WRAP_CPP(MOC_SRCS
TableView.h
PropertiesEditor.h
navigabletableview.h
)

QTX_ADD_RESOURCES(RCC_SRCS TableView.qrc)

ADD_LIBRARY(TableView-${TulipVersion} SHARED ${LIB_SRCS} ${MOC_SRCS} ${UI_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(TableView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLESName} ${LibTulipGUIName})

INSTALL_TULIP_PLUGIN(TableView-${TulipVersion} ${TulipViewPluginsInstallDir})

