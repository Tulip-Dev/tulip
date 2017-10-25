TULIP_INSTALL_PYTHON_FILES(tulip/plugins ${CMAKE_CURRENT_SOURCE_DIR}/import
                                         ${CMAKE_CURRENT_SOURCE_DIR}/color
                                         ${CMAKE_CURRENT_SOURCE_DIR}/general)


ADD_CUSTOM_TARGET(copyTulipPythonPlugins ALL
                                         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${TULIP_PYTHON_FOLDER}/plugins/
                                         COMMAND ${CMAKE_COMMAND} -E remove ${TULIP_PYTHON_FOLDER}/plugins/CMakeLists.txt)
IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  ADD_DEPENDENCIES(tulip-core-wheel copyTulipPythonPlugins)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
