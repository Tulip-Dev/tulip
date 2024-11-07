SET(PYTHON_VERSION_NO_DOT ${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
SET(PYTHON_VERSION ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR})
SET(TulipPythonModulesInstallDir ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir}/tulip/python)

MACRO(TULIP_DISABLE_COMPILER_WARNINGS_PYTHON)
    TULIP_SET_CXX_COMPILER_FLAG("-Wno-old-style-cast -Wno-deprecated-copy -Wno-unused-variable -Wno-overloaded-virtual -Wno-missing-field-initializers")
    TULIP_SET_C_COMPILER_FLAG("-Wno-old-style-cast -Wno-deprecated-copy -Wno-unused-variable -Wno-overloaded-virtual -Wno-missing-field-initializers")
ENDMACRO(TULIP_DISABLE_COMPILER_WARNINGS_PYTHON)

GET_FILENAME_COMPONENT(PYTHON_HOME_PATH ${Python_EXECUTABLE} DIRECTORY)

# Ensure the detection of Python library installed through a bundle downloaded from Python.org or through a macports installation
IF(APPLE)
  IF(NOT "${Python_EXECUTABLE}" MATCHES "^/usr/bin/python.*$")
    EXECUTE_PROCESS(COMMAND bash -c "cd ${PYTHON_HOME_PATH}/.. > /dev/null; echo -n $(pwd)" OUTPUT_VARIABLE PYTHON_DIR_PATH)
    SET(CMAKE_PREFIX_PATH ${PYTHON_HOME_PATH}/.. ${CMAKE_PREFIX_PATH})
  ENDIF()
ENDIF(APPLE)

# Ensure headers correspond to the ones associated to the detected Python library on MacOS
IF(APPLE AND NOT "${Python_EXECUTABLE}" MATCHES "^/usr/bin/python.*$"
   AND EXISTS ${PYTHON_HOME_PATH}/../Headers)
  SET(Python_INCLUDE_DIRS ${PYTHON_HOME_PATH}/../Headers CACHE PATH "" FORCE)
ENDIF()

SET(SIP_BUILD ${Python_EXECUTABLE} -m sipbuild.tools.build) #instead of sip-build
SET(SIP_MODULE_PROG ${Python_EXECUTABLE} -m sipbuild.tools.module) #instead of sip-module
SET(SIP_VERSION 6.8.5)
#check if sip is installed (it is up to the user to install it)
#use the detected python interpreter to call sip instead of the command line tool
#to be sure to use the correct version (command line tool may not be in the PATH)
execute_process(
        COMMAND ${SIP_BUILD} --version
        RESULT_VARIABLE EXIT_CODE
        OUTPUT_QUIET
)

if (NOT ${EXIT_CODE} EQUAL 0)
    message(FATAL_ERROR
            "The \"sip\" Python package is not installed. Please install it using a command like this one: \"${Python_EXECUTABLE} -m pip install sip\".")
endif()

#check sip version
EXECUTE_PROCESS(COMMAND ${SIP_BUILD} --version OUTPUT_VARIABLE SIP_MODULE_OUTPUT)
STRING(STRIP ${SIP_MODULE_OUTPUT} SIP_MODULE_OUTPUT)
IF(${SIP_MODULE_OUTPUT} VERSION_GREATER_EQUAL ${SIP_VERSION})
    MESSAGE(STATUS "Found SIP version ${SIP_MODULE_OUTPUT}")
ELSE()
    MESSAGE(FATAL_ERROR "SIP Python package at least version ${SIP_VERSION} not found (found ${SIP_MODULE_OUTPUT}).")
