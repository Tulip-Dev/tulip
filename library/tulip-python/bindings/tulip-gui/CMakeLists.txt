SET(TULIP_GUI_SIP_FILES
    Interactor.sip
    Module.sip
    View.sip
    GlMainView.sip
    NodeLinkDiagramComponent.sip
    TulipGui.sip
    ColorScalesManager.sip
)

SET(TULIP_GUI_PYTHON_BINDINGS_SRC
    sip_tulipguipart0.cpp
    sip_tulipguipart1.cpp
    sip_tulipguipart2.cpp
)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_GUI_PYTHON_BINDINGS_SRC}
  COMMAND ${SIP_EXE} -n tulip.native.sip -I ${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -c ${CMAKE_CURRENT_BINARY_DIR} -j3 ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating Python Bindings for libtulip-gui"
  DEPENDS ${TULIP_GUI_SIP_FILES} ${TULIP_SIP_FILES} ${STL_SIP_FILES} VERBATIM)

QTX_WRAP_CPP(tulipguiutils_MOC TulipViewsUtils.h)
QTX_ADD_RESOURCES(tulipgui_RCC_SRCS TulipGui.qrc)

SET(TULIP_GUI_PYTHON_BINDINGS_SRC ${TULIP_GUI_PYTHON_BINDINGS_SRC} TulipViewsUtils.cpp ${tulipguiutils_MOC} ${tulipgui_RCC_SRCS})

DISABLE_COMPILER_WARNINGS()

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR}  ${TulipCoreBuildInclude} ${TulipGUIBuildInclude})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${OPENGL_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${QT_INCLUDE_DIR}/QtOpenGL ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

QTX_SET_INCLUDES_AND_DEFINITIONS()

ADD_LIBRARY(${LibTulipGUIPythonBindingsName} SHARED ${TULIP_GUI_PYTHON_BINDINGS_SRC})
ADD_DEPENDENCIES(${LibTulipGUIPythonBindingsName} ${LibTulipCorePythonBindingsName})

SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIPGUI_PYTHON_NATIVE_FOLDER})

# Ensure bindings build output folder exists
ADD_CUSTOM_TARGET(create-tulipgui-python-native-folder ALL
                  COMMAND ${CMAKE_COMMAND} -E make_directory ${TULIPGUI_PYTHON_NATIVE_FOLDER})
ADD_DEPENDENCIES(${LibTulipGUIPythonBindingsName} create-tulipgui-python-native-folder)

ADD_CUSTOM_TARGET(copyTulipGuiInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py ${TULIPGUI_PYTHON_FOLDER}/__init__.py)

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

FILE(RELATIVE_PATH TulipLibsInstallRelPath ${TulipPythonModulesInstallDir}/tulipgui/native ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir})

# On MacOS, add the paths of dependencies dylibs in install rpaths of the _tulipgui.so binary
# That way, the tulipgui module can be imported in a classical Python shell without having to
# modify the content of the DYLD_LIBRARY_PATH environment variable
IF(APPLE)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipGUIPythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS};@loader_path/${TulipLibsInstallRelPath}/;@loader_path/../../../../Frameworks")
ELSEIF(LINUX)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipGUIPythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS}:$ORIGIN/${TulipLibsInstallRelPath}")
ENDIF(APPLE)

TARGET_LINK_LIBRARIES(${LibTulipGUIPythonBindingsName} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES} ${PYTHON_LIBRARY})

TULIP_INSTALL_PYTHON_FILES(tulipgui ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)
TULIP_INSTALL_PYTHON_FILES(tulipgui/native ${LibTulipGUIPythonBindingsName})

ADD_CUSTOM_COMMAND(TARGET ${LibTulipGUIPythonBindingsName}
                   POST_BUILD
                   COMMAND ${SIP_EXE} -n tulip.native.sip -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -a ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
                   COMMENT "Generating API file for Tulip GUI Python bindings"
                   VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulipgui.api;sipAPI_tulipgui.h;tulipgui_module")
