#===========================================================================
#
#    Description: Find directories used to build projects using Tulip framework
#    Filename: FindTULIP.cmake
#    Authors: Tulip team
#
#===========================================================================
# Following variables should be added to the CMake build variables:
#
# TULIP_DIR: Root directory for Tulip installation
# TULIP_INCLUDE_DIR: Directory containing include files for Tulip
# TULIP_PLUGINS_PATH: installation path for plugins
# TULIP_LIBRARY: Absolute path of libtulip.
# TULIP_QT4_LIBRARY: Absolute path of libtulip-qt
# TULIP_OGL_LIBRARY: Absolute path of libtulip-ogl
# TULIP_PLUGINS_MANAGER_LIBRARY: Absolute path of libtulip-pluginsmanager
# TULIP_LIBS: Convenience variable containing all the previously mentioned Tulip libraries
# TULIP_VERSION: Tulip release version number: A.B
# TULIP_PLUGIN_VERSION: Tulip plugins release version number: A.B.x
#
# User can specify any of this directory to force using a specific installation.
# If TULIP_DIR is given, it will be used as the root path to find all other libraries. CMake WILL NOT look in other directories if TULIP_DIR is given
# Keep in mind that FindTulip will try to use libraries and include files coming from the same Tulip blundle, it will not fail if some of the libraries are not found (since you may want to only link against libtulip eg)
# FindTulip will only set previously unset variables. User can specify any variable mentioned before manually.

# Include and root directories
IF(TULIP_DIR AND NOT TULIP_INCLUDE_DIR)
  IF(APPLE)
    FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h
              PATHS ${TULIP_DIR}/Contents/include
              NO_DEFAULT_PATH)
  ELSE()
    FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h
              PATHS ${TULIP_DIR}/include
              NO_DEFAULT_PATH)
  ENDIF()
ELSEIF(NOT TULIP_DIR AND NOT TULIP_INCLUDE_DIR)
  IF(APPLE)
    FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h)
  ELSE()
    FIND_PATH(TULIP_INCLUDE_DIR tulip/TulipRelease.h)
  ENDIF()
ENDIF()

SET(TulipVersionSeparator .)
IF(${CMAKE_GENERATOR} MATCHES "Visual Studio" OR ${CMAKE_GENERATOR} MATCHES "NMake Makefiles") #visual studio does not recognize these options
   SET(TulipVersionSeparator _)
ENDIF()

# Tulip version
IF(NOT TULIP_VERSION AND TULIP_INCLUDE_DIR)
FILE(STRINGS ${TULIP_INCLUDE_DIR}/tulip/TulipRelease.h
     TEMPVAR
     REGEX "[0-9]*\\${TulipVersionSeparator}[0-9]*"
     NO_HEX_CONVERSION)

STRING(REGEX MATCH "[0-9]*\\${TulipVersionSeparator}[0-9]*\\${TulipVersionSeparator}[0-9]*"
       TULIP_PLUGIN_VERSION
       ${TEMPVAR})
ENDIF()

# Plugin version
IF(NOT TULIP_VERSION AND TULIP_PLUGIN_VERSION)
STRING(REGEX MATCH "[0-9]*\\${TulipVersionSeparator}[0-9]*"
       TULIP_VERSION
       ${TULIP_PLUGIN_VERSION})
ENDIF()

# No effect if TULIP_DIR was already given
IF(TULIP_INCLUDE_DIR AND NOT TULIP_DIR)
  SET(TULIP_DIR ${TULIP_INCLUDE_DIR}/..)
ENDIF()

# From now on, TULIP_DIR must be set
IF(TULIP_DIR)
  IF(WIN32)
    SET(LIBDIR ${TULIP_DIR}/bin)
  ELSEIF(APPLE)
    SET(LIBDIR ${TULIP_DIR}/Frameworks)
  ELSE()
    SET(LIBDIR ${TULIP_DIR}/lib)
  ENDIF()

  IF(NOT TULIP_LIBRARY)
    FIND_LIBRARY(TULIP_LIBRARY "tulip-${TULIP_VERSION}"
                 PATHS ${LIBDIR}
                 NO_DEFAULT_PATH)
  ENDIF()
  IF(NOT TULIP_OGL_LIBRARY)
    FIND_LIBRARY(TULIP_OGL_LIBRARY "tulip-ogl-${TULIP_VERSION}"
                 PATHS ${LIBDIR}
                 NO_DEFAULT_PATH)
  ENDIF()
  IF(NOT TULIP_QT4_LIBRARY)
    FIND_LIBRARY(TULIP_QT4_LIBRARY "tulip-qt4-${TULIP_VERSION}"
                 PATHS ${LIBDIR}
                 NO_DEFAULT_PATH)
  ENDIF()
  IF(NOT TULIP_PLUGINS_MANAGER_LIBRARY)
    FIND_LIBRARY(TULIP_PLUGINS_MANAGER_LIBRARY "tulip-pluginsmanager-${TULIP_VERSION}"
                 PATHS ${LIBDIR}
                 NO_DEFAULT_PATH)
  ENDIF()

  SET(TULIP_LIBS ${TULIP_LIBRARY} ${TULIP_OGL_LIBRARY} ${TULIP_QT4_LIBRARY})

  IF(NOT TULIP_PLUGINS_PATH)
    SET(TULIP_PLUGINS_PATH ${TULIP_DIR}/lib/tulip)
  ENDIF()