ENDIF()
##########################################################
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  # STRING(REGEX REPLACE "[^0-9.]" "" TULIP_PYTHON_WHEEL_VERSION "${Tulip_VERSION}")
  SET(TULIP_PYTHON_WHEEL_NAME tulip-python)
  string(REPLACE "-" "_" TULIP_PYTHON_WHEEL_NAME_ ${TULIP_PYTHON_WHEEL_NAME})
  IF(WIN32)
    SET(WHEEL_INSTALL_PATH "\\")
  ELSE(WIN32)
    SET(WHEEL_INSTALL_PATH "/")
  ENDIF(WIN32)
  #check for build
  execute_process(
          COMMAND ${Python_EXECUTABLE} -m pip show build
          RESULT_VARIABLE EXIT_CODE
          OUTPUT_QUIET
  )
  if (NOT ${EXIT_CODE} EQUAL 0)
      message(FATAL_ERROR
              "The \"build\" Python package is not installed. Please install it using a command like this one: \"${Python_EXECUTABLE} -m pip install build\".")
  endif()

  ADD_CUSTOM_TARGET(wheel
    COMMAND ${Python_EXECUTABLE} -m build --wheel
    WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})

  IF(LINUX)
  #where to put wheel after having it repaired
  IF(NOT TULIP_WHEELS_PREFIX)
    SET(TULIP_WHEELS_PREFIX ./dist)
  ENDIF()
  #check for auditwheel (installed by default on manylinux)
  find_program(AUDITWHEEL_CMD auditwheel REQUIRED)
  #filename under linux does not implement Python_SOABI well => cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-linux_x86_64 instead of cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-x86_64-linux-gnu
  SET(TULIP_PYTHON_WHEEL_FILE ${TULIP_PYTHON_WHEEL_NAME_}-${Tulip_VERSION}-cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-linux_x86_64.whl)
  ADD_CUSTOM_COMMAND(TARGET wheel POST_BUILD
      COMMAND bash -xc "LD_LIBRARY_PATH=${Qhull_LIBDIR}:$ENV{LD_LIBRARY_PATH} ${AUDITWHEEL_CMD} repair -L native -w ${TULIP_WHEELS_PREFIX} ./dist/${TULIP_PYTHON_WHEEL_FILE}"
      WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
      COMMENT "Repairing tulip-core wheel" VERBATIM)
  ENDIF(LINUX)
  GET_FILENAME_COMPONENT(PYTHON_EXE_PATH ${Python_EXECUTABLE} DIRECTORY)
  EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -m site --user-base OUTPUT_VARIABLE USER_EXE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
  IF(LINUX OR APPLE)
    SET(USER_EXE_PATH "${USER_EXE_PATH}/bin")
  ELSE()
    SET(USER_EXE_PATH "${USER_EXE_PATH}/../Scripts")
    SET(PYTHON_EXE_PATH "${PYTHON_EXE_PATH}/Scripts")
  ENDIF()
  IF(WIN32)
    find_program(DELVEWHEEL_CMD delvewheel HINTS ${USER_EXE_PATH} ${PYTHON_EXE_PATH} REQUIRED)
    FILE(TO_NATIVE_PATH "${TULIP_WHEELS_PREFIX}" TULIP_WHEELS_PREFIX)
    SET(TULIP_PYTHON_WHEEL_FILE ${TULIP_PYTHON_WHEEL_NAME_}-${Tulip_VERSION}-cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-${Python_SOABI}.whl)
    ADD_CUSTOM_COMMAND(TARGET wheel POST_BUILD
        #ignore-existing parameter below is important to tell delvewheel to not consider libtulip-core-x.dll
        COMMAND ${DELVEWHEEL_CMD} repair -v --ignore-existing --add-path=${TULIP_PYTHON_NATIVE_FOLDER} --wheel-dir=${TULIP_WHEELS_PREFIX} ./dist/${TULIP_PYTHON_WHEEL_FILE}
        WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
        COMMENT "Repairing tulip-core wheel" VERBATIM)
  ENDIF(WIN32)
  IF(APPLE)
    find_program(DELOCATE delocate-wheel HINTS ${USER_EXE_PATH} ${PYTHON_EXE_PATH} REQUIRED)
    FILE(TO_NATIVE_PATH "${TULIP_WHEELS_PREFIX}" TULIP_WHEELS_PREFIX)
    # as the built wheel is not an "universal2" wheel, it has to be renamed
    # with the architecture used by the current build, before repairing
    IF (CMAKE_OSX_ARCHITECTURES)
      SET(ARCH ${CMAKE_OSX_ARCHITECTURES})
    ELSE()
      EXECUTE_PROCESS(COMMAND uname -m
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE ARCH)
    ENDIF()
    EXECUTE_PROCESS(COMMAND sw_vers -productVersion
        OUTPUT_STRIP_TRAILING_WHITESPACE
        OUTPUT_VARIABLE MACOS_VERSION)
    STRING(REGEX MATCH "^[0-9]+\.[0-9]+" MACOS_VERSION ${MACOS_VERSION})
    STRING(REPLACE "." "_" MACOS_VERSION ${MACOS_VERSION})
    SET(TULIP_PYTHON_WHEEL_FILE ${TULIP_PYTHON_WHEEL_NAME_}-${Tulip_VERSION}-cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-macosx_${MACOS_VERSION}_${ARCH}.whl)
    ADD_CUSTOM_COMMAND(TARGET wheel POST_BUILD
        COMMAND bash -c -x "mv ./dist/${TULIP_PYTHON_WHEEL_NAME_}-${Tulip_VERSION}-cp${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-*.whl ./dist/${TULIP_PYTHON_WHEEL_FILE}"
        COMMAND ${DELOCATE} ./dist/${TULIP_PYTHON_WHEEL_FILE} -w ${TULIP_WHEELS_PREFIX}
        WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
        COMMENT "Repairing tulip-core wheel" VERBATIM)
  ENDIF(APPLE)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
