# inclusion header guard
IF(TulipUseFile_included)
  RETURN()
ENDIF(TulipUseFile_included)
SET(TulipUseFile_included TRUE)

## ========================================================
## Build type detection
## ========================================================

IF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  SET(CMAKE_DEBUG_MODE TRUE)
ELSE(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
  SET(CMAKE_DEBUG_MODE FALSE)
ENDIF(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")

## -----------------------------------------------------------------------------------------------
## Toolchains options
## -----------------------------------------------------------------------------------------------
MACRO(TULIP_SET_CXX_COMPILER_FLAG flag)
  STRING(FIND "${CMAKE_CXX_FLAGS}" "${flag}" FLAG_POS)
  IF (${FLAG_POS} EQUAL -1)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
  ENDIF()
ENDMACRO(TULIP_SET_CXX_COMPILER_FLAG)

MACRO(TULIP_SET_C_COMPILER_FLAG flag)
  STRING(FIND "${CMAKE_C_FLAGS}" "${flag}" FLAG_POS)
  IF (${FLAG_POS} EQUAL -1)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
  ENDIF()
ENDMACRO(TULIP_SET_C_COMPILER_FLAG)

MACRO(TULIP_SET_CACHE_VAR cache_var_name content concat force)
  GET_PROPERTY(HELP_STR CACHE ${cache_var_name} PROPERTY HELPSTRING)
  STRING(FIND "${${cache_var_name}}" "${content}" CNT_POS)
  IF(${CNT_POS} EQUAL -1)
    SET(VAR_VALUE "${content}")
    IF(${concat})
      SET(VAR_VALUE "${${cache_var_name}} ${content}")
    ENDIF(${concat})
    IF(${force})
      SET(${cache_var_name} "${VAR_VALUE}" CACHE STRING "${HELP_STR}" FORCE)
    ELSE(${force})
      SET(${cache_var_name} "${VAR_VALUE}" CACHE STRING "${HELP_STR}")
    ENDIF(${force})
  ENDIF(${CNT_POS} EQUAL -1)
ENDMACRO(TULIP_SET_CACHE_VAR cache_var_name content concat force)

MACRO(TULIP_SET_COMPILER_OPTIONS)

  STRING(COMPARE EQUAL "${CMAKE_SIZEOF_VOID_P}" "8" X64)

  # enable C++17 (required for Qt6 at least)
  SET(CMAKE_CXX_STANDARD 17)
  SET(CXX_STANDARD_REQUIRED ON)

  ## ========================================================
  ## Operating system preprocessor macros
  ## ========================================================
  IF(LINUX)
    ADD_COMPILE_DEFINITIONS(_LINUX)
  ENDIF(LINUX)
  IF(WIN32)
    ADD_COMPILE_DEFINITIONS(_WIN32)
    # ensure WIN32 is defined (as it is not the case when compiling with MinGW and C++11 standard activated)
    ADD_COMPILE_DEFINITIONS(WIN32)
    # ensure math defines (e.g. M_PI) are available (as they have been dropped from C++11 standard)
    ADD_COMPILE_DEFINITIONS(_USE_MATH_DEFINES)
  ENDIF(WIN32)
  IF(APPLE)
    ADD_COMPILE_DEFINITIONS(__APPLE__)
  ENDIF(APPLE)

  IF(NOT MSVC) # Visual Studio does not recognize these options
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused -Wno-long-long -Wold-style-cast")
    IF(CMAKE_CXX_COMPILER_ID MATCHES Clang)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dtor-name")
    ENDIF(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    IF(NOT APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    ENDIF(NOT APPLE)

    IF(BSD)
      # That compiler flag is required on FreeBSD in order to get a backtrace when Tulip crashes
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_C99 -fno-omit-frame-pointer")
      SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath=/usr/local/lib/gcc5")
      SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath=/usr/local/lib/gcc5")
      SET(CMAKE_MODULE_LINKER_FLAGS "-Wl,-rpath=/usr/local/lib/gcc5")
    ENDIF(BSD)
  ENDIF(NOT MSVC)

  IF(MSVC)
      # Tells VS to use multiple threads to compile
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
      # Makes VS define M_PI
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
      # Prevents VS to define min and max macros (name clash with std::min and std::max)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
      # Don't warn about the use of unsafe function
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
      # Disable some annoying compiler warnings
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4267 /wd4275 /wd4244 /wd4355 /wd4800 /wd4503 /wd4344 /wd4996")

      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD")

      SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:windows /ENTRY:mainCRTStartup")
      SET(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /SUBSYSTEM:windows /ENTRY:mainCRTStartup")

      IF(X64)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  /STACK:10000000 /MACHINE:X64")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  /STACK:10000000 /MACHINE:X64")
        SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}  /STACK:10000000 /MACHINE:X64")
      ELSE(X64)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MACHINE:X86")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X86")
        SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MACHINE:X86")
      ENDIF(X64)
    ENDIF(MSVC)

  # OpenMP
    FIND_PACKAGE(Threads)
    IF(CMAKE_DEBUG_MODE)
      OPTION(TULIP_ENABLE_MULTI_THREADING "Do you want to enable multithreaded code (debug mode)?" OFF)
    ELSE()
      OPTION(TULIP_ENABLE_MULTI_THREADING "Do you want to enable multithreaded code?" ON)
    ENDIF()
    IF(TULIP_ENABLE_MULTI_THREADING)
      # TULIP_CXX_THREADS can be set to force the use of the cxx threads
      # regardless the OpenMP availability
      IF(NOT TULIP_CXX_THREADS)
      IF(APPLE AND (CMAKE_CXX_COMPILER_ID MATCHES Clang))
          EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} --version OUTPUT_VARIABLE CLANG_VERSION)
          STRING(FIND "${CLANG_VERSION}" "Apple" APPLE_POS)
          STRING(COMPARE EQUAL "${APPLE_POS}" "-1" LLVM_LIBOMP)
          # When using LLVM clang on MacOS (from Homebrew or MacPorts), some extra setup
          # might be required in order to detect and use OpenMP through the libomp runtime
          IF(LLVM_LIBOMP)
            TULIP_SET_CACHE_VAR(OpenMP_C_FLAGS "-fopenmp=libomp" FALSE FALSE)
            TULIP_SET_CACHE_VAR(OpenMP_C_LIB_NAMES "libomp" FALSE FALSE)
            TULIP_SET_CACHE_VAR(OpenMP_CXX_FLAGS "-fopenmp=libomp" FALSE FALSE)
            TULIP_SET_CACHE_VAR(OpenMP_CXX_LIB_NAMES "libomp" FALSE FALSE)
            TULIP_SET_CACHE_VAR(OpenMP_libomp_LIBRARY "libomp" FALSE FALSE)
            GET_FILENAME_COMPONENT(LLVM_COMPILER_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
            TULIP_SET_CACHE_VAR(CMAKE_C_FLAGS "-I${LLVM_COMPILER_DIR}/../include" TRUE TRUE)
            TULIP_SET_CACHE_VAR(CMAKE_CXX_FLAGS "-I${LLVM_COMPILER_DIR}/../include" TRUE TRUE)
            TULIP_SET_CACHE_VAR(CMAKE_EXE_LINKER_FLAGS "-L${LLVM_COMPILER_DIR}/../lib" TRUE TRUE)
            TULIP_SET_CACHE_VAR(CMAKE_SHARED_LINKER_FLAGS "-L${LLVM_COMPILER_DIR}/../lib" TRUE TRUE)
          ENDIF(LLVM_LIBOMP)
        ENDIF(APPLE AND (CMAKE_CXX_COMPILER_ID MATCHES Clang))
        FIND_PACKAGE(OpenMP)
        IF(OPENMP_FOUND)
          SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS}")
          SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS}")
          SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS}")
          SET(OPENMP_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
          IF(WIN32)
            IF(MSVC)
              SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /openmp")
              SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /openmp")
              SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /openmp")
              SET(OPENMP_CXX_FLAGS "/openmp")
            ELSE()
              SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp ${CMAKE_THREAD_LIBS_INIT}")
              SET(OPENMP_LIBRARIES "-lgomp -lpthread")
            ENDIF()
          ENDIF()
        ELSE(OPENMP_FOUND)
          IF(WIN32)
            STRING(COMPARE NOTEQUAL "${OpenMP_C_FLAGS}" "" OMP_CFLAGS)
            IF(OMP_CFLAGS)
              # Force setting OpenMP flags on Windows platforms
              SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_C_FLAGS}")
              SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_C_FLAGS}")
              SET(OPENMP_CXX_FLAGS "${OpenMP_C_FLAGS}")
              IF(NOT MSVC)
                SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_C_FLAGS}")
                SET(OPENMP_LINKER_FLAGS "${OpenMP_C_FLAGS}")
                SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lgomp -lpthread")
                SET(OPENMP_LIBRARIES "-lgomp -lpthread")
              ENDIF(NOT MSVC)
              SET(OPENMP_FOUND TRUE)
            ELSE(OMP_CFLAGS)
              MESSAGE("OpenMP not found: Multithreaded code will use C++11 threads")
            ENDIF(OMP_CFLAGS)
          ELSE(WIN32)
            MESSAGE("OpenMP not found: Multithreaded code will use C++11 threads")
          ENDIF(WIN32)
        ENDIF(OPENMP_FOUND)
      ELSE(NOT TULIP_CXX_THREADS)
        MESSAGE("Multithreaded code will use C++11 threads")
      ENDIF(NOT TULIP_CXX_THREADS)
    ELSE(TULIP_ENABLE_MULTI_THREADING)
      MESSAGE("Multithreaded code is disabled")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTLP_NO_THREADS")
    ENDIF(TULIP_ENABLE_MULTI_THREADING)

  IF(APPLE)
    ADD_COMPILE_DEFINITIONS(GL_SILENCE_DEPRECATION)
  ENDIF(APPLE)

