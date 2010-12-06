FILE(GLOB COLORSCALES *.png)

INSTALL(FILES ${COLORSCALES}
        DESTINATION ${TulipBitmapInstallDir}/colorscales
        COMPONENT tulip_app)
