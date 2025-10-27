INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

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

ADD_LIBRARY(TableView-${Tulip_VERSION} SHARED ${LIB_SRCS} ${MOC_SRCS} ${UI_SRCS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(TableView-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

TULIP_INSTALL_PLUGIN(TableView-${Tulip_VERSION} ${TulipViewPluginsInstallDir})

