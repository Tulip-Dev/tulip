INSTALL(DIRECTORY bitmaps/
        DESTINATION ${TulipBitmapInstallDir}
        COMPONENT tulip_app)
ADD_SUBDIRECTORY(crash_handling)
ADD_SUBDIRECTORY(tulip)
