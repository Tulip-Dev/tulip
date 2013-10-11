SET(BUILD_DOC ON CACHE BOOL "Do you want to build the documentation ? [ON|OFF]")
#Only
IF(BUILD_DOC)
  ADD_SUBDIRECTORY(python)
 
  IF(LINUX)
    FIND_PACKAGE(Doxygen)
    IF(DOXYGEN_FOUND)
## ========================================================
## Custom target for doxygen documentation
## ========================================================
ADD_CUSTOM_TARGET(doc
  ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/doc/html
  COMMAND doxygen tulip.Doxyfile
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc/doxygen/style/navtree.css ${CMAKE_SOURCE_DIR}/doc/html
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} VERBATIM)
    ENDIF(DOXYGEN_FOUND)
  ENDIF(LINUX)
ENDIF(BUILD_DOC)