##########################################################
#generate the sip module sources
SET(SIP_MODULE tulip.native.sip)
string(REPLACE "." "_" SIP_MODULE_ ${SIP_MODULE})
SET(SIP_LIB sip)
SET(SIP_INCLUDE_DIR ${PROJECT_BINARY_DIR}/thirdparty/sip)

FILE(MAKE_DIRECTORY ${SIP_INCLUDE_DIR})
execute_process(
    COMMAND ${SIP_MODULE_PROG} --sdist --target-dir=${SIP_INCLUDE_DIR} ${SIP_MODULE}
    COMMAND_ERROR_IS_FATAL ANY)
FILE(GLOB SIP_DIST LIST_DIRECTORIES false CONFIGURE_DEPENDS ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-*.tar.gz)
STRING(REGEX MATCH "[0-9]+\.[0-9]+\.[0-9]+" SIP_API_FULL ${SIP_DIST})
STRING(REGEX MATCH "^[0-9]+\.[0-9]+" SIP_API ${SIP_API_FULL}) #used in pyproject.toml files
FILE(ARCHIVE_EXTRACT INPUT ${SIP_DIST} DESTINATION ${SIP_INCLUDE_DIR} PATTERNS *.c *.h)
SET (SIP_H_DIR ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL})
#######################
#compile the sip module on our own instead of pip (produce faulty binaries on Windows and module is not compiled with gcc)
SET(SIP_PYTHON_MODULE_SRC
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_array.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_core.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_enum.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_descriptors.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_int_convertors.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_object_map.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_threads.c
    ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}/sip_voidptr.c
)

Python_add_library(${SIP_LIB} MODULE WITH_SOABI ${SIP_PYTHON_MODULE_SRC})
target_include_directories(${SIP_LIB} PUBLIC ${Python_INCLUDE_DIRS})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES COMPILE_OPTIONS -w)
TARGET_COMPILE_DEFINITIONS(${SIP_LIB} PUBLIC "DYNAMIC_ANNOTATIONS_ENABLED=1")
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${TULIP_PYTHON_NATIVE_FOLDER})
SET_TARGET_PROPERTIES(${SIP_LIB} PROPERTIES OUTPUT_NAME sip)
IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${SIP_LIB})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)

TULIP_INSTALL_PYTHON_FILES(tulip/native ${SIP_LIB})
