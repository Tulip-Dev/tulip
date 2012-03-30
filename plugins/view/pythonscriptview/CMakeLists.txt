IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(QT_USE_QTWEBKIT true)

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

# Force to use SIP headers located in thirdparty and not those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

SET(pythonscriptview_LIB_SRCS
    ConsoleOutputModule.h
    PythonCodeEditor.cpp
    PythonCodeEditor.h
    PythonCodeHighlighter.cpp
    ParenMatcherHighlighter.cpp
    PythonInterpreter.cpp
    PythonInterpreter.h
    PythonScriptView.h
    PythonScriptView.cpp
    PythonScriptViewWidget.h
    PythonScriptViewWidget.cpp
    PythonShellWidget.cpp
    TulipUtilsModule.h
    PluginCreationDialog.h
    PluginCreationDialog.cpp
    ConsoleOutputHandler.h
    APIDataBase.cpp
    AutoCompletionDataBase.cpp
)

SET(pythonscriptview_UI
    PythonScriptViewWidget.ui
    PluginCreationDialog.ui
    FindReplaceDialog.ui
)

SET(pythonscriptview_MOC
    PythonScriptView.h
    PythonScriptViewWidget.h
    PythonInterpreter.h
    PythonShellWidget.h
    PluginCreationDialog.h
    ConsoleOutputHandler.h
    PythonCodeEditor.h
)

SET(pythonscriptview_RCC
    PythonScriptViewResource.qrc
)

TULIP_QT4_WRAP_UI(pythonscriptview_UI_SRCS ${pythonscriptview_UI})
TULIP_QT4_WRAP_CPP(pythonscriptview_MOC_SRCS ${pythonscriptview_MOC})
QT4_ADD_RESOURCES(pythonscriptview_RCC_SRCS ${pythonscriptview_RCC})

ADD_LIBRARY(pythonscriptview-${TulipVersion} SHARED ${pythonscriptview_LIB_SRCS} ${pythonscriptview_MOC_SRCS} ${pythonscriptview_UI_SRCS} ${pythonscriptview_RCC_SRCS})

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipQtName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTWEBKIT_LIBRARY})
IF(LINUX)
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTWEBKIT_LIBRARY_RELEASE})
ENDIF(LINUX)
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${PYTHON_LIBRARY})

ADD_DEPENDENCIES(pythonscriptview-${TulipVersion} ${LibTulipName} ${LibTulipQtName})

INSTALL(TARGETS pythonscriptview-${TulipVersion} DESTINATION ${TulipPluginsInstallDir}/view COMPONENT tulip_python)

IF(WIN32)
	FIND_LIBRARY_GLOB(PYTHON_INSTALL_PATH "python2*.dll")
	INSTALL_EXTERNAL_LIB(PYTHON_INSTALL_PATH tulip_python)
ENDIF()

INSTALL(FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Python-2.4.api
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Python-2.5.api
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Python-2.6.api
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Python-2.7.api
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Python-3.1.api
        ${CMAKE_CURRENT_SOURCE_DIR}/api/Python-3.2.api
        ${CMAKE_CURRENT_SOURCE_DIR}/api/PyQt4.api
        DESTINATION ${TulipShareInstallDir}/apiFiles 
        COMPONENT tulip_python)

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
