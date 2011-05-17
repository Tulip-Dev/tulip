INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${CMAKE_CURRENT_BINARY_DIR} .)	

SET(spreadsheet_LIB_SRCS
    SpreadView.cpp 
    TulipTableWidget.cpp
    GraphTableModel.cpp
    ElementCollection.cpp
    TulipItemDelegate.cpp
    TulipQVariantBuilder.cpp
    VectorEditionWidget.cpp    
    HeaderSelectionDialog.cpp
    GlyphPreviewGenerator.cpp
)

set(spreadsheet_UI
    SpreadViewWidget.ui
    VectorEditionWidget.ui
    HeaderSelectionDialog.ui
)

set(spreadsheet_MOC
    SpreadView.h
    TulipTableWidget.h
    GraphTableModel.h
    ElementCollection.h
    TulipItemDelegate.h
    VectorEditionWidget.h    
    HeaderSelectionDialog.h
)

QT4_WRAP_UI(spreadsheet_UI_SRCS ${spreadsheet_UI})
QT4_WRAP_CPP(spreadsheet_MOC_SRCS ${spreadsheet_MOC})

ADD_LIBRARY(SpreadSheet-${TulipVersion} SHARED ${spreadsheet_UI_SRCS} ${spreadsheet_MOC_SRCS} ${spreadsheet_LIB_SRCS})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(SpreadSheet-${TulipVersion} ${LibTulipQtName})
ADD_DEPENDENCIES(SpreadSheet-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName})

INSTALL(TARGETS SpreadSheet-${TulipVersion} DESTINATION ${TulipPluginsInstallDir}/view/ COMPONENT tulip_plugins_spreadsheetview)
