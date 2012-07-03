## -----------------------------------------------------------------------------------------------
## Toolchains options
## -----------------------------------------------------------------------------------------------
MACRO(SET_COMPILER_OPTIONS)
  IF(NOT MSVC) #visual studio does not recognize these options
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused -Wno-long-long")
    IF(NOT APPLE)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    ENDIF(NOT APPLE)
  ENDIF()

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
   ENDIF()
  ENDIF()
ENDMACRO(SET_COMPILER_OPTIONS)


# Plugin server generation
FUNCTION(install)
  IF(TULIP_GENERATE_PLUGINSERVER)

    cmake_parse_arguments(PLUGIN "" "DESTINATION;COMPONENT" "TARGETS" ${ARGN})
    IF(PLUGIN_UNPARSED_ARGUMENTS)
      cmake_parse_arguments(PLUGIN "" "DESTINATION;COMPONENT" "FILES" ${ARGN})
      STRING(REPLACE ${TULIP_DIR} "" DEST ${PLUGIN_DESTINATION})
      SET(PLUGIN_DESTINATION "${CMAKE_BINARY_DIR}/pluginserver/${PLUGIN_COMPONENT}/${DEST}")

      IF(PLUGIN_UNPARSED_ARGUMENTS)
        cmake_parse_arguments(PLUGIN "" "DESTINATION;COMPONENT" "DIRECTORY" ${ARGN})
        STRING(REPLACE ${TULIP_DIR} "" DEST ${PLUGIN_DESTINATION})
        SET(PLUGIN_DESTINATION "${CMAKE_BINARY_DIR}/pluginserver/${PLUGIN_COMPONENT}/${DEST}")
        _install(DIRECTORY ${PLUGIN_DIRECTORY} COMPONENT ${PLUGIN_COMPONENT} DESTINATION ${PLUGIN_DESTINATION})
      ENDIF()
      _install(FILES ${PLUGIN_FILES} COMPONENT ${PLUGIN_COMPONENT} DESTINATION ${PLUGIN_DESTINATION})
    ELSE()
      STRING(REPLACE ${TULIP_DIR} "" DEST ${PLUGIN_DESTINATION})
      SET(PLUGIN_DESTINATION "${CMAKE_BINARY_DIR}/pluginserver/${PLUGIN_COMPONENT}/${DEST}")
      FOREACH(TARGET ${PLUGIN_TARGETS})
        _install(TARGETS ${PLUGIN_TARGETS} COMPONENT ${PLUGIN_COMPONENT} DESTINATION ${PLUGIN_DESTINATION})
      ENDFOREACH()
    ENDIF()

  ELSE(TULIP_GENERATE_PLUGINSERVER)
    _install(${ARGN})
  ENDIF()
ENDFUNCTION(install)

MACRO(DISABLE_COMPILER_WARNINGS)
  IF(MSVC)
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  ELSE(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w ")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w ")
  ENDIF(MSVC)
ENDMACRO()

# External libraries macros
IF(WIN32)

MACRO(INSTALL_EXTERNAL_LIB pattern component)
  FOREACH(win_path $ENV{CMAKE_LIBRARY_PATH} ${CMAKE_LIBRARY_PATH} ${QT_BINARY_DIR})
    STRING(REPLACE "\\" "/" cmake_path "${win_path}")
    FILE(GLOB match "${cmake_path}/${pattern}")
    IF(match)
      SET(results ${results} ${match})
    ENDIF(match)
  ENDFOREACH()

  IF(NOT results)
    MESSAGE("[CPack] ${pattern} could not be located and will not be installed with package")
  ELSE(NOT results)
    FOREACH(F ${results})
        INSTALL(FILES ${F} DESTINATION ${TulipBinInstallDir} COMPONENT ${component})
    ENDFOREACH(F ${results})
  ENDIF(NOT results)
ENDMACRO(INSTALL_EXTERNAL_LIB)

ENDIF(WIN32)

IF(APPLE)
ENDIF(APPLE)

IF(LINUX)
ENDIF(LINUX)
