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

## -----------------------------------------------------------------------------------------------
## General macros
## -----------------------------------------------------------------------------------------------
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

  ELSE(TULIP_GENERATE_PLUGINSERVER) # Check CPack component and forward to install
    cmake_parse_arguments(TGT "" "COMPONENT;TARGETS;FILES" "" ${ARGN})
    IF(NOT TGT_COMPONENT) # Check COMPONENT declaration
      MESSAGE(FATAL_ERROR "[CPack] Component not defined for ${TGT_TARGETS} ${TGT_FILES}. Component must be defined to ensure correct installer generation.")
    ENDIF(NOT TGT_COMPONENT)
    SET(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} ${TGT_COMPONENT} CACHE INTERNAL "")
    FOREACH(T ${TGT_TARGETS})
      SET(BUNDLE_${T}_COMP ${TGT_COMPONENT} CACHE INTERNAL "")
      SET(BUNDLE_TARGETS ${BUNDLE_TARGETS} ${T} CACHE INTERNAL "")
    ENDFOREACH()
    _install(${ARGN})
  ENDIF()
ENDFUNCTION(install)

IF(NOT APPLE)

FUNCTION(TARGET_LINK_LIBRARIES)
  FOREACH(A ${ARGV})
    IF(NOT FIRST_ARG)
      SET(LIB ${A})
      SET(FIRST_ARG TRUE)
    ELSE()
      SET(DEPS ${DEPS} ${A})
    ENDIF()
  ENDFOREACH()
  SET(BUNDLE_TARGET_DEPS_${LIB} ${BUNDLE_TARGET_DEPS_${LIB}} ${DEPS} CACHE INTERNAL "" FORCE)
  _target_link_libraries(${ARGN})
ENDFUNCTION(TARGET_LINK_LIBRARIES)

ENDIF(NOT APPLE)

MACRO(BUNDLE_INIT)
  FOREACH(T ${BUNDLE_TARGETS})
    SET(BUNDLE_${T}_COMP CACHE INTERNAL "" FORCE)
    SET(BUNDLE_${T}_COMP CACHE INTERNAL "" FORCE)
    SET(BUNDLE_TARGET_DEPS_${T} CACHE INTERNAL "" FORCE)
  ENDFOREACH()
  SET(BUNDLE_TARGETS CACHE INTERNAL "" FORCE)
  FOREACH(CMP ${CPACK_COMPONENTS_ALL})
    STRING(TOUPPER ${CMP} C)
    SET(CPACK_COMPONENT_${C}_DISPLAY_NAME CACHE INTERNAL "" FORCE)
    SET(CPACK_COMPONENT_${C}_DESCRIPTION CACHE INTERNAL "" FORCE)
    SET(CPACK_COMPONENT_${C}_GROUP CACHE INTERNAL "" FORCE)
  ENDFOREACH()
  SET(CPACK_COMPONENTS_ALL "" CACHE INTERNAL "")
ENDMACRO(BUNDLE_INIT)

MACRO(BUNDLE_CHECK)
  FOREACH(CMP ${CPACK_COMPONENTS_ALL})
    STRING(TOUPPER ${CMP} C)
    IF(NOT CPACK_COMPONENT_${C}_DISPLAY_NAME) # Check display name
      MESSAGE(FATAL_ERROR "[CPack] ${C}: No display name declared. See BUNDLE_DEFINE_COMPONENT for infos")
    ENDIF(NOT CPACK_COMPONENT_${C}_DISPLAY_NAME)
    IF(NOT CPACK_COMPONENT_${C}_DESCRIPTION) # Check description
      MESSAGE(FATAL_ERROR "[CPack] ${C}: No description declared. See BUNDLE_DEFINE_COMPONENT for infos")
    ENDIF(NOT CPACK_COMPONENT_${C}_DESCRIPTION)
    IF(NOT CPACK_COMPONENT_${C}_GROUP) # Check group
      MESSAGE(FATAL_ERROR "[CPack] ${C}: No group declared. See BUNDLE_DEFINE_COMPONENT for infos")
    ENDIF(NOT CPACK_COMPONENT_${C}_GROUP)
  ENDFOREACH()
ENDMACRO(BUNDLE_CHECK)

MACRO(BUNDLE_DEFINE_COMPONENT component display_name description group)
  STRING(TOUPPER ${component} C)
  SET(CPACK_COMPONENT_${C}_DISPLAY_NAME ${display_name} CACHE INTERNAL "")
  SET(CPACK_COMPONENT_${C}_DESCRIPTION ${description} CACHE INTERNAL "")
  SET(CPACK_COMPONENT_${C}_GROUP ${group} CACHE INTERNAL "")
ENDMACRO(BUNDLE_DEFINE_COMPONENT)

