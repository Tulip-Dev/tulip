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
