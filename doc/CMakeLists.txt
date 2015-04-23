SET(BUILD_DOC ON CACHE BOOL "Do you want to build the documentation ? [ON|OFF]")

#Only
IF(BUILD_DOC)
  # Unset the previous values of the CMake cache variable SPHINX_EXECUTABLE
  # in case the value of PYTHON_EXECUTABLE CMake variable changed
  UNSET(SPHINX_EXECUTABLE CACHE)
  SET(Sphinx_DIR ${CMAKE_CURRENT_SOURCE_DIR}/python)
  MARK_AS_ADVANCED(Sphinx_DIR)
  FIND_PACKAGE(Sphinx)
  IF(SPHINX_FOUND)
    IF(BUILD_PYTHON_COMPONENTS)
      ADD_SUBDIRECTORY(python)
    ENDIF(BUILD_PYTHON_COMPONENTS)
    ADD_SUBDIRECTORY(user)
    ADD_SUBDIRECTORY(developer)
  ELSE(SPHINX_FOUND)
    MESSAGE(AUTHOR_WARNING "Sphinx not found: unable to generate the documentation.")
  ENDIF(SPHINX_FOUND)

  IF(LINUX OR APPLE)
    FIND_PACKAGE(Doxygen)
    IF(DOXYGEN_FOUND)
      ADD_SUBDIRECTORY(doxygen)
    ELSE(DOXYGEN_FOUND)
      MESSAGE(AUTHOR_WARNING "doxygen not found: unable to generate the documentation.")
    ENDIF(DOXYGEN_FOUND)
  ENDIF(LINUX OR APPLE)
ENDIF(BUILD_DOC)
