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
    ColorScalesManager.sip
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

IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
STRING(REGEX REPLACE "[^0-9.]" "" TulipVersionClean ${TulipVersion})

IF(WIN32)
SET(WHEEL_INSTALL_PATH "\\")
ELSE(WIN32)
SET(WHEEL_INSTALL_PATH "/")
ENDIF(WIN32)

ADD_CUSTOM_COMMAND(OUTPUT ${TULIPGUI_PYTHON_FOLDER}/../setup.py ${TULIPGUI_PYTHON_FOLDER}/../setup.cfg
  COMMAND ${CMAKE_COMMAND} -DINPUT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/packaging/
                           -DOUTPUT_DIR=${TULIPGUI_PYTHON_FOLDER}/../
                           -DTulipVersion=${TulipVersion}
                           -DTulipVersionClean=${TulipVersionClean}
                           -DPythonWheelVersionSuffix=${PythonWheelVersionSuffix}
                           -DWHEEL_INSTALL_PATH=${WHEEL_INSTALL_PATH}
                           -P ${CMAKE_CURRENT_SOURCE_DIR}/packaging/configure_tulipgui_python_setup.cmake
  DEPENDS
    packaging/setup.py.in
    packaging/setup.cfg.in
  COMMENT "Generating tulipgui-python packaging files"
  VERBATIM)

ADD_CUSTOM_TARGET(copyTulipGuiPythonPackagingFiles ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/MANIFEST.in ${TULIPGUI_PYTHON_FOLDER}/../MANIFEST.in
                                                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/README.rst ${TULIPGUI_PYTHON_FOLDER}/../README.rst
                                                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/packaging/copyTulipGuiDllDependencies.cmake ${TULIPGUI_PYTHON_FOLDER}/../copyTulipGuiDllDependencies.cmake
                                                   DEPENDS ${TULIPGUI_PYTHON_FOLDER}/../setup.py ${TULIPGUI_PYTHON_FOLDER}/../setup.cfg
)
ADD_DEPENDENCIES(tulip-gui-wheel copyTulipGuiPythonPackagingFiles)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

# Force to use SIP headers located in thirdparty
# instead of those installed in the system
INCLUDE_DIRECTORIES(BEFORE ${SIP_INCLUDE_DIR}  ${TulipCoreBuildInclude} ${TulipGUIBuildInclude})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PYTHON_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH} ${OPENGL_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${QT_INCLUDE_DIR}/QtOpenGL ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

ADD_LIBRARY(${LibTulipGUIPythonBindingsName} SHARED ${TULIP_GUI_PYTHON_BINDINGS_SRC})

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
IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ADD_DEPENDENCIES(tulip-gui-wheel copyTulipGuiInitPy)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

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

# On MacOS, add the paths of dependencies dylibs in install rpaths of the _tulipgui.so binary
# That way, the tulipgui module can be imported in a classical Python shell without having to
# modify the content of the DYLD_LIBRARY_PATH environment variable
IF(APPLE AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipGUIPythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS};@loader_path/../../../;@loader_path/../../../../Frameworks")
ELSEIF(LINUX AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  GET_TARGET_PROPERTY(CURRENT_INSTALL_RPATHS ${LibTulipGUIPythonBindingsName} INSTALL_RPATH)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES INSTALL_RPATH "${CURRENT_INSTALL_RPATHS}:$ORIGIN/../../../")
ENDIF(APPLE AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

TARGET_LINK_LIBRARIES(${LibTulipGUIPythonBindingsName} ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})
# When building Python wheel for MacOS, don't link the C extension module with the Python library
# and use dynamic lookup for retrieving its symbols.
# That way, we can produce a C extension module that can be imported through the Python interpreter
# provided by Apple with the System and the one provided by Python.org
IF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  SET_TARGET_PROPERTIES(${LibTulipGUIPythonBindingsName} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
ELSE(APPLE AND TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  IF(NOT LINUX OR NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
    TARGET_LINK_LIBRARIES(${LibTulipGUIPythonBindingsName} ${PYTHON_LIBRARY})
  ENDIF(NOT LINUX OR NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ENDIF(APPLE AND TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

INSTALL_TULIP_PYTHON_FILES(tulipgui ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py)
INSTALL_TULIP_PYTHON_FILES(tulipgui/native ${LibTulipGUIPythonBindingsName})

ADD_CUSTOM_COMMAND(
  TARGET ${LibTulipGUIPythonBindingsName}
  POST_BUILD
  COMMAND ${SIP_EXE} -I ${CMAKE_CURRENT_BINARY_DIR}/../tulip-core -I ${CMAKE_CURRENT_SOURCE_DIR}/../tulip-core -a ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api ${CMAKE_CURRENT_SOURCE_DIR}/Module.sip
  COMMENT "Generating API file for Tulip GUI Python bindings"
  VERBATIM)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulipgui.api DESTINATION ${TulipShareInstallDir}/apiFiles COMPONENT tulip_python)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "tulipgui.api;sipAPI_tulipgui.h;tulipgui_module")

ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND)
ENDIF(PYTHONINTERP_FOUND)
