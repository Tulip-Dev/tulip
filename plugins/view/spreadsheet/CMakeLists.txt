INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipOGLInclude} ${TulipQtInclude})	

SET(spreadsheet_LIB_SRCS
    SpreadView.h
    SpreadView.cpp 
    SpreadWidget.cpp 
    SpreadTable.cpp
    SpreadCell.cpp
    SpreadCalculator.cpp
    SpreadValue.cpp
    SpreadItemDelegate.cpp
    SpreadCommands.cpp
    SpreadWidget.h
    SpreadTable.h
    SpreadCell.h
    SpreadCalculator.h
    SpreadValue.h
    SpreadItemDelegate.h
    SpreadCommands.h
    SpreadLineEdit.h
)

set(spreadsheet_UI
    SpreadInsertColumnDialog.ui
    SpreadMainWidget.ui
)

set(spreadsheet_MOC
    SpreadView.h
    SpreadWidget.h
    SpreadTable.h
    SpreadCell.h
    SpreadCalculator.h
    SpreadValue.h
    SpreadItemDelegate.h
    SpreadCommands.h
    SpreadLineEdit.h
)

TULIP_QT4_WRAP_UI(spreadsheet_UI_SRCS ${spreadsheet_UI})
TULIP_QT4_WRAP_CPP(spreadsheet_MOC_SRCS ${spreadsheet_MOC})

ADD_LIBRARY(SpreadSheet-${TulipVersion} SHARED ${spreadsheet_UI_SRCS} ${spreadsheet_MOC_SRCS} ${spreadsheet_LIB_SRCS})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(SpreadSheet-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

INSTALL(TARGETS SpreadSheet-${TulipVersion} DESTINATION ${CMAKE_INSTALL_PREFIX}${TulipLibInstallDir}tlp/view/)
