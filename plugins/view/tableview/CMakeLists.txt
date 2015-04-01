INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

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

TARGET_LINK_LIBRARIES(TableView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS TableView-${TulipVersion} DESTINATION ${TulipViewPluginsInstallDir} COMPONENT TableView)

