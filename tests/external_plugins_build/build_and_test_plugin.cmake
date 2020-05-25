# CMake script to test that external Tulip plugins can be
# successfully configured and built using the distributed
# Tulip CMake modules or the tulip-config script

# Temporary Tulip installation directory
SET(TULIP_INSTALL_DIR "${CMAKE_SOURCE_DIR}/tulip_install")

# Temporary plugins build directory
SET(PLUGINS_BIN_DIR "${CMAKE_SOURCE_DIR}/plugins_build")

# Cleanup temporary directories
FILE(REMOVE_RECURSE ${TULIP_INSTALL_DIR})
FILE(REMOVE_RECURSE ${PLUGINS_BIN_DIR})

IF(NOT MSVC)
  # Ensure plugins loading test executable is built
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
                            --build ${TULIP_BIN_DIR}
                            --config ${MSVC_CONFIGURATION}
                            --target ${TEST_EXTERNAL_PLUGINS_LOAD}
                  OUTPUT_QUIET)
ENDIF(NOT MSVC)

# Adjust PATH for plugins loading test
IF(WIN32)
  SET(ENV{PATH} "${TULIP_INSTALL_DIR}/bin;${CMAKE_LIBRARY_PATH};$ENV{PATH}")
ELSE(WIN32)
  SET(ENV{PATH} "${TULIP_INSTALL_DIR}/bin:${CMAKE_LIBRARY_PATH}:$ENV{PATH}")
ENDIF(WIN32)

# Location of plugins loading test executable
SET(TEST_PLUGINS_EXE ${CMAKE_SOURCE_DIR}/${TEST_EXTERNAL_PLUGINS_LOAD})

# Install Tulip in a temporary location
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
                  -DCMAKE_INSTALL_PREFIX=${TULIP_INSTALL_DIR}
                  -P ${TULIP_BIN_DIR}/cmake_install.cmake
                OUTPUT_QUIET)

# Copy plugins source files in build directory
FILE(COPY ${PLUGINS_SRC_DIR}
     DESTINATION ${CMAKE_SOURCE_DIR})
GET_FILENAME_COMPONENT(PLUGINS_SRC_DIR_NAME "${PLUGINS_SRC_DIR}" NAME)

# Build a Tulip plugin using CMake
# ================================

# Generate plugins build files with CMake and use the freshly
# installed Tulip to link against
FILE(MAKE_DIRECTORY ${PLUGINS_BIN_DIR})
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
                  -G "${CMAKE_GENERATOR}"
                  -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                  -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                  -DCMAKE_PREFIX_PATH=${TULIP_INSTALL_DIR}/lib/cmake
                  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                  ../${PLUGINS_SRC_DIR_NAME}
                WORKING_DIRECTORY ${PLUGINS_BIN_DIR}
                RESULT_VARIABLE CMD_RESULT)

# Compile and install the plugins if no error
IF("${CMD_RESULT}" STREQUAL "0")
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND}
                            --build ${PLUGINS_BIN_DIR}
                            --config ${MSVC_CONFIGURATION}
                            --target install
                  RESULT_VARIABLE CMD_RESULT)
ENDIF("${CMD_RESULT}" STREQUAL "0")

# Try to load the plugins if no error
IF("${CMD_RESULT}" STREQUAL "0")
  EXECUTE_PROCESS(COMMAND ${TEST_PLUGINS_EXE} ${TULIP_INSTALL_DIR}/lib/tulip/test
                  RESULT_VARIABLE CMD_RESULT)
ELSE()
# Exit with error if something went wrong
MESSAGE(FATAL_ERROR "The test of build of external plugins (cmake) failed !")
ENDIF("${CMD_RESULT}" STREQUAL "0")

# Build a Tulip plugin using make and tulip-config
# ================================================

# Only for Unix based systems
IF(NOT MSVC)

  # Generate Makefile
  CONFIGURE_FILE(${PLUGINS_SRC_DIR}/Makefile.in
                 ${PLUGINS_SRC_DIR_NAME}/Makefile @ONLY)

  # Adjust Tulip install prefix in tulip-config file copied
  # in temporary Tulip installation directory
  FILE(READ ${TULIP_INSTALL_DIR}/bin/tulip-config TULIP_CONFIG)
  STRING(REPLACE "prefix=${CMAKE_INSTALL_PREFIX}" "prefix=${TULIP_INSTALL_DIR}" TULIP_CONFIG "${TULIP_CONFIG}")
  FILE(WRITE ${TULIP_INSTALL_DIR}/bin/tulip-config "${TULIP_CONFIG}")

  # ensure a clean build
  EXECUTE_PROCESS(COMMAND make clean
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins_src
    RESULT_VARIABLE CMD_RESULT)

  IF("${CMD_RESULT}" STREQUAL "0")
    EXECUTE_PROCESS(COMMAND make
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins_src
                    RESULT_VARIABLE CMD_RESULT)
  ENDIF("${CMD_RESULT}" STREQUAL "0")

  IF("${CMD_RESULT}" STREQUAL "0")
    EXECUTE_PROCESS(COMMAND make install
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins_src
                    RESULT_VARIABLE CMD_RESULT)
  ENDIF("${CMD_RESULT}" STREQUAL "0")

  IF("${CMD_RESULT}" STREQUAL "0")
    EXECUTE_PROCESS(COMMAND ${TEST_PLUGINS_EXE} ${TULIP_INSTALL_DIR}/lib/tulip/test
                    RESULT_VARIABLE CMD_RESULT)
  ENDIF("${CMD_RESULT}" STREQUAL "0")

# Exit with error if something went wrong
IF(NOT "${CMD_RESULT}" STREQUAL "0")
  MESSAGE(FATAL_ERROR "The test of build of external plugins (tulip-config + make) failed !")
ENDIF(NOT "${CMD_RESULT}" STREQUAL "0")

ENDIF(NOT MSVC)
