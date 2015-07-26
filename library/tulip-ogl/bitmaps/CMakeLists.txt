INSTALL(FILES
  fontb.ttf
  font.ttf 
  fontawesome-webfont.ttf
  halfCylinderTexture.png
  cylinderTexture.png
  roundTexture.png
DESTINATION ${TulipBitmapInstallDir}
COMPONENT tulip_ogl)

INSTALL(DIRECTORY
  fonts
COMPONENT tulip_ogl
DESTINATION ${TulipBitmapInstallDir}
PATTERN ".svn" EXCLUDE)

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
ADD_CUSTOM_TARGET(copyTulipOglBitmaps ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fontb.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fontb.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/font.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/font.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fontawesome-webfont.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fontawesome-webfont.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/halfCylinderTexture.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/halfCylinderTexture.png
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cylinderTexture.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/cylinderTexture.png
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/roundTexture.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/roundTexture.png)
ADD_DEPENDENCIES(tulip-gui-wheel copyTulipOglBitmaps)
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)
