INCLUDE(GetPrerequisites)

SET(TULIP_NATIVE_PYTHON_PATH "${CWD}/../../tulip-core/tulip_module/tulip/native/")
SET(TULIPOGL_NATIVE_PYTHON_PATH "${CWD}/tulipogl/native/")
SET(TULIPGUI_NATIVE_PYTHON_PATH "${CWD}/tulipgui/native/")

SET(TULIPOGL_NATIVE_PYTHON_MODULE "${TULIPOGL_NATIVE_PYTHON_PATH}/_tulipogl.pyd")
SET(TULIPGUI_NATIVE_PYTHON_MODULE "${TULIPGUI_NATIVE_PYTHON_PATH}/_tulipgui.pyd")

STRING(REPLACE ";" "\;" LIBRARY_PATHS "${LIBRARY_PATHS}")

SET(DIRS "${TULIP_NATIVE_PYTHON_PATH}\;${TULIPOGL_NATIVE_PYTHON_PATH}\;${TULIPGUI_NATIVE_PYTHON_PATH}\;${LIBRARY_PATHS}")

MESSAGE("Gathering dll dependencies for tulipogl and tulipgui Python modules ...")

GET_PREREQUISITES(${TULIPOGL_NATIVE_PYTHON_MODULE} TULIPOGL_NATIVE_PYTHON_MODULE_TL_DEPS 1 0 "" ${DIRS})

GET_PREREQUISITES(${TULIPGUI_NATIVE_PYTHON_MODULE} TULIPGUI_NATIVE_PYTHON_MODULE_TL_DEPS 1 0 "" ${DIRS})

# The DLL_DEP_RESOLVED variable is not set when calling GP_RESOLVE_ITEM
# even if the item is correctly resolved (I added some debug messages to GetPrerequisites.cmake to verify).
# My guess is PARENT_SCOPE is not honored when executing cmake in script mode ($ cmake -P ...).
# Hopefully, CMake provides a hook mechanism called at the end of the GP_RESOLVE_ITEM process
# that enables us to get the resolved item

# Store the resolved top level items for the tulipogl module in an environment variable (as PARENT_SCOPE does not seem
# to work when executing CMake in script mode ($ cmake -P ...))
FUNCTION(gp_resolve_item_override context item exepath dirs resolved_item_var resolved)
  IF(DEFINED ENV{TULIPOGL_NATIVE_PYTHON_MODULE_DEPS})
   SET(ENV{TULIPOGL_NATIVE_PYTHON_MODULE_DEPS} "$ENV{TULIPOGL_NATIVE_PYTHON_MODULE_DEPS};${${resolved_item_var}}")
  ELSE()
   SET(ENV{TULIPOGL_NATIVE_PYTHON_MODULE_DEPS} "${${resolved_item_var}}")
  ENDIF()
ENDFUNCTION()

# Append the top level prerequisites items to the TULIPOGL_NATIVE_PYTHON_MODULE_DEPS list
# and resolve them (get their absolute path)
FOREACH(DLL_DEP ${TULIPOGL_NATIVE_PYTHON_MODULE_TL_DEPS})
  IF(NOT "${DLL_DEP}" MATCHES "^python[0-9][0-9].dll$")
    LIST(APPEND TULIPOGL_NATIVE_PYTHON_MODULE_DEPS ${DLL_DEP})
    GP_RESOLVE_ITEM(${TULIPOGL_NATIVE_PYTHON_MODULE} ${DLL_DEP} "" ${DIRS} DLL_DEP_RESOLVED)
  ENDIF()
ENDFOREACH()

# Store the resolved top level items for the tulipgui module in an environment variable (as PARENT_SCOPE does not seem
# to work when executing CMake in script mode ($ cmake -P ...)
FUNCTION(gp_resolve_item_override context item exepath dirs resolved_item_var resolved)
  IF(DEFINED ENV{TULIPGUI_NATIVE_PYTHON_MODULE_DEPS})
   SET(ENV{TULIPGUI_NATIVE_PYTHON_MODULE_DEPS} "$ENV{TULIPGUI_NATIVE_PYTHON_MODULE_DEPS};${${resolved_item_var}}")
  ELSE()
   SET(ENV{TULIPGUI_NATIVE_PYTHON_MODULE_DEPS} "${${resolved_item_var}}")
  ENDIF()
ENDFUNCTION()

