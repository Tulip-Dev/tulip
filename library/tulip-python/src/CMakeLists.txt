IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipPythonInclude} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

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
    PythonTabWidget.cpp
    ConsoleHandlers.h
    PythonEditorsTabWidget.cpp
    PythonCppTypesConverter.cpp
)

SET(${LibTulipPythonName}_UI
    ../designer/FindReplaceDialog.ui
)

SET(${LibTulipPythonName}_MOC
    ConsoleHandlers.h
    ../include/tulip/PythonShellWidget.h
    ../include/tulip/PythonCodeEditor.h
    ../include/tulip/PythonEditorsTabWidget.h
)

QTX_WRAP_UI(${LibTulipPythonName}_UI_SRCS ${${LibTulipPythonName}_UI})
QTX_WRAP_CPP(${LibTulipPythonName}_MOC_SRCS ${${LibTulipPythonName}_MOC})
QTX_ADD_RESOURCES(${LibTulipPythonName}_RCC_SRCS ../resources/TulipPython.qrc)

ADD_LIBRARY(${LibTulipPythonName} SHARED ${${LibTulipPythonName}_LIB_SRCS} ${${LibTulipPythonName}_MOC_SRCS} ${${LibTulipPythonName}_UI_SRCS} ${${LibTulipPythonName}_RCC_SRCS})

TARGET_LINK_LIBRARIES(${LibTulipPythonName} ${LibTulipCoreName})
TARGET_LINK_LIBRARIES(${LibTulipPythonName} ${LibTulipGUIName})
TARGET_LINK_LIBRARIES(${LibTulipPythonName} ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(${LibTulipPythonName} ${PYTHON_LIBRARY})

INSTALL(TARGETS ${LibTulipPythonName}
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_python
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_python
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_python)

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
