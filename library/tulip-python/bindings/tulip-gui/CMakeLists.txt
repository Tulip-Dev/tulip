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

SET(TULIP_GUI_PYTHON_BINDINGS_SRC
    sip_tulipguipart0.cpp
    sip_tulipguipart1.cpp
    sip_tulipguipart2.cpp
)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_GUI_PYTHON_BINDINGS_SRC}
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -c ${CMAKE_CURRENT_BINARY_DIR} -j3 ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating Python Bindings for libtulip-gui"
  DEPENDS ${TULIP_GUI_SIP_FILES} ${TULIP_OGL_SIP_FILES} ${TULIP_SIP_FILES} ${STL_SIP_FILES} VERBATIM)

QTX_WRAP_CPP(tulipguiutils_MOC TulipViewsUtils.h)
QTX_ADD_RESOURCES(tulipgui_RCC_SRCS TulipGui.qrc)

SET(TULIP_GUI_PYTHON_BINDINGS_SRC ${TULIP_GUI_PYTHON_BINDINGS_SRC} TulipViewsUtils.cpp ${tulipguiutils_MOC} ${tulipgui_RCC_SRCS})
           
DISABLE_COMPILER_WARNINGS()

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${QT_INCLUDE_DIR} ${QT_INCLUDE_DIR}/QtOpenGL ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${TulipGUIBuildInclude} ${TulipCoreBuildInclude})

ADD_LIBRARY(${LibTulipGUIPythonBindingsName} SHARED ${TULIP_GUI_PYTHON_BINDINGS_SRC})

SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/native")
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/native")
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/native")
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/native")
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/native")
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/native")

ADD_CUSTOM_COMMAND(TARGET ${LibTulipGUIPythonBindingsName}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${CMAKE_CURRENT_BINARY_DIR}/tulipgui_module/tulipgui/__init__.py
                   VERBATIM)

IF(NOT SYSTEM_SIP)
ADD_DEPENDENCIES(${LibTulipGUIPythonBindingsName} ${SIP_LIB})
ENDIF(NOT SYSTEM_SIP)

SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES OUTPUT_NAME tulipgui PREFIX "_")

IF(WIN32)
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES SUFFIX ".pyd")
ELSE(WIN32)
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES SUFFIX ".so")
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${LibTulipGUIPythonBindingsName} ${LibTulipCoreName} ${LibTulipOglName} ${LibTulipGUIName} ${PYTHON_LIBRARY} ${QT_LIBRARIES} )


IF(NOT PYTHON_DIST_PACKAGE)
INSTALL(TARGETS ${LibTulipGUIPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/python/tulipgui/native COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python/tulipgui/native COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python/tulipgui/native COMPONENT tulip_python)
IF(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipBinInstallDir}/python/tulipgui COMPONENT tulip_python)
ELSE(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipLibInstallDir}/python/tulipgui COMPONENT tulip_python)
ENDIF(WIN32)
ELSE(NOT PYTHON_DIST_PACKAGE)
INSTALL(TARGETS ${LibTulipGUIPythonBindingsName}
        RUNTIME DESTINATION ${TulipBinInstallDir}/../lib/python${PYTHON_VERSION}/dist-packages/tulipgui/native COMPONENT tulip_python
        LIBRARY DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulipgui/native COMPONENT tulip_python
        ARCHIVE DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulipgui/native COMPONENT tulip_python)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py DESTINATION ${TulipLibInstallDir}/python${PYTHON_VERSION}/dist-packages/tulipgui COMPONENT tulip_python)
ENDIF(NOT PYTHON_DIST_PACKAGE)

ADD_CUSTOM_COMMAND(
  TARGET ${LibTulipGUIPythonBindingsName}
  POST_BUILD
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -a ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating API file for Tulip GUI Python bindings"
  VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulipgui.api;sipAPI_tulipgui.h")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)
