IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

ADD_DEFINITIONS("-DQSCINTILLA_DLL")

# Force to use SIP and QScintilla headers located in thirdparty and not those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${qscintilla2Include} ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

SET(SRCS
  ConsoleOutputModule.h
  PythonCodeEditor.cpp
  PythonCodeEditor.h
  PythonInterpreter.cpp
  PythonInterpreter.h
  PythonScriptView.h
  PythonScriptView.cpp
  PythonScriptViewWidget.h
  PythonScriptViewWidget.cpp
  TulipUtilsModule.h)

QT4_WRAP_UI(UI_HEADERS
  PythonScriptViewWidget.ui
)

QT4_WRAP_CPP(MOC_SRCS
  PythonScriptView.h
  PythonScriptViewWidget.h
  PythonInterpreter.h
)

QT4_ADD_RESOURCES(RCC_SRCS
  PythonScriptViewResource.qrc
)

IF(HAS_TULIP_QT_PYTHON_BINDINGS)
  SET_SOURCE_FILES_PROPERTIES(PythonInterpreter.cpp PROPERTIES COMPILE_DEFINITIONS HAS_TULIP_QT_PYTHON_BINDINGS)
ENDIF()

ADD_LIBRARY(pythonscriptview-${TulipVersion} SHARED ${SRCS} ${MOC_SRCS} ${UI_HEADERS} ${RCC_SRCS})

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${QT_LIBRARIES} qscintilla2 ${PYTHON_LIBRARY})

ADD_DEPENDENCIES(pythonscriptview-${TulipVersion} ${LibStlPythonName} ${LibTulipPythonName} ${LibTulipOglPythonName})

INSTALL(TARGETS pythonscriptview-${TulipVersion} DESTINATION ${TulipPluginsInstallDir}/view/ COMPONENT tulip_gui_pythonview)

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