ENDMACRO(TULIP_SET_COMPILER_OPTIONS)

MACRO(TULIP_DISABLE_COMPILER_WARNINGS)
  IF(MSVC)
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  ELSE(MSVC)
    TULIP_SET_CXX_COMPILER_FLAG("-w")
    TULIP_SET_C_COMPILER_FLAG("-w")
  ENDIF(MSVC)
ENDMACRO(TULIP_DISABLE_COMPILER_WARNINGS)

# External libraries macros
# IF(WIN32)

#   IF(MINGW)
#     # get paths to MINGW binaries, libraries and headers
#     cmake_path(GET CMAKE_AR ROOT_PATH MINGW_BIN_PATH)
#     SET(MINGW_LIB_PATH ${MINGW_BIN_PATH}/../lib)
#     SET(MINGW_LIB64_PATH ${MINGW_BIN_PATH}/../lib64)
#     SET(MINGW_INCLUDE_PATH ${MINGW_BIN_PATH}/../include)
#   ENDIF(MINGW)
# ENDIF(WIN32)

MACRO(TULIP_COPY_TARGET_LIBRARY_POST_BUILD target_name destination)

  STRING(MD5 DESTINATION_HASH "${destination}")
  SET(COPY_TARGET_NAME copy-${target_name}-${DESTINATION_HASH})
  STRING(REPLACE "/" "" COPY_TARGET_NAME ${COPY_TARGET_NAME})

  IF(WIN32)
    ADD_CUSTOM_TARGET(${COPY_TARGET_NAME} ALL
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${target_name}> ${destination}/$<TARGET_FILE_NAME:${target_name}>
                      DEPENDS ${target_name}
                      VERBATIM)
  ELSE(WIN32)
    ADD_CUSTOM_TARGET(${COPY_TARGET_NAME} ALL
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_SONAME_FILE:${target_name}> ${destination}/$<TARGET_SONAME_FILE_NAME:${target_name}>
                      DEPENDS ${target_name}
                      VERBATIM)
  ENDIF(WIN32)

  # optional parameters of the macro corresponds to targets that depend on the above created custom target
  SET(DEPENDENCIES_TARGETS ${ARGN})

  FOREACH(DEPENDENCY_TARGET ${DEPENDENCIES_TARGETS})
    ADD_DEPENDENCIES(${DEPENDENCY_TARGET} ${COPY_TARGET_NAME})
  ENDFOREACH()
