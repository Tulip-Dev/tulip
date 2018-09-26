ADD_CORE_FILES(.)

TULIP_INSTALL_PYTHON_FILES("" ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py)

# tulipplugins.py must be found by python tests and doc
# but it cannot be compiled in the its source dir
# so we will copy it in the current binary dir
ADD_CUSTOM_TARGET(copyTulipPluginsPyInBuild ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py ${CMAKE_CURRENT_BINARY_DIR}/tulipplugins.py)

IF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
  ADD_CUSTOM_TARGET(copyTulipPluginsPy ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py ${TULIP_PYTHON_FOLDER}/../tulipplugins/__init__.py)
  ADD_DEPENDENCIES(wheel copyTulipPluginsPy)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEEL_TARGET)
