INSTALL(FILES
  startup.gif
  headerlogo.bmp
  logo32x32.bmp
  logo.bmp
  logo32x32.ico
  welcomelogo.bmp
  logotulip.jpg
  logolabri.jpg
  tex_back.png
DESTINATION ${TulipBitmapInstallDir}
COMPONENT tulip_app
)

ADD_SUBDIRECTORY(colorscales)