ENDMACRO(TULIP_COPY_TARGET_LIBRARY_POST_BUILD)

# internal cache variable to hold the names of the Tulip plugin targets
SET(TULIP_PLUGIN_TARGETS "" CACHE INTERNAL "")

# Tulip Plugin install macro (its purpose is to disable the installation of MinGW import libraries)
MACRO(TULIP_INSTALL_PLUGIN plugin_target destination)
  SET(COMPONENT_NAME ${plugin_target})
  STRING(REPLACE "-${Tulip_VERSION}" "" COMPONENT_NAME "${COMPONENT_NAME}")
  INSTALL(TARGETS ${plugin_target}
          RUNTIME DESTINATION ${destination}
          LIBRARY DESTINATION ${destination}
          COMPONENT ${COMPONENT_NAME})

  # append the plugin target to the overall plugin targets list
  SET(TULIP_PLUGIN_TARGETS "${TULIP_PLUGIN_TARGETS};${plugin_target}" CACHE INTERNAL "")

  # When building a Python wheel, copy Tulip plugins in wheel build folder
  # in order to package them with the Tulip Python bindings
  IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
    SET(TULIP_PLUGIN_WHEEL_INSTALL_DIR "${TULIP_PYTHON_NATIVE_FOLDER}/plugins")

    TULIP_COPY_TARGET_LIBRARY_POST_BUILD(${plugin_target} ${TULIP_PLUGIN_WHEEL_INSTALL_DIR} wheel)
  ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
ENDMACRO(TULIP_INSTALL_PLUGIN)

MACRO(TULIP_INSTALL_PYTHON_FILES install_suffix)
  FOREACH(PYTHON_FILE ${ARGN})
    IF(TARGET ${PYTHON_FILE})
      INSTALL(TARGETS ${PYTHON_FILE}
              RUNTIME DESTINATION ${TulipPythonModulesInstallDir}/${install_suffix} COMPONENT tulip_python
              LIBRARY DESTINATION ${TulipPythonModulesInstallDir}/${install_suffix} COMPONENT tulip_python)
    ELSEIF(IS_DIRECTORY ${PYTHON_FILE})
      INSTALL(DIRECTORY ${PYTHON_FILE} DESTINATION ${TulipPythonModulesInstallDir}/${install_suffix} COMPONENT tulip_python)
    ELSE()
      INSTALL(FILES ${PYTHON_FILE} DESTINATION ${TulipPythonModulesInstallDir}/${install_suffix} COMPONENT tulip_python)
    ENDIF(TARGET ${PYTHON_FILE})
  ENDFOREACH()
ENDMACRO(TULIP_INSTALL_PYTHON_FILES)
