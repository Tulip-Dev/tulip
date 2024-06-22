SET(PYTHON_VERSION_NO_DOT ${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
SET(PYTHON_VERSION ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR})
SET(TulipPythonModulesInstallDir ${CMAKE_INSTALL_PREFIX}/${TulipLibInstallDir}/tulip/python)

MACRO(TULIP_DISABLE_COMPILER_WARNINGS_PYTHON)
    TULIP_SET_CXX_COMPILER_FLAG("-Wno-old-style-cast -Wno-deprecated-copy -Wno-unused-variable -Wno-overloaded-virtual")
    TULIP_SET_C_COMPILER_FLAG("-Wno-old-style-cast -Wno-deprecated-copy -Wno-unused-variable -Wno-overloaded-virtual")
ENDMACRO(TULIP_DISABLE_COMPILER_WARNINGS_PYTHON)

GET_FILENAME_COMPONENT(PYTHON_HOME_PATH ${Python_EXECUTABLE} DIRECTORY)

# Ensure the detection of Python library installed through a bundle downloaded from Python.org or through a macports installation
IF(APPLE)
  IF(NOT "${Python_EXECUTABLE}" MATCHES "^/usr/bin/python.*$")
    EXECUTE_PROCESS(COMMAND bash -c "cd ${PYTHON_HOME_PATH}/.. > /dev/null; echo -n $(pwd)" OUTPUT_VARIABLE PYTHON_DIR_PATH)
    SET(CMAKE_PREFIX_PATH ${PYTHON_HOME_PATH}/.. ${CMAKE_PREFIX_PATH})
  ENDIF()
ENDIF(APPLE)

# IF(MINGW)
  # Check if Python is provided by MSYS2 (it is compiled with GCC in that case instead of MSVC)
  # EXECUTE_PROCESS(COMMAND ${Python_EXECUTABLE} -VV OUTPUT_VARIABLE PYTHON_VERSION_FULL ERROR_VARIABLE PYTHON_VERSION_FULL)
  # STRING(REGEX MATCH "GCC" MSYS2_PYTHON "${PYTHON_VERSION_FULL}")

  # Python 64bits does not provide a dll import library for MinGW.
  # Fortunately, we can directly link to the Python dll with that compiler.
  # So find the location of that dll and overwrite the Python_LIBRARIES CMake cache variable with it

  # IF(MSYS2_PYTHON)
    # IF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll)
      # SET(Python_LIBRARIES ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll CACHE FILEPATH "" FORCE)
    # ELSEIF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}m.dll)
      # SET(Python_LIBRARIES ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}m.dll CACHE FILEPATH "" FORCE)
    # ENDIF(EXISTS ${PYTHON_HOME_PATH}/libpython${PYTHON_VERSION}.dll)
  # ELSE(MSYS2_PYTHON)
  #   # Check if the Python dll is located in the Python home directory (when Python is installed for current user only)
  #   IF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
  #     SET(Python_LIBRARIES ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
  #     #If not, the Python dll is located in %WINDIR%/System32 (when Python is installed for all users)
  #   ELSE(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
  #     STRING(REPLACE "\\" "/" WINDIR $ENV{WINDIR})
  #     IF(NOT WIN_AMD64 OR X64)
  #       SET(Python_LIBRARIES ${WINDIR}/System32/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
  #     ELSE(NOT WIN_AMD64 OR X64)
  #       SET(Python_LIBRARIES ${WINDIR}/SysWOW64/python${PYTHON_VERSION_NO_DOT}.dll CACHE FILEPATH "" FORCE)
  #     ENDIF(NOT WIN_AMD64 OR X64)
  #   ENDIF(EXISTS ${PYTHON_HOME_PATH}/python${PYTHON_VERSION_NO_DOT}.dll)
  # ENDIF(MSYS2_PYTHON)
# ENDIF(MINGW)

# Ensure headers correspond to the ones associated to the detected Python library on MacOS
IF(APPLE AND NOT "${Python_EXECUTABLE}" MATCHES "^/usr/bin/python.*$"
   AND EXISTS ${PYTHON_HOME_PATH}/../Headers)
  SET(Python_INCLUDE_DIRS ${PYTHON_HOME_PATH}/../Headers CACHE PATH "" FORCE)
ENDIF()

SET(SIP_BUILD ${Python_EXECUTABLE} -m sipbuild.tools.build) #instead of sip-build
SET(SIP_MODULE_PROG ${Python_EXECUTABLE} -m sipbuild.tools.module) #instead of sip-module
SET(SIP_VERSION 6.8.5)
SET(SIP_API 13.7)
SET(SIP_API_FULL 13.7.0)
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

SET(SIP_MODULE tulip.native.sip)
string(REPLACE "." "_" SIP_MODULE_ ${SIP_MODULE})
SET(SIP_LIB sip)

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  STRING(REGEX REPLACE "[^0-9.]" "" TULIP_PYTHON_WHEEL_VERSION "${Tulip_VERSION}")

  IF(WIN32)
    SET(WHEEL_INSTALL_PATH "\\")
  ELSE(WIN32)
    SET(WHEEL_INSTALL_PATH "/")
  ENDIF(WIN32)
  #check for wheel
  execute_process(
          COMMAND ${Python_EXECUTABLE} -m pip show wheel
          RESULT_VARIABLE EXIT_CODE
          OUTPUT_QUIET
  )
  if (NOT ${EXIT_CODE} EQUAL 0)
      message(FATAL_ERROR
              "The \"wheel\" Python package is not installed. Please install it using a command like this one: \"${Python_EXECUTABLE} -m pip install wheel\".")
  endif()

  ADD_CUSTOM_TARGET(wheel
    COMMAND ${Python_EXECUTABLE} setup.py bdist_wheel
    WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})

  # check generation of test wheels
  STRING(COMPARE NOTEQUAL "${TULIP_PYTHON_TEST_WHEEL_SUFFIX}" "" TULIP_GENERATE_TESTPYPI_WHEEL)

  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
    SET(TULIP_PYTHON_TEST_WHEEL_VERSION ${TULIP_PYTHON_WHEEL_VERSION}.${TULIP_PYTHON_TEST_WHEEL_SUFFIX})

    ADD_CUSTOM_TARGET(test-wheel
      COMMAND ${Python_EXECUTABLE} setuptest.py bdist_wheel
            WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER})
    ADD_DEPENDENCIES(test-wheel wheel)
  ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)

  IF(LINUX)
  #where to put wheel after having it repaired
  IF(NOT TULIP_WHEELS_PREFIX)
    SET(TULIP_WHEELS_PREFIX ./dist)
  ENDIF()
  #check for auditwheel (installed by default on manylinux)
  find_program(AUDITWHEEL_CMD auditwheel REQUIRED)
    ADD_CUSTOM_COMMAND(TARGET wheel POST_BUILD
      COMMAND bash -xc "LD_LIBRARY_PATH=${Qhull_LIBDIR}:$ENV{LD_LIBRARY_PATH} ${AUDITWHEEL_CMD} repair -L native -w ${TULIP_WHEELS_PREFIX} ./dist/$(ls -t ./dist/ | head -1)"
      COMMAND bash -xc "rm ./dist/$(ls -t ./dist/ | head -2 | tail -1)"
      WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
      COMMENT "Repairing tulip-core wheel" VERBATIM)

  IF(TULIP_GENERATE_TESTPYPI_WHEEL)
      ADD_CUSTOM_COMMAND(TARGET test-wheel POST_BUILD
        COMMAND bash -xc "LD_LIBRARY_PATH=${Qhull_LIBDIR}:$ENV{LD_LIBRARY_PATH} ${AUDITWHEEL_CMD} repair -L native -w ${TULIP_WHEELS_PREFIX} ./dist/$(ls -t ./dist/ | head -1)"
        COMMAND bash -xc "rm ./dist/$(ls -t ./dist/ | head -2 | tail -1)"
        WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER}
        COMMENT "Repairing tulip-core test wheel"  VERBATIM
        )
    ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)

  ENDIF(LINUX)

  # In order to upload the generated wheels, an account must be created on PyPi
  # and the following configuration must be stored in the ~/.pypirc file
  ##############################################################
  # [distutils]
  # index-servers=
  #     pypi
  #     testpypi
  #
  # [testpypi]
  # repository: https://test.pypi.org/legacy/
  # username: <your user name goes here>
  # password: <your password goes here>
  #
  # [pypi]
  # repository: https://upload.pypi.org/legacy/
  # username: <your user name goes here>
  # password: <your password goes here>
  ###############################################################


  # SET(TWINE twine)
  # IF(EXISTS ${PYTHON_HOME_PATH}/twine)
  #   SET(TWINE ${PYTHON_HOME_PATH}/twine)
  # ENDIF(EXISTS ${PYTHON_HOME_PATH}/twine)
  # IF(WIN32)
  #   SET(TWINE ${Python_INCLUDE_DIRS}/../Scripts/twine.exe)
  # ENDIF(WIN32)
  # SET(WHEEL_FILES_REGEXP "*${TULIP_PYTHON_WHEEL_VERSION}-cp*")
  # ADD_CUSTOM_TARGET(wheel-upload
  #   COMMAND bash -c "echo -e 'uploading wheels:\\n' $(ls ${TULIP_PYTHON_ROOT_FOLDER}/dist/${WHEEL_FILES_REGEXP})"
  #   COMMAND ${TWINE} upload -r pypi dist/${WHEEL_FILES_REGEXP}
  #   WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER} VERBATIM)

  # IF(TULIP_GENERATE_TESTPYPI_WHEEL)
  #   SET(TEST_WHEEL_FILES_REGEXP "*${TULIP_PYTHON_TEST_WHEEL_VERSION}*")
  #   ADD_CUSTOM_TARGET(test-wheel-upload
  #     COMMAND bash -c "echo -e 'uploading test wheels:\\n' $(ls ${TULIP_PYTHON_ROOT_FOLDER}/dist/${TEST_WHEEL_FILES_REGEXP})"
  #     COMMAND ${TWINE} upload -r testpypi dist/${TEST_WHEEL_FILES_REGEXP}
  #     WORKING_DIRECTORY ${TULIP_PYTHON_ROOT_FOLDER} VERBATIM)
  # ENDIF(TULIP_GENERATE_TESTPYPI_WHEEL)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
##########################################################
#generate the sip module sources
SET(SIP_INCLUDE_DIR ${PROJECT_BINARY_DIR}/thirdparty/sip)
SET (SIP_H_DIR ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL})
SET(SIP_MODULE_SRC ${SIP_INCLUDE_DIR}/${SIP_MODULE_}-${SIP_API_FULL}.tar.gz)
IF(NOT EXISTS ${SIP_MODULE_SRC})
    MESSAGE(STATUS "Generating SIP module sources")
    FILE(MAKE_DIRECTORY ${SIP_INCLUDE_DIR})
    execute_process(
        COMMAND ${SIP_MODULE_PROG} --sdist --abi-version=${SIP_API} --target-dir=${SIP_INCLUDE_DIR} ${SIP_MODULE}
        COMMAND_ERROR_IS_FATAL ANY)
    execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar zxf  ${SIP_MODULE_SRC}
    WORKING_DIRECTORY ${SIP_INCLUDE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
    )
ENDIF()
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

#######################"

TULIP_INSTALL_PYTHON_FILES(tulip/native ${SIP_LIB})

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_DEPENDENCIES(wheel ${SIP_LIB})
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
