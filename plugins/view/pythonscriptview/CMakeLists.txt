IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

ADD_DEFINITIONS("-DQSCINTILLA_DLL")

# Force to use SIP and QScintilla headers located in thirdparty and not those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${qscintilla2Include} ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${Tulip3CompatInclude} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

SET(QT_USE_QTWEBKIT true)

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

ADD_DEPENDENCIES(pythonscriptview-${TulipVersion} ${LibTulipName} ${LibTulipQtName} qscintilla2)

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipQtName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulip3CompatName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} qscintilla2)
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTWEBKIT_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${PYTHON_LIBRARY})

INSTALL(TARGETS pythonscriptview-${TulipVersion} DESTINATION ${TulipPluginsInstallDir} COMPONENT pythonscriptview-${TulipVersion})

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
