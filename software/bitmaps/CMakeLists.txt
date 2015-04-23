INSTALL(FILES
logo.bmp
welcomelogo.bmp
DESTINATION ${TulipBitmapInstallDir}
COMPONENT tulip_app
)

INSTALL(DIRECTORY samplePictures DESTINATION ${TulipBitmapInstallDir}
  COMPONENT tulip_app
  PATTERN ".svn" EXCLUDE)

ADD_SUBDIRECTORY(colorscales)
