INSTALL(FILES logo.bmp welcomelogo.bmp
        DESTINATION ${TulipBitmapInstallDir}
        COMPONENT tulip_app)

INSTALL(DIRECTORY samplePictures colorscales
        DESTINATION ${TulipBitmapInstallDir}
        COMPONENT tulip_app)
