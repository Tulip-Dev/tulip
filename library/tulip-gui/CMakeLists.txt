IF(WIN32)
    add_compile_definitions("DLL_TULIP_QT")
ENDIF(WIN32)

INSTALL(DIRECTORY samplePictures colorscales
        DESTINATION ${TulipBitmapInstallDir})

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)