ENDIF()

IF(TULIP_INCLUDE_DIR AND TULIP_DIR AND TULIP_LIBRARY AND TULIP_PLUGIN_VERSION AND TULIP_VERSION AND TULIP_PLUGINS_PATH)
  SET(TULIP_FOUND "YES")
  MARK_AS_ADVANCED(TULIP_DIR)
  MARK_AS_ADVANCED(TULIP_INCLUDE_DIR)
  MARK_AS_ADVANCED(TULIP_LIBRARY)
  MARK_AS_ADVANCED(TULIP_PLUGIN_VERSION)
  MARK_AS_ADVANCED(TULIP_VERSION)
  MARK_AS_ADVANCED(TULIP_PLUGINS_PATH)
  IF(TULIP_OGL_LIBRARY)
    MARK_AS_ADVANCED(TULIP_OGL_LIBRARY)
    IF(TULIP_QT4_LIBRARY)
      MARK_AS_ADVANCED(TULIP_QT4_LIBRARY)
      IF(TULIP_PLUGINS_MANAGER_LIBRARY)
        MARK_AS_ADVANCED(TULIP_PLUGINS_MANAGER_LIBRARY)
      ENDIF()
    ENDIF()
  ENDIF()
ENDIF()

IF(NOT TULIP_FOUND)
  SET(TULIP_DIR "" CACHE PATH "Root of tulip install tree.")
  MESSAGE("tulip installation was not found. Please provide TULIP_DIR:")
  MESSAGE("  - through the GUI when working with ccmake, ")
  MESSAGE("  - as a command line argument when working with cmake e.g. ")
  MESSAGE("    cmake .. -DTULIP_DIR:PATH=/usr/local/tulip ")
  MESSAGE("Note: the following message is triggered by cmake on the first ")
  MESSAGE("    undefined necessary PATH variable (e.g. TULIP_INCLUDE_DIR).")
  MESSAGE("    Providing TULIP_DIR (as above described) is probably the")
  MESSAGE("    simplest solution unless you have a really customized/odd")
  MESSAGE("    tulip installation...")
ENDIF(NOT TULIP_FOUND)

MACRO (TULIP_QT4_WRAP_UI outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}Data.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (TULIP_QT4_WRAP_UI)

MACRO (TULIP_QT4_WRAP_UI_IN_INCLUDE outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/../include/${outfile}Data.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (TULIP_QT4_WRAP_UI_IN_INCLUDE)

MACRO (TULIP_QT4_WRAP_UI_IN_INCLUDE_TULIP outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/${outfile}Data.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (TULIP_QT4_WRAP_UI_IN_INCLUDE_TULIP)

MACRO (TULIP_WRAP_QT_PLUGINS_IN_INCLUDE_TULIP outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    STRING(TOLOWER ${outfile} outfile_lower)
    SET(outfile_lower ${CMAKE_CURRENT_BINARY_DIR}/../include/tulip/${outfile_lower}.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile_lower}
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${infile} ${outfile_lower}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile_lower})
  ENDFOREACH (it)
ENDMACRO (TULIP_WRAP_QT_PLUGINS_IN_INCLUDE_TULIP)

MACRO (TULIP_QT4_WRAP_CPP outfiles )
  QT4_GET_MOC_FLAGS(moc_flags)
  QT4_EXTRACT_OPTIONS(moc_files moc_options ${ARGN})
  FOREACH (it ${moc_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(it ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/moc_${outfile}.cpp)
    QT4_CREATE_MOC_COMMAND(${it} ${outfile} "${moc_flags}" "${moc_options}")
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)
ENDMACRO (TULIP_QT4_WRAP_CPP)
