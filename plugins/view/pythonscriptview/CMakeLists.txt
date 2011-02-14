IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${SIP_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${qscintilla2Include})	

SET(QT_USE_QTWEBKIT true)

SET(pythonscriptview_LIB_SRCS
    ConsoleOutputModule.h
    PythonCodeEditor.cpp
    PythonCodeEditor.h
    PythonInterpreter.cpp
    PythonInterpreter.h
    PythonScriptView.h
    PythonScriptView.cpp
    PythonScriptViewWidget.h
    PythonScriptViewWidget.cpp
    TulipUtilsModule.h
)

SET(pythonscriptview_UI
    PythonScriptViewWidget.ui
)

SET(pythonscriptview_MOC
      PythonScriptView.h
	  PythonInterpreter.h
)

SET(pythonscriptview_RCC
	PythonScriptViewResource.qrc
)

TULIP_QT4_WRAP_UI(pythonscriptview_UI_SRCS ${pythonscriptview_UI})
TULIP_QT4_WRAP_CPP(pythonscriptview_MOC_SRCS ${pythonscriptview_MOC})
QT4_ADD_RESOURCES(pythonscriptview_RCC_SRCS ${pythonscriptview_RCC})

EXECUTE_PROCESS(COMMAND ${SIP_EXE} -a ${CMAKE_CURRENT_SOURCE_DIR}/tulip.api ${CMAKE_SOURCE_DIR}/library/tulip-python/tulip/Module.sip)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

ADD_LIBRARY(pythonscriptview-${TulipVersion} SHARED ${pythonscriptview_UI_SRCS} ${pythonscriptview_MOC_SRCS} ${pythonscriptview_LIB_SRCS} ${pythonscriptview_RCC_SRCS})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipQtName})

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibStlPythonName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipPythonName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipOglPythonName})

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${PYTHON_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${SIP_LIB})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} qscintilla2)

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTOPENGL_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTWEBKIT_LIBRARY})

ADD_DEPENDENCIES(pythonscriptview-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName} ${LibStlPythonName} ${LibTulipPythonName} ${LibTulipOglPythonName} qscintilla2)

INSTALL(TARGETS pythonscriptview-${TulipVersion} DESTINATION ${TulipPluginsInstallDir}/view/ COMPONENT tulip_plugins_pythonscriptview)

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
