INSTALL(FILES fontb.ttf
              font.ttf
              fa-brands-400.ttf
              fa-brands-400.woff
              fa-brands-400.woff2
              fa-regular-400.ttf
              fa-regular-400.woff
              fa-regular-400.woff2
              fa-solid-900.ttf
              fa-solid-900.woff
              fa-solid-900.woff2
              materialdesignicons-webfont.ttf
              materialdesignicons-webfont.woff
              materialdesignicons-webfont.woff2
              halfCylinderTexture.png
              cylinderTexture.png
              roundTexture.png
        DESTINATION ${TulipBitmapInstallDir}
        COMPONENT tulip_ogl)

INSTALL(DIRECTORY fonts
        COMPONENT tulip_ogl
        DESTINATION ${TulipBitmapInstallDir}
        PATTERN ".svn" EXCLUDE)

IF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
ADD_CUSTOM_TARGET(copyTulipGuiBitmaps ALL ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fontb.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fontb.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/font.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/font.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-brands-400.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-brands-400.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-brands-400.woff ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-brands-400.woff
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-brands-400.woff2 ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-brands-400.woff2
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-regular-400.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-regular-400.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-regular-400.woff ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-regular-400.woff
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-regular-400.woff2 ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-regular-400.woff2
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-solid-900.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-solid-900.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-solid-900.woff ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-solid-900.woff
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/fa-solid-900.woff2 ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/fa-solid-900.woff2
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/materialdesignicons-webfont.ttf ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/materialdesignicons-webfont.ttf
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/materialdesignicons-webfont.woff ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/materialdesignicons-webfont.woff
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/materialdesignicons-webfont.woff2 ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/materialdesignicons-webfont.woff2
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/halfCylinderTexture.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/halfCylinderTexture.png
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/cylinderTexture.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/cylinderTexture.png
                                      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/roundTexture.png ${TULIPGUI_PYTHON_FOLDER}/share/bitmaps/roundTexture.png)
ADD_DEPENDENCIES(tulip-gui-wheel copyTulipGuiBitmaps)
ENDIF(TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
