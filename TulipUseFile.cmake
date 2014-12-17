## -----------------------------------------------------------------------------------------------
## Toolchains options
## -----------------------------------------------------------------------------------------------
MACRO(SET_COMPILER_OPTIONS)

  STRING(COMPARE EQUAL "${CMAKE_SIZEOF_VOID_P}" "8" X64)

  IF(NOT MSVC) #visual studio does not recognize these options
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused -Wno-long-long")
    IF(NOT APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    ENDIF(NOT APPLE)
  ENDIF(NOT MSVC)

  IF(EMSCRIPTEN)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-warn-absolute-paths")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-warn-absolute-paths")
  ENDIF(EMSCRIPTEN)

  IF(WIN32)
    IF(NOT MSVC) #visual studio does not recognize these options
      # Dynamic ling against libstdc++ on win32/MinGW
      # The second test is for the case where ccache is used (CMAKE_CXX_COMPILER_ARG1 contains the path to the g++ compiler)
      IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ARG1}" MATCHES ".*[g][+][+].*")
        EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
                OUTPUT_VARIABLE GCXX_VERSION)

        IF(GCXX_VERSION VERSION_GREATER 4.0)
          SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,windows")
          #GCC 4.4 use double dashes and gcc 4.6 single dashes for this option
          IF(GCXX_VERSION VERSION_LESS 4.6)
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shared-libgcc -Wl,--allow-multiple-definition")
            SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --shared-libgcc  -Wl,--allow-multiple-definition")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --shared-libgcc  -Wl,--allow-multiple-definition")
          ELSE()
            SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -shared-libgcc -Wl,--allow-multiple-definition")
            SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared-libgcc  -Wl,--allow-multiple-definition")
            SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared-libgcc  -Wl,--allow-multiple-definition")
          ENDIF()

        ENDIF()

        IF(GCXX_VERSION VERSION_EQUAL 4.4)
          SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lstdc++_s")
        ELSEIF(GCXX_VERSION VERSION_GREATER 4.5 OR GCXX_VERSION VERSION_EQUAL 4.5)
          #mingw 4.4.0 cannot link the tulip core library as it does not have exceptions symbols correctly defined (MinGW bug #2836185)
          SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DLL")
          SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lstdc++")
        ENDIF()
      ENDIF()
    ENDIF(NOT MSVC)

    IF(MSVC)
      IF(${CMAKE_GENERATOR} MATCHES "Visual Studio 9") # Visual studio 2008 needs boost
        FIND_PACKAGE(BOOST REQUIRED)
        INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}/boost/tr1)
      ENDIF()

      # Tells VS to use multiple threads to compile
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
      # Makes VS define M_PI
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES")
      # Prevents VS to define min and max macros (name clash with std::min and std::max)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
      # Don't warn about the use of unsafe function
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
      # Disable some annoying compiler warnings
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4267 /wd4275 /wd4244 /wd4355 /wd4800 /wd4503 /wd4344")

      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /NODEFAULTLIB:LIBCMTD")

      SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /SUBSYSTEM:windows /ENTRY:mainCRTStartup")

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
  ENDIF(WIN32)

  # Use debug mode with GLIBC
  IF(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ARG1}" MATCHES ".*[g][+][+].*")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGLIBCXX")
  ENDIF()

ENDMACRO(SET_COMPILER_OPTIONS)


# Plugin server generation
FUNCTION(INSTALL)
  IF(TULIP_GENERATE_PLUGINSERVER)

    CMAKE_PARSE_ARGUMENTS(PLUGIN "" "DESTINATION;COMPONENT" "TARGETS" ${ARGN})
    IF(PLUGIN_UNPARSED_ARGUMENTS)
      CMAKE_PARSE_ARGUMENTS(PLUGIN "" "DESTINATION;COMPONENT" "FILES" ${ARGN})
      STRING(REPLACE ${TULIP_DIR} "" DEST ${PLUGIN_DESTINATION})
      SET(PLUGIN_DESTINATION "${CMAKE_BINARY_DIR}/pluginserver/${PLUGIN_COMPONENT}/${DEST}")

      IF(PLUGIN_UNPARSED_ARGUMENTS)
        CMAKE_PARSE_ARGUMENTS(PLUGIN "" "DESTINATION;COMPONENT" "DIRECTORY" ${ARGN})
        STRING(REPLACE ${TULIP_DIR} "" DEST ${PLUGIN_DESTINATION})
        SET(PLUGIN_DESTINATION "${CMAKE_BINARY_DIR}/pluginserver/${PLUGIN_COMPONENT}/${DEST}")
        _INSTALL(DIRECTORY ${PLUGIN_DIRECTORY} COMPONENT ${PLUGIN_COMPONENT} DESTINATION ${PLUGIN_DESTINATION})
      ENDIF()
      _INSTALL(FILES ${PLUGIN_FILES} COMPONENT ${PLUGIN_COMPONENT} DESTINATION ${PLUGIN_DESTINATION})
    ELSE()
      STRING(REPLACE ${TULIP_DIR} "" DEST ${PLUGIN_DESTINATION})
      SET(PLUGIN_DESTINATION "${CMAKE_BINARY_DIR}/pluginserver/${PLUGIN_COMPONENT}/${DEST}")
      FOREACH(TARGET ${PLUGIN_TARGETS})
        _INSTALL(TARGETS ${PLUGIN_TARGETS} COMPONENT ${PLUGIN_COMPONENT} DESTINATION ${PLUGIN_DESTINATION})
      ENDFOREACH()
    ENDIF()

  ELSE(TULIP_GENERATE_PLUGINSERVER)
    _INSTALL(${ARGN})
  ENDIF()
