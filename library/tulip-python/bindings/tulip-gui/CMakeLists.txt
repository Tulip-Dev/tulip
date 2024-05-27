SET(TULIP_GUI_SIP_FILES
    Interactor.sip
    Module.sip
    View.sip
    GlMainView.sip
    NodeLinkDiagramComponent.sip
    TulipGui.sip
    ColorScalesManager.sip
    CACHE INTERNAL ""
)

SET(TULIPGUI_BINDINGS_DIR ${CMAKE_CURRENT_BINARY_DIR}/build/_tulipgui)
SET(TULIP_GUI_PYTHON_BINDINGS_SRC
    ${TULIPGUI_BINDINGS_DIR}/sip_tulipguipart0.cpp
    ${TULIPGUI_BINDINGS_DIR}/sip_tulipguipart1.cpp
    ${TULIPGUI_BINDINGS_DIR}/sip_tulipguipart2.cpp
    ${TULIPGUI_BINDINGS_DIR}/sip_tulipguipart3.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_CPP(tulipguiutils_MOC TulipViewsUtils.h)
QTX_ADD_RESOURCES(tulipgui_RCC_SRCS TulipGui.qrc)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml.in ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml @ONLY)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIP_GUI_PYTHON_BINDINGS_SRC}
COMMAND ${SIP_BUILD} --verbose --protected-is-public --concatenate 4 --api-dir=${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating Python Bindings and API file for libtulip-gui"
  DEPENDS ${TULIP_GUI_SIP_FILES} ${TULIP_SIP_FILES} ${tulipguiutils_MOC} ${tulipgui_RCC_SRCS}  VERBATIM)

SET(TULIP_GUI_PYTHON_BINDINGS_SRC ${TULIP_GUI_PYTHON_BINDINGS_SRC} TulipViewsUtils.cpp ${tulipguiutils_MOC} ${tulipgui_RCC_SRCS})

TULIP_DISABLE_COMPILER_WARNINGS_PYTHON()

Python_add_library(${LibTulipGUIPythonBindingsName} MODULE WITH_SOABI ${TULIP_GUI_PYTHON_BINDINGS_SRC})
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${LibTulipGUIPythonBindingsName})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

TARGET_COMPILE_DEFINITIONS(${LibTulipGUIPythonBindingsName} PUBLIC "protected=public")
TARGET_COMPILE_DEFINITIONS(${LibTulipGUIPythonBindingsName} PUBLIC "SIP_PROTECTED_IS_PUBLIC")
TARGET_COMPILE_DEFINITIONS(${LibTulipGUIPythonBindingsName} PUBLIC "DYNAMIC_ANNOTATIONS_ENABLED=1")
TARGET_INCLUDE_DIRECTORIES(${LibTulipGUIPythonBindingsName}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/build/ ${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipPythonInclude}
    PUBLIC ${TulipOGLInclude} ${TulipGUIInclude}
    PUBLIC ${TULIPGUI_BINDINGS_DIR}
)
ADD_DEPENDENCIES(${LibTulipGUIPythonBindingsName} ${LibTulipCorePythonBindingsName})

SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIPGUI_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIPGUI_PYTHON_NATIVE_FOLDER})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in ${CMAKE_CURRENT_BINARY_DIR}/__init__.py @ONLY)
ADD_CUSTOM_TARGET(copyTulipGuiInitPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/__init__.py ${TULIPGUI_PYTHON_FOLDER}/__init__.py
    COMMENT "Copying tulipgui module __init__.py file"
    DEPENDS ${LibTulipGUIPythonBindingsName}
    VERBATIM)

SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName}
                      PROPERTIES OUTPUT_NAME tulipgui PREFIX "_")

FILE(RELATIVE_PATH TulipLibsInstallRelPath ${TulipPythonModulesInstallDir}/tulipgui/native ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir})

# # On MacOS, add the paths of dependencies dylibs in install rpaths of the _tulipgui.so binary
# # That way, the tulipgui module can be imported in a classical Python shell without having to
# # modify the content of the DYLD_LIBRARY_PATH environment variable
IF(APPLE)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipGUIPythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS};@loader_path/${TulipLibsInstallRelPath}/;@loader_path/../../../../Frameworks")
ELSEIF(LINUX)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipGUIPythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS}:$ORIGIN/${TulipLibsInstallRelPath}")
ENDIF(APPLE)

TARGET_LINK_LIBRARIES(${LibTulipGUIPythonBindingsName} PUBLIC ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})

TULIP_INSTALL_PYTHON_FILES(tulipgui ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
TULIP_INSTALL_PYTHON_FILES(tulipgui/native ${LibTulipGUIPythonBindingsName})

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/build/tulipgui.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_CLEAN_FILES "build;tulipgui_module")
