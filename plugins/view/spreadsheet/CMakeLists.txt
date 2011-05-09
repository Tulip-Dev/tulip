INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${CMAKE_CURRENT_BINARY_DIR} .)	

SET(SRCS
    SpreadView.h
    SpreadView.cpp 
    SpreadWidget.cpp 
    SpreadTable.cpp
    SpreadCell.cpp
    SpreadCalculator.cpp
    SpreadValue.cpp
    SpreadItemDelegate.cpp
    SpreadCommands.cpp
    SpreadConfigurationWidget.cpp
    SpreadWidget.h
    SpreadTable.h
    SpreadCell.h
    SpreadCalculator.h
    SpreadValue.h
    SpreadItemDelegate.h
    SpreadCommands.h
    SpreadLineEdit.h
    SpreadConfigurationWidget.h
)

QT4_WRAP_UI(UI_HEADERS
    SpreadInsertColumnDialog.ui
    SpreadMainWidget.ui
    SpreadConfigurationWidget.ui
)

QT4_WRAP_CPP(MOC_SRCS
    SpreadView.h
    SpreadWidget.h
    SpreadTable.h
    SpreadItemDelegate.h
    SpreadLineEdit.h
    SpreadConfigurationWidget.h
)

ADD_LIBRARY(SpreadSheet-${TulipVersion} SHARED ${SRCS} ${MOC_SRCS} ${UI_HEADERS})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(SpreadSheet-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

INSTALL(TARGETS SpreadSheet-${TulipVersion} DESTINATION ${TulipPluginsInstallDir}/view/ COMPONENT tulip_plugins_spreadsheetview)
