INSTALL(FILES
  fontb.ttf
  font.ttf 
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
