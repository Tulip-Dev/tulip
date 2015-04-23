# this doc is only build & installed on linux or MacOSX
# when doxygen is available
# check for dot to enable the generation of class diagrams
IF(DOXYGEN_DOT_FOUND)
  SET(DOXY_HAVE_DOT "YES")
ELSE(DOXYGEN_DOT_FOUND)
  SET(DOXY_HAVE_DOT "NO")
  MESSAGE(AUTHOR_WARNING "dot tool not found: doxygen diagrams and graphs will not be generated")
ENDIF(DOXYGEN_DOT_FOUND)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/tulip.doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/tulip.doxyfile)

ADD_CUSTOM_TARGET(doxydoc ALL
  ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/html
  COMMAND doxygen ${CMAKE_CURRENT_BINARY_DIR}/tulip.doxyfile
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/doc/html ${CMAKE_CURRENT_BINARY_DIR}/html
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/style/doxygen.css ${CMAKE_CURRENT_BINARY_DIR}/html
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/style/navtree.css ${CMAKE_CURRENT_BINARY_DIR}/html
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/style/tabs.css ${CMAKE_CURRENT_BINARY_DIR}/html
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/doc/html
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} VERBATIM)

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html 
  DESTINATION ${TulipShareInstallDir}/doc/doxygen
  COMPONENT doxygen_doc)
