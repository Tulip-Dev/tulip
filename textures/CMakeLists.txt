INSTALL(FILES arrowSprite.png
              candiSprite.png
              edgeBlomSprite.png
              edgeSpring.png
              titlebarGradient.png
              logo32x32.ico
              logolabri.jpg
        DESTINATION ${TulipBitmapInstallDir}
        COMPONENT tulip_textures)

IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
  ADD_CUSTOM_TARGET(copyTextures ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/titlebarGradient.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/titlebarGradient.png
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/logolabri.jpg ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/logolabri.jpg)
  ADD_DEPENDENCIES(tulip-gui-wheel copyTextures)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
