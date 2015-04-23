IF(PYTHONINTERP_FOUND)
IF(PYTHONLIBS_FOUND)
IF(SIP_OK)

SET(TULIP_GUI_SIP_FILES
  Interactor.sip
  Module.sip
  View.sip
  GlMainView.sip
  NodeLinkDiagramComponent.sip
  TulipGui.sip
)

# First, generate tulip properties bindings based on template SIP files
# Then, generate a SIP build file and parse its content to retrieve source files list
EXECUTE_PROCESS(COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -b ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.sbf ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.sbf TULIP_GUI_SBF)
STRING(REGEX MATCHALL "[0-9A-Za-z_]*\\.cpp" TULIP_GUI_PYTHON_BINDINGS_SRC ${TULIP_GUI_SBF})

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_GUI_PYTHON_BINDINGS_SRC}
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating Python Bindings for libtulip-gui"
  DEPENDS ${TULIP_GUI_SIP_FILES} ${TULIP_OGL_SIP_FILES} ${TULIP_SIP_FILES} ${STL_SIP_FILES} VERBATIM)

QTX_WRAP_CPP(tulipguiutils_MOC TulipViewsUtils.h)
QTX_ADD_RESOURCES(tulipgui_RCC_SRCS TulipGui.qrc)

SET(TULIP_GUI_PYTHON_BINDINGS_SRC ${TULIP_GUI_PYTHON_BINDINGS_SRC} TulipViewsUtils.cpp ${tulipguiutils_MOC} ${tulipgui_RCC_SRCS})
           
DISABLE_COMPILER_WARNINGS()

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${QT_INCLUDE_DIR} ${QT_INCLUDE_DIR}/QtOpenGL ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipPythonInclude} ${TulipGUIBuildInclude} ${TulipCoreBuildInclude})

ADD_LIBRARY(${LibTulipGUIPythonBindingsName} SHARED ${TULIP_GUI_PYTHON_BINDINGS_SRC})

IF(NOT SYSTEM_SIP)
ADD_DEPENDENCIES(${LibTulipGUIPythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES OUTPUT_NAME tulipgui PREFIX "")

IF(WIN32)
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipGUIPythonBindingsName} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${LibTulipPythonName} ${PYTHON_LIBRARY} ${QT_LIBRARIES} )

IF(NOT UBUNTU_PPA_BUILD)
INSTALL(TARGETS ${LibTulipGUIPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python)
ELSE()
INSTALL(TARGETS ${LibTulipGUIPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/../lib/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages COMPONENT tulip_python)
ENDIF()

ADD_CUSTOM_TARGET(touchTulipGuiPythonCMakeLists ALL
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
  DEPENDS ${LibTulipGUIPythonBindingsName} VERBATIM)

ADD_CUSTOM_COMMAND(
  TARGET ${LibTulipGUIPythonBindingsName}
  POST_BUILD
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -a ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating QScintilla API file for Tulip GUI Python bindings"
  VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulipgui.sbf;tulipgui.api;sipAPItulipgui.h")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)
