# Turn back the CMAKE_LIBRARY_PATH string into a list
STRING(REPLACE "," ";" CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}")

# Fix possible sphinx-build issue on Windows
IF(WIN32)
  SET(ENV{PATH} "${CMAKE_LIBRARY_PATH};$ENV{PATH}")
ENDIF(WIN32)

# Fix an encoding issue on Mac OS
IF(APPLE)
  SET(ENV{LC_ALL} "en_EN.UTF-8")
ENDIF(APPLE)

EXECUTE_PROCESS(COMMAND ${SPHINX_EXECUTABLE} -c ${BIN_DIR} -b html -E -d ${BIN_DIR}/doctrees ${SRC_DIR} ${BIN_DIR}/html)