ENDFUNCTION(INSTALL)

MACRO(DISABLE_COMPILER_WARNINGS)
  IF(MSVC)
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  ELSE(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w ")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w ")
  ENDIF(MSVC)
ENDMACRO()

MACRO(GET_FILE_DIRECTORY file_path file_dir_var_name)
  IF(CMAKE_VERSION VERSION_LESS "2.8.12")
    GET_FILENAME_COMPONENT(${file_dir_var_name} ${file_path} PATH)
  ELSE()
    GET_FILENAME_COMPONENT(${file_dir_var_name} ${file_path} DIRECTORY)
  ENDIF()
ENDMACRO(GET_FILE_DIRECTORY)

# External libraries macros
IF(WIN32)

  IF(MINGW)
    # get paths to MINGW binaries, libraries and headers
    STRING(REPLACE "ar.exe" "" MINGW_BIN_PATH ${CMAKE_AR})
    SET(MINGW_LIB_PATH ${MINGW_BIN_PATH}/../lib)
    SET(MINGW_LIB64_PATH ${MINGW_BIN_PATH}/../lib64)
    SET(MINGW_INCLUDE_PATH ${MINGW_BIN_PATH}/../include)
  ENDIF(MINGW)

  MACRO(FIND_EXTERNAL_LIB pattern result_var_name)
    UNSET(${result_var_name})
    UNSET(found_paths)
    FOREACH(win_path $ENV{CMAKE_LIBRARY_PATH} ${QT_BINARY_DIR} ${MINGW_BIN_PATH} ${CMAKE_LIBRARY_PATH})
      STRING(REPLACE "\\" "/" cmake_path "${win_path}")
      FILE(GLOB match "${cmake_path}/${pattern}")
      IF(match)
        SET(found_paths ${found_paths} ${match})
      ENDIF(match)
    ENDFOREACH()
    IF(found_paths)
      LIST(REMOVE_DUPLICATES found_paths)
      SET(${result_var_name} ${found_paths})
    ENDIF(found_paths)
  ENDMACRO(FIND_EXTERNAL_LIB)

  # Try to locate a dll whose name matches the regular expression passed as argument
  # by looking in the following directories :
  #    * those stored in the CMAKE_LIBRARY_PATH environment variable
  #    * those stored in the CMAKE_LIBRARY_PATH CMake variable
  #    * the MinGW binaries directory
  #    * the Qt binaries directory
  # If a dll is found, install it in the application binaries directory
  MACRO(INSTALL_EXTERNAL_LIB pattern component)
    FIND_EXTERNAL_LIB(${pattern} results)
    IF(NOT results)
      MESSAGE("${pattern} could not be located and will not be installed in the binary directory of the application.\n"
              "That library is required to run the application. Please add its path in the CMAKE_LIBRARY_PATH variable in order to install it automatically.")
    ELSE(NOT results)
      FOREACH(F ${results})
        INSTALL(FILES ${F} DESTINATION ${TulipBinInstallDir} COMPONENT ${component})
      ENDFOREACH(F ${results})
    ENDIF(NOT results)
  ENDMACRO(INSTALL_EXTERNAL_LIB)

  MACRO(GET_DLL_NAME_FROM_IMPORT_LIBRARY import_library dll_name)
    UNSET(${dll_name})
    IF(MINGW)
      EXECUTE_PROCESS(COMMAND ${MINGW_BIN_PATH}/dlltool.exe -I ${import_library} OUTPUT_VARIABLE DLL_FILENAME ERROR_VARIABLE DLLTOOL_ERROR)
      IF(DLLTOOL_ERROR)
        MESSAGE("${import_library} is not a valid import library (likely a copy of the associated dll). Please provide a valid one in order to determine the dll the application depends to.")
      ELSE(DLLTOOL_ERROR)
        STRING(REPLACE "\n" "" ${dll_name} ${DLL_FILENAME})
      ENDIF(DLLTOOL_ERROR)
    ENDIF(MINGW)
    IF(MSVC)
      # Get path of MSVC compiler
      IF(CMAKE_VERSION VERSION_LESS "2.8.12")
        GET_FILENAME_COMPONENT(COMPILER_DIRECTORY ${CMAKE_CXX_COMPILER} PATH)
      ELSE()
       GET_FILENAME_COMPONENT(COMPILER_DIRECTORY ${CMAKE_CXX_COMPILER} DIRECTORY)
      ENDIF()
      # Get root path of Visual Studio
      IF(MSVC12)
        GET_FILENAME_COMPONENT(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\12.0\\Setup\\VS;ProductDir] REALPATH)
      ELSEIF(MSVC11)
        GET_FILENAME_COMPONENT(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\11.0\\Setup\\VS;ProductDir] REALPATH)
      ELSEIF(MSVC10)
        GET_FILENAME_COMPONENT(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\10.0\\Setup\\VS;ProductDir] REALPATH)
      ELSEIF(MSVC90)
        GET_FILENAME_COMPONENT(VS_DIR [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\9.0\\Setup\\VS;ProductDir] REALPATH)
      ENDIF()
      # Add temporarily some paths to the PATH environment variable in order to locate some dlls needed to run lib.exe command (mspdb*.dll)
      SET(VS_IDE_DIR "${VS_DIR}/Common7/IDE")
      SET(VS_VC_BIN_DIR "${VS_DIR}/VC/bin")
      SET(PATH_BACKUP "$ENV{PATH}")
      SET(ENV{PATH} "${VS_IDE_DIR};${VS_VC_BIN_DIR};$ENV{PATH}")
      # Run the lib.exe command to list the content of the library file
      EXECUTE_PROCESS(COMMAND ${COMPILER_DIRECTORY}/lib.exe /list ${import_library} OUTPUT_VARIABLE LIBRARY_CONTENTS)
      # If the library is an import library, lib.exe outputs the associated dll name instead of the object files for a static libary
      STRING(REGEX MATCH "[^\r?\n]+\\.dll" ${dll_name} ${LIBRARY_CONTENTS})
      # Restore original PATH environement variable value
      SET(ENV{PATH} "${PATH_BACKUP}")
    ENDIF(MSVC)
  ENDMACRO(GET_DLL_NAME_FROM_IMPORT_LIBRARY)

  # That macro checks if an external library has to be installed in the application directory.
  # It first checks if the library provided as argument is an import library or a static library (for MinGW and MSVC).
  # For MinGW, it also checks if the provided library is a dll (as that compiler allows to directly link with such a library).
  # If the library is static, it does not need to be installed in the application directory.
  # If the library is an import one, the name of the associated dll is retrieved from it and then provided as parameter to the INSTALL_EXTERNAL_LIB macro.
  # If the library is a dll, its name is provided as parameter to the INSTALL_EXTERNAL_LIB macro.
  MACRO(INSTALL_EXTERNAL_LIB_IF_NEEDED library component)

    IF(MINGW)
      STRING(REGEX MATCH ".*dll\\.a" IMPORT_LIBRARY ${library})
      STRING(REGEX MATCH ".*dll" DLL_LIBRARY ${library})
      # If an import library is used, we can easily retrieve the dll name with the dlltool utility
      IF(IMPORT_LIBRARY)
         GET_DLL_NAME_FROM_IMPORT_LIBRARY(${library} DLL_NAME)
         IF(DLL_NAME)
           INSTALL_EXTERNAL_LIB(${DLL_NAME} ${component})
         ENDIF(DLL_NAME)
      # If a dll is directly used, just extract its name from its path
      ELSEIF(DLL_LIBRARY)
        GET_FILE_DIRECTORY(${library} DLL_DIRECTORY)
        GET_FILENAME_COMPONENT(DLL_NAME ${library} NAME)
        SET(CMAKE_LIBRARY_PATH "${DLL_DIRECTORY} ${CMAKE_LIBRARY_PATH}")
        INSTALL_EXTERNAL_LIB(${DLL_NAME} ${component})
      ENDIF()
    ENDIF(MINGW)

    IF(MSVC)
      GET_DLL_NAME_FROM_IMPORT_LIBRARY(${library} DLL_NAME)
      # If we found a dll name, we need to install that dll
      IF(DLL_NAME)
        INSTALL_EXTERNAL_LIB(${DLL_NAME} ${component})
      ENDIF(DLL_NAME)
    ENDIF(MSVC)

  ENDMACRO(INSTALL_EXTERNAL_LIB_IF_NEEDED)

ENDIF(WIN32)
