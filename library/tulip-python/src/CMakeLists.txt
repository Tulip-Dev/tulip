QTX_SET_INCLUDES_AND_DEFINITIONS()

SET(LibTulipPythonName "tulip-python")

SET(${LibTulipPythonName}_LIB_SRCS
    ConsoleUtilsModule.cpp
    PythonCodeEditor.cpp
    PythonCodeHighlighter.cpp
    ParenMatcherHighlighter.cpp
    PythonInterpreter.cpp
    PythonShellWidget.cpp
    TulipUtilsModule.cpp
    APIDataBase.cpp
    AutoCompletionDataBase.cpp
    ConsoleHandlers.h
    PythonEditorsTabWidget.cpp
    PythonCppTypesConverter.cpp
    PythonIDE.cpp
    PythonPanel.cpp
    PythonPluginCreationDialog.cpp
)

SET(${LibTulipPythonName}_UI
    ../designer/FindReplaceDialog.ui
    ../designer/PythonPluginCreationDialog.ui
    ../designer/PythonIDE.ui
    ../designer/PythonIDEWarningDialog.ui
    ../designer/PythonPanel.ui
)

SET(${LibTulipPythonName}_MOC
    ConsoleHandlers.h
    ../include/tulip/PythonShellWidget.h
    ../include/tulip/PythonCodeEditor.h
    ../include/tulip/PythonEditorsTabWidget.h
    ../include/tulip/PythonInterpreter.h
    ../include/tulip/PythonIDE.h
    ../include/tulip/PythonPanel.h
    ../include/tulip/PythonPluginCreationDialog.h
)

QTX_WRAP_UI(${LibTulipPythonName}_UI_SRCS ${${LibTulipPythonName}_UI})
QTX_WRAP_CPP(${LibTulipPythonName}_MOC_SRCS ${${LibTulipPythonName}_MOC})

ADD_LIBRARY(${LibTulipPythonName}-${Tulip_VERSION} SHARED ${${LibTulipPythonName}_LIB_SRCS} ${${LibTulipPythonName}_MOC_SRCS} ${${LibTulipPythonName}_UI_SRCS})
TARGET_INCLUDE_DIRECTORIES(${LibTulipPythonName}-${Tulip_VERSION}
    PUBLIC ${Python_INCLUDE_DIRS} ${SIP_H_DIR}
    PUBLIC ${TulipCoreBuildInclude}
    PUBLIC ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipPythonInclude}
    ${CMAKE_CURRENT_BINARY_DIR}
)
IF(WIN32)
  TARGET_COMPILE_DEFINITIONS(${LibTulipPythonName}-${Tulip_VERSION} PUBLIC "DLL_TULIP_PYTHON")
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(${LibTulipPythonName}-${Tulip_VERSION} ${LibTulipCoreName} ${LibTulipGUIName} ${QT_LIBRARIES} ${Python_LIBRARIES})

TULIP_INSTALL_PLUGIN(${LibTulipPythonName}-${Tulip_VERSION} ${TulipPluginsInstallDir})
