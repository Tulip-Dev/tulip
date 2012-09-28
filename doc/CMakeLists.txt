ADD_SUBDIRECTORY(python)
 
## ========================================================
## Custom target for documentation
## ========================================================
ADD_CUSTOM_TARGET(doc
  ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/doc/html
  COMMAND doxygen tulip.Doxyfile
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/doc/doxygen/style/navtree.css ${CMAKE_SOURCE_DIR}/doc/html
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

