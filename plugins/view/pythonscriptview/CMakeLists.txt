IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

INCLUDE("${PROJECT_SOURCE_DIR}/${TulipUseFile}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQSCINTILLA_DLL")

# Force to use SIP and QScintilla headers located in thirdparty and not those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${qscintilla2Include} ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${TulipInclude} ${TulipBuildInclude} ${TulipOGLInclude} ${TulipQtInclude} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

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
    PythonScriptViewWidget.h
    PythonInterpreter.h
)

SET(pythonscriptview_RCC
    PythonScriptViewResource.qrc
)

TULIP_QT4_WRAP_UI(pythonscriptview_UI_SRCS ${pythonscriptview_UI})
TULIP_QT4_WRAP_CPP(pythonscriptview_MOC_SRCS ${pythonscriptview_MOC})
QT4_ADD_RESOURCES(pythonscriptview_RCC_SRCS ${pythonscriptview_RCC})

IF(HAS_TULIP_QT_PYTHON_BINDINGS)
SET_SOURCE_FILES_PROPERTIES(PythonInterpreter.cpp
                            PROPERTIES COMPILE_DEFINITIONS HAS_TULIP_QT_PYTHON_BINDINGS)
ENDIF()

ADD_LIBRARY(pythonscriptview-${TulipVersion} SHARED ${pythonscriptview_UI_SRCS} ${pythonscriptview_MOC_SRCS} ${pythonscriptview_LIB_SRCS} ${pythonscriptview_RCC_SRCS})

IF(${MSVC} AND ${CMAKE_GENERATOR}  MATCHES "Visual Studio")
	ADD_CUSTOM_COMMAND(
				   TARGET pythonscriptview-${TulipVersion}
				   PRE_BUILD
                   COMMAND ${SIP_EXE} -a ${CMAKE_CURRENT_SOURCE_DIR}/tulip.api -I ${CMAKE_CURRENT_SOURCE_DIR}/../../../library/tulip-python/tulip/  ${CMAKE_CURRENT_SOURCE_DIR}/../../../library/tulip-python/tulip/Module.sip
                   COMMENT "Generating QScintilla API file for Tulip Python bindings"
                   DEPENDS library/tulip-python/tulip/*.sip
				   VERBATIM)
ELSE()
	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tulip.api
                   COMMAND ${SIP_EXE} -a ${CMAKE_CURRENT_SOURCE_DIR}/tulip.api ${CMAKE_SOURCE_DIR}/library/tulip-python/tulip/Module.sip
                   COMMENT "Generating QScintilla API file for Tulip Python bindings"
                   DEPENDS ${CMAKE_SOURCE_DIR}/library/tulip-python/tulip/*.sip)
ENDIF()

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipOglName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${LibTulipQtName})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} qscintilla2)
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTCORE_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTGUI_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTOPENGL_LIBRARY})
TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${QT_QTWEBKIT_LIBRARY})

TARGET_LINK_LIBRARIES(pythonscriptview-${TulipVersion} ${PYTHON_LIBRARY})

ADD_DEPENDENCIES(pythonscriptview-${TulipVersion} ${LibTulipName} ${LibTulipOglName} ${LibTulipQtName} ${LibStlPythonName} ${LibTulipPythonName} ${LibTulipOglPythonName} qscintilla2)

INSTALL(TARGETS pythonscriptview-${TulipVersion} DESTINATION ${TulipPluginsInstallDir}/view/ COMPONENT tulip_plugins_pythonscriptview)

IF(WIN32)
	FIND_LIBRARY_GLOB(PYTHON_INSTALL_PATH "python2*.dll")
	INSTALL_EXTERNAL_LIB(PYTHON_INSTALL_PATH tulip_plugins_pythonscriptview)
ENDIF()

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
