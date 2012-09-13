INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIB_SRCS
TableView.cpp
TableViewConfiguration.cpp
PropertiesEditor.cpp
navigabletableview.cpp
)

QT4_WRAP_UI(UI_SRCS
AddPropertyDialog.ui
TableView.ui
PropertiesEditor.ui
TableViewConfiguration.ui
)

QT4_WRAP_CPP(MOC_SRCS
TableView.h
TableViewConfiguration.h
PropertiesEditor.h
navigabletableview.h
)

ADD_LIBRARY(TableView-${TulipVersion} SHARED ${LIB_SRCS} ${MOC_SRCS} ${UI_SRCS})

TARGET_LINK_LIBRARIES(TableView-${TulipVersion} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName})

INSTALL(TARGETS TableView-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT TableView)

