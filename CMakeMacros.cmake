## -----------------------------------------------------------------------------------------------
## General macros
## -----------------------------------------------------------------------------------------------

# Define a software component. Setting variables for its dependencies, display name, description, etc
# Those meta informations are mainly used when building software bundle using CPack
MACRO(DEFINE_COMPONENT comp display_name desc deps group)
  SET(CPACK_COMPONENTS_ALL
  ${CPACK_COMPONENTS_ALL}
  ${comp})
  STRING(TOUPPER ${comp} CN)
  SET(CPACK_COMPONENT_${CN}_DISPLAY_NAME ${display_name})
  SET(CPACK_COMPONENT_${CN}_DESCRIPTION "${desc}.")
  IF(NOT "${deps}" STREQUAL "")
    SET(CPACK_COMPONENT_${CN}_DEPENDS ${deps})
  ENDIF()
  SET(CPACK_COMPONENT_${CN}_GROUP ${group})
ENDMACRO()

## -----------------------------------------------------------------------------------------------
## Windows-specific macros
## -----------------------------------------------------------------------------------------------
IF(WIN32)

# Find a library matching a pattern in common cmake library paths.
# List of matching file will be set into the out_var variable.
# Note: if no matching library is found, outvar will be set to ${out_var}-NOTFOUND
MACRO(FIND_LIBRARY_GLOB out_var pattern)
  SET(SEARCH_DIRS $ENV{CMAKE_LIBRARY_PATH} ${CMAKE_SYSTEM_LIBRARY_PATH} ${QT_BINARY_DIR})
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

# Install external libraries along with a given sotfware component
# out_var is the name of the variable containing libraries path.
MACRO(INSTALL_EXTERNAL_LIB var_name component)
  IF(${var_name})
    FOREACH(ITL ${${var_name}})
      INSTALL(FILES ${ITL} DESTINATION ${TulipBinInstallDir} COMPONENT ${component})
    ENDFOREACH()
  ELSE()
    MESSAGE("${var_name} could not be located and will not be installed with package")
  ENDIF()
ENDMACRO()

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

MACRO(FIND_LIBRARY_DEPENDANCIES out_var library)
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