# Append the top level prerequisites items to the TULIPGUI_NATIVE_PYTHON_MODULE_DEPS list
# and resolve them (get their absolute path)
FOREACH(DLL_DEP ${TULIPGUI_NATIVE_PYTHON_MODULE_TL_DEPS})
  IF(NOT "${DLL_DEP}" MATCHES "^python[0-9][0-9].dll$")
    LIST(APPEND TULIPGUI_NATIVE_PYTHON_MODULE_DEPS ${DLL_DEP})
    GP_RESOLVE_ITEM(${TULIPGUI_NATIVE_PYTHON_MODULE} ${DLL_DEP} "" ${DIRS} DLL_DEP_RESOLVED)
  ENDIF()
ENDFOREACH()

FUNCTION(gp_resolve_item_override context item exepath dirs resolved_item_var resolved)
ENDFUNCTION()

# Get recusively prerequisites of top level prerequisites for the tulipogl module and append them to the TULIPOGL_NATIVE_PYTHON_MODULE_DEPS list
FOREACH(DLL_DEP $ENV{TULIPOGL_NATIVE_PYTHON_MODULE_DEPS})
  GET_PREREQUISITES(${DLL_DEP} DLL_DEP_DEPS 1 1 "" ${DIRS})
  FOREACH(DLL_DEP_DEP ${DLL_DEP_DEPS})
    LIST(APPEND TULIPOGL_NATIVE_PYTHON_MODULE_DEPS ${DLL_DEP_DEP})
  ENDFOREACH()
ENDFOREACH()

# Get recusively prerequisites of top level prerequisites for the tulipgui module and append them to the TULIPGUI_NATIVE_PYTHON_MODULE_DEPS list
FOREACH(DLL_DEP $ENV{TULIPGUI_NATIVE_PYTHON_MODULE_DEPS})
  GET_PREREQUISITES(${DLL_DEP} DLL_DEP_DEPS 1 1 "" ${DIRS})
  FOREACH(DLL_DEP_DEP ${DLL_DEP_DEPS})
    LIST(APPEND TULIPGUI_NATIVE_PYTHON_MODULE_DEPS ${DLL_DEP_DEP})
  ENDFOREACH()
ENDFOREACH()

# Remove duplicate entries in the TULIPOGL_NATIVE_PYTHON_MODULE_DEPS list
LIST(REMOVE_DUPLICATES TULIPOGL_NATIVE_PYTHON_MODULE_DEPS)

# Remove duplicate entries in the TULIPGUI_NATIVE_PYTHON_MODULE_DEPS list
LIST(REMOVE_DUPLICATES TULIPGUI_NATIVE_PYTHON_MODULE_DEPS)

FUNCTION(gp_resolve_item_override context item exepath dirs resolved_item_var resolved)
  IF("${context}" MATCHES ".*tulipogl.*")
    IF(NOT EXISTS "${TULIP_NATIVE_PYTHON_PATH}/${item}" AND NOT EXISTS "${TULIPOGL_NATIVE_PYTHON_PATH}/${item}")
      MESSAGE("Copying ${item} inside tulipogl Python module native folder")
      FILE(COPY ${${resolved_item_var}} DESTINATION ${TULIPOGL_NATIVE_PYTHON_PATH})
    ENDIF()
  ENDIF()
  IF("${context}" MATCHES ".*tulipgui.*")
    IF(NOT EXISTS "${TULIP_NATIVE_PYTHON_PATH}/${item}" AND NOT EXISTS "${TULIPOGL_NATIVE_PYTHON_PATH}/${item}" AND
       NOT EXISTS "${TULIPGUI_NATIVE_PYTHON_PATH}/${item}")
      MESSAGE("Copying ${item} inside tulipgui Python module native folder")
      FILE(COPY ${${resolved_item_var}} DESTINATION ${TULIPGUI_NATIVE_PYTHON_PATH})
    ENDIF()
  ENDIF()
ENDFUNCTION()

# Finally, resolve all prerequisites for the tulipogl Python module and copy them to the module native folder
FOREACH(DLL_DEP ${TULIPOGL_NATIVE_PYTHON_MODULE_DEPS})
  GP_RESOLVE_ITEM(${TULIPOGL_NATIVE_PYTHON_MODULE} ${DLL_DEP} "" ${DIRS} DLL_DEP_RESOLVED)
ENDFOREACH()

# Finally, resolve all prerequisites for the tulipgui Python module and copy them to the module native folder
FOREACH(DLL_DEP ${TULIPGUI_NATIVE_PYTHON_MODULE_DEPS})
  GP_RESOLVE_ITEM(${TULIPGUI_NATIVE_PYTHON_MODULE} ${DLL_DEP} "" ${DIRS} DLL_DEP_RESOLVED)
ENDFOREACH()
