#===========================================================================
#
#===========================================================================
#
#    Description: Find the Tulip libraries and include dir
#    Filename: FindTULIP.cmake
#    Authors: Damien Genet
#
#===========================================================================
#
#    Description:
#
#===========================================================================
#
# TULIP_INCLUDE_DIR  - Directories to include to use tulip
# TULIP_LIBRARY      - Files to link against to use tulip
# TULIP_FOUND        - When false, don't try to use tulip
# TULIP_DIR          - (optional) Suggested installation directory to search
# TULIP_VERSION		 - The version of Tulip
# TULIP_CONFIG		 - tulip-config file.
#
# TULIP_DIR can be used to make it simpler to find the various include
# directories and compiled libraries when not installed in the
# usual/well-known directories (e.g. because you made an in tree-source
# compilation or because you installed it in an "unusual" directory).
# Just set TULIP_DIR it to your specific installation directory
#
FIND_PATH(TULIP_INCLUDE_DIR tulip/Graph.h
  PATHS ${TULIP_DIR}/include
        $ENV{TULIP_DIR}/include
	/usr/include
	/usr/local/include
	/opt/local/include
	)

IF(WIN32)
SET(TULIP_PATHS
  ${TULIP_DIR}/bin
  $ENV{TULIP_DIR}/bin
  )
ELSE(WIN32)
SET(TULIP_PATHS
  ${TULIP_DIR}/lib
  $ENV{TULIP_DIR}/lib
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  )
ENDIF(WIN32)

IF(NOT TULIP_CONFIG)
FIND_FILE(TULIP_CONFIG tulip-config
	PATHS ${TULIP_DIR}/bin
       $ENV{TULIP_DIR}/bin	
	)
ENDIF(NOT TULIP_CONFIG)

IF(NOT TULIP_PLUGIN_VERSION AND TULIP_CONFIG)
EXEC_PROGRAM(sh ARGS "${TULIP_CONFIG} --version" OUTPUT_VARIABLE TULIP_PLUGIN_VERSION)
string(SUBSTRING ${TULIP_PLUGIN_VERSION} 0 3 TULIP_VERSION)
ENDIF(NOT TULIP_PLUGIN_VERSION AND TULIP_CONFIG)

IF(NOT TULIP_PLUGINS_PATH AND TULIP_CONFIG)
EXEC_PROGRAM(sh ARGS "${TULIP_CONFIG} --pluginpath" OUTPUT_VARIABLE TULIP_PLUGINS_PATH)
ENDIF(NOT TULIP_PLUGINS_PATH AND TULIP_CONFIG)

FIND_LIBRARY(TULIP_LIBRARY "tulip-${TULIP_VERSION}"
  PATHS ${TULIP_PATHS}
  )

FIND_LIBRARY(TULIP_QT4_LIBRARY "tulip-qt4-${TULIP_VERSION}"
  PATHS ${TULIP_PATHS}
  )

FIND_LIBRARY(TULIP_OGL_LIBRARY "tulip-ogl-${TULIP_VERSION}"
  PATHS ${TULIP_PATHS}
  )

FIND_LIBRARY(TULIP_PLUGINS_MANAGER_LIBRARY "tulip-pluginsmanager-${TULIP_VERSION}"
  PATHS ${TULIP_PATHS}
  )

SET(TULIP_LIBS ${TULIP_LIBRARY}
	       ${TULIP_QT4_LIBRARY}
	       ${TULIP_OGL_LIBRARY}
	       ${TULIP_PLUGINS_MANAGER_LIBRARY}
)

IF( TULIP_INCLUDE_DIR )
  IF( TULIP_LIBRARY )
    IF( TULIP_QT4_LIBRARY )
      IF( TULIP_OGL_LIBRARY )
        IF(TULIP_PLUGINS_MANAGER_LIBRARY)
  	  SET( TULIP_FOUND "YES" )
  	  MARK_AS_ADVANCED( TULIP_CONFIG )
	  MARK_AS_ADVANCED( TULIP_DIR )	
	  MARK_AS_ADVANCED( TULIP_INCLUDE_DIR )
	  MARK_AS_ADVANCED( TULIP_LIBRARY )
	  MARK_AS_ADVANCED( TULIP_OGL_LIBRARY )
	  MARK_AS_ADVANCED( TULIP_QT4_LIBRARY )	  
	  MARK_AS_ADVANCED( TULIP_PLUGINS_MANAGER_LIBRARY )
	  MARK_AS_ADVANCED( TULIP_PLUGIN_VERSION )
	  MARK_AS_ADVANCED( TULIP_VERSION )
	  MARK_AS_ADVANCED( TULIP_PLUGINS_PATH )	  	  	  
		ENDIF( TULIP_PLUGINS_MANAGER_LIBRARY )
      ENDIF( TULIP_OGL_LIBRARY )
    ENDIF( TULIP_QT4_LIBRARY )
  ENDIF( TULIP_LIBRARY )
ENDIF( TULIP_INCLUDE_DIR )

IF( NOT TULIP_FOUND )
  MESSAGE("tulip installation was not found. Please provide TULIP_DIR:")
  MESSAGE("  - through the GUI when working with ccmake, ")
  MESSAGE("  - as a command line argument when working with cmake e.g. ")
  MESSAGE("    cmake .. -DTULIP_DIR:PATH=/usr/local/tulip ")
  MESSAGE("Note: the following message is triggered by cmake on the first ")
  MESSAGE("    undefined necessary PATH variable (e.g. TULIP_INCLUDE_DIR).")
  MESSAGE("    Providing TULIP_DIR (as above described) is probably the")
  MESSAGE("    simplest solution unless you have a really customized/odd")
  MESSAGE("    tulip installation...")
  SET( TULIP_DIR "" CACHE PATH "Root of tulip install tree." )
ENDIF( NOT TULIP_FOUND )


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
      COMMAND cp
      ARGS ${infile} ${outfile_lower}
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