MACRO(BUNDLE_GENERATE_DEPS)
  FOREACH(T ${BUNDLE_TARGETS})
    SET(CMP_SRC ${BUNDLE_${T}_COMP})
    FOREACH(D ${BUNDLE_TARGET_DEPS_${T}})
      SET(CMP_TGTS ${CMP_TGTS} ${BUNDLE_${D}_COMP})
    ENDFOREACH()

    STRING(TOUPPER ${CMP_SRC} C_SRC)
    SET(CPACK_COMPONENT_${C_SRC}_DEPENDS ${CMP_TGTS} CACHE INTERNAL "")

  ENDFOREACH()
ENDMACRO(BUNDLE_GENERATE_DEPS)

MACRO(DISABLE_COMPILER_WARNINGS)
  IF(MSVC)
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    STRING(REGEX REPLACE "/W[0-9]" "/W0" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  ELSE(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w ")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w ")
  ENDIF(MSVC)
ENDMACRO()

## -----------------------------------------------------------------------------------------------
## Windows-specific macros
## -----------------------------------------------------------------------------------------------
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

ENDIF()

## -----------------------------------------------------------------------------------------------
## Mac OS-specific macros
## -----------------------------------------------------------------------------------------------
IF(APPLE)

# Find a library matching a pattern in common cmake library paths.
# List of matching file will be set into the out_var variable.
# Note: if no matching library is found, outvar will be set to ${out_var}-NOTFOUND
MACRO(FIND_LIBRARY_GLOB_DIR out_var pattern dir)
  SET(SEARCH_DIRS $ENV{CMAKE_LIBRARY_PATH} ${dir} ${CMAKE_SYSTEM_LIBRARY_PATH})
  FOREACH(ITP ${SEARCH_DIRS})
    STRING(REPLACE "\\" "/" P "${ITP}")
    FILE(GLOB RES "${P}/${pattern}")
    IF(RES)
      SET(${out_var} ${RES})
    ENDIF()
  ENDFOREACH()
IF(NOT ${out_var})
  SET(${out_var} "${out_var}-NOTFOUND")
ENDIF()
ENDMACRO()

# Find a library matching a pattern in common cmake library paths.
# List of matching file will be set into the out_var variable.
# Note: if no matching library is found, outvar will be set to ${out_var}-NOTFOUND
MACRO(FIND_LIBRARY_GLOB out_var pattern)
  FIND_LIBRARY_GLOB_DIR(${out_var} ${pattern} "")
ENDMACRO()

# Install external libraries along with a given sotfware component
# out_var is the name of the variable containing libraries path.
MACRO(INSTALL_EXTERNAL_LIB var_name component)
  IF(${var_name})
    FOREACH(ITL ${${var_name}})
      IF(IS_SYMLINK ${ITL})
          GET_FILENAME_COMPONENT(REAL_LIB ${ITL} REALPATH)
          INSTALL(FILES ${REAL_LIB} DESTINATION ${TulipLibInstallDir} COMPONENT ${component})
      ELSE()
          INSTALL(FILES ${ITL} DESTINATION ${TulipLibInstallDir} COMPONENT ${component})
      ENDIF()
    ENDFOREACH()
  ELSE()
    MESSAGE("${var_name} could not be located and will not be installed with package")
  ENDIF()
ENDMACRO()

MACRO(FIND_LIBRARY_DEPENDENCIES out_var library)
      EXECUTE_PROCESS(COMMAND otool -L ${library} COMMAND grep "dylib \\|/4/Qt" COMMAND awk "{print $1}" OUTPUT_VARIABLE OUTPUT RESULT_VARIABLE RESULT)
      #Transform the output to list
      STRING(REPLACE "\n" ";" DEP_LIST ${OUTPUT})
      #ADD filter on system var and try to locate the lib.
      #The list of uninstalled libraries.
      SET(UNINSTALLED_LIBS_REGEXP "(.*libSystem.*dylib)|(.*libobjc.*dylib)|(.*libgcc_s.*dylib)|(.*libicucore.*dylib)|(.*libstdc\\+\\+.*dylib)|(.*libxml2.*dylib)|(.*libtulip.*)|^(@executable_path/../Frameworks)")
      FOREACH(ITL ${DEP_LIST})
          STRING(REGEX MATCH ${UNINSTALLED_LIBS_REGEXP} OUT ${ITL})
          #IF(OUT)
          #    LIST(REMOVE_ITEM DEP_LIST ${ITL})
          #ELSE()
          IF(NOT OUT)
              #try to find the library before copying it
              FIND_LIBRARY_GLOB_DIR( FILE_PATH ${ITL} "/;/Library/Frameworks/;")
              IF(FILE_PATH)
                  SET(FILTERED_DEPS ${FILTERED_DEPS} ${FILE_PATH})
              ENDIF()
          ENDIF()
      ENDFOREACH()
      LIST(REMOVE_DUPLICATES FILTERED_DEPS)
      SET(${out_var} ${FILTERED_DEPS})
ENDMACRO()
ENDIF()
