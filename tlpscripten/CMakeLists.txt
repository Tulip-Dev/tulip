ADD_CORE_FILES(.)

## ========================================================
## Configure file for directories initialization
## ========================================================
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/init_dirs.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/init_dirs.sh" IMMEDIATE @ONLY)

## ========================================================
## copy scripts from src dir to bin dir
## ========================================================
FILE(GLOB SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/*.sh)

ADD_CUSTOM_TARGET(copy_scripts)
FOREACH(script ${SCRIPTS})
  ADD_CUSTOM_COMMAND(TARGET copy_scripts PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                     copy_if_different ${script} ${CMAKE_CURRENT_BINARY_DIR})
ENDFOREACH()

## ========================================================
## invoke main script
## ========================================================
ADD_CUSTOM_TARGET(tlpscripten sh tlpscripten.sh
                  DEPENDS copy_scripts)
