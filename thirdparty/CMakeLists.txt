ADD_SUBDIRECTORY(gzstream)
ADD_SUBDIRECTORY(ftgl)
ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(quazip)
IF(WIN32)
  ADD_SUBDIRECTORY(dbus)
  ADD_SUBDIRECTORY(qdbus)
ENDIF(WIN32)

IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
  IF(SIP_OK)
    ADD_SUBDIRECTORY(QScintilla-gpl-2.5.1)
    ADD_SUBDIRECTORY(sip-4.12.1)
  ENDIF(SIP_OK)
ENDIF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)

## -----------------------------------------------------------------------------------------------
## Install external libraries.
## Each external library is defined in its own component to ensure easy modifications of the CPack component dependency tree
## -----------------------------------------------------------------------------------------------

IF(WIN32)
## -----------------------------------------------------------------------------------------------
## Windows
## -----------------------------------------------------------------------------------------------

# Note: On Win32 systems, CMake variables doesn't provide the right libraries that have to be included in the bundle. (For instance, Qt variables redirect to .lib and not .dll)
#       We have to manually find those libs by invoking the FIND_LIBRARY_GLOB macro

# Base System
# Libs needed to provide sufficient runtime under Windows environments
  FIND_LIBRARY_GLOB(ICONV_INSTALL_PATH "iconv.dll")
  FIND_LIBRARY_GLOB(MINGWM_INSTALL_PATH "mingwm*.dll")
  FIND_LIBRARY_GLOB(GCC_INSTALL_PATH "libgcc_s_dw2-1.dll")
  FIND_LIBRARY_GLOB(STDCPP_INSTALL_PATH "libstdc++-*.dll")
  FIND_LIBRARY_GLOB(PTHREAD_INSTALL_PATH "*pthread*.dll")
  FIND_LIBRARY_GLOB(GOMP_INSTALL_PATH "libgomp-*.dll")
  INSTALL_EXTERNAL_LIB(ICONV_INSTALL_PATH base_system)
  INSTALL_EXTERNAL_LIB(MINGWM_INSTALL_PATH base_system)
  INSTALL_EXTERNAL_LIB(GCC_INSTALL_PATH base_system)
  INSTALL_EXTERNAL_LIB(STDCPP_INSTALL_PATH base_system)
  INSTALL_EXTERNAL_LIB(PTHREAD_INSTALL_PATH base_system)
  INSTALL_EXTERNAL_LIB(GOMP_INSTALL_PATH base_system)

# zlib
  FIND_LIBRARY_GLOB(ZLIB_INSTALL_PATH "zlib1.dll")
  INSTALL_EXTERNAL_LIB(ZLIB_INSTALL_PATH zlib)

# Qt libraries
  FIND_LIBRARY_GLOB(QTCORE_INSTALL_PATH "QtCore4.dll")
  INSTALL_EXTERNAL_LIB(QTCORE_INSTALL_PATH qt_core)

  FIND_LIBRARY_GLOB(QTGUI_INSTALL_PATH "QtGui4.dll")
  INSTALL_EXTERNAL_LIB(QTGUI_INSTALL_PATH qt_gui)

  FIND_LIBRARY_GLOB(QTNETWORK_INSTALL_PATH "QtNetwork4.dll")
  INSTALL_EXTERNAL_LIB(QTNETWORK_INSTALL_PATH qt_network)

  FIND_LIBRARY_GLOB(QTOPENGL_INSTALL_PATH "QtOpenGL4.dll")
  INSTALL_EXTERNAL_LIB(QTOPENGL_INSTALL_PATH qt_opengl)

  FIND_LIBRARY_GLOB(QTXML_INSTALL_PATH "QtXml4.dll")
  INSTALL_EXTERNAL_LIB(QTXML_INSTALL_PATH qt_xml)

# LibXML
  FIND_LIBRARY_GLOB(XML_INSTALL_PATH "libxml*.dll")
  INSTALL_EXTERNAL_LIB(XML_INSTALL_PATH libxml2)

# Freetype
  FIND_LIBRARY_GLOB(FREETYPE_INSTALL_PATH "freetype*.dll")
  INSTALL_EXTERNAL_LIB(FREETYPE_INSTALL_PATH freetype)

# Glew
  FIND_LIBRARY_GLOB(GLEW_INSTALL_PATH "glew*.dll")
  INSTALL_EXTERNAL_LIB(GLEW_INSTALL_PATH glew)

# JPEG
  FIND_LIBRARY_GLOB(JPEG_INSTALL_PATH "jpeg*.dll")
  INSTALL_EXTERNAL_LIB(JPEG_INSTALL_PATH jpeg)

# PNG
  FIND_LIBRARY_GLOB(PNG_INSTALL_PATH "libpng*.dll")
  INSTALL_EXTERNAL_LIB(PNG_INSTALL_PATH png)

# python
  FIND_LIBRARY_GLOB(PYTHON_INSTALL_PATH "python2*.dll")
  INSTALL_EXTERNAL_LIB(PYTHON_INSTALL_PATH python)

#D-Bus
  FIND_LIBRARY_GLOB(LIBDBUS_PATH "dbus-1.dll")
  FIND_LIBRARY_GLOB(DBUS_DAEMON_PATH "dbus-daemon.exe")
  FIND_LIBRARY_GLOB(DBUS_ENV_PATH "dbus-env.bat")
  FIND_LIBRARY_GLOB(DBUS_LAUNCH_PATH "dbus-launch.exe")
  FIND_LIBRARY_GLOB(EXPAT_PATH "libexpat.dll")

  INSTALL_EXTERNAL_LIB(LIBDBUS_PATH dbus)
  INSTALL_EXTERNAL_LIB(DBUS_DAEMON_PATH dbus)
  INSTALL_EXTERNAL_LIB(DBUS_ENV_PATH dbus)
  INSTALL_EXTERNAL_LIB(DBUS_LAUNCH_PATH dbus)
  INSTALL_EXTERNAL_LIB(EXPAT_PATH dbus)

ELSEIF(APPLE)
## -----------------------------------------------------------------------------------------------
## Mac OS
## -----------------------------------------------------------------------------------------------

# zlib
  INSTALL_EXTERNAL_LIB(ZLIB_LIBRARY zlib)

# Qt libraries
  FIND_LIBRARY_GLOB_DIR(QTCORE_LIBFILE "QtCore.framework/QtCore" "/Library/Frameworks")
  INSTALL_EXTERNAL_LIB(QTCORE_LIBFILE qt_core)

  FIND_LIBRARY_GLOB_DIR(QTGUI_LIBFILE "QtGui.framework/QtGui" "/Library/Frameworks")
  INSTALL_EXTERNAL_LIB(QTGUI_LIBFILE qt_gui)

  FIND_LIBRARY_GLOB_DIR(QTNETWORK_LIBFILE "QtNetwork.framework/QtNetwork" "/Library/Frameworks")
  INSTALL_EXTERNAL_LIB(QTNETWORK_LIBFILE qt_network)

  FIND_LIBRARY_GLOB_DIR(QTOPENGL_LIBFILE "QtOpenGL.framework/QtOpenGL" "/Library/Frameworks")
  INSTALL_EXTERNAL_LIB(QTOPENGL_LIBFILE qt_opengl)

  FIND_LIBRARY_GLOB_DIR(QTXML_LIBFILE "QtXml.framework/QtXml" "/Library/Frameworks")
  INSTALL_EXTERNAL_LIB(QTXML_LIBFILE qt_xml)

#  #Copy Qt image plugins.
#  INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${TulipAppleContentsDir} COMPONENT tulip_gui)
#  #Copy Qt image plugins dependancies
#  FILE(GLOB PLUGINS "${QT_PLUGINS_DIR}/imageformats/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
#  FOREACH(PLUGIN ${PLUGINS})
#    FIND_LIBRARY_DEPENDENCIES(QT_PLUGINS_DEPENDENCIES "${PLUGIN}")
#    INSTALL_EXTERNAL_LIB(QT_PLUGINS_DEPENDENCIES tulip_gui)
#  ENDFOREACH()
  #Copy Qt menu.
  INSTALL(DIRECTORY "${QT_LIBRARY_DIR}/QtGui.framework/Versions/Current/Resources/qt_menu.nib" DESTINATION "${TulipAppleResourcesDir}" COMPONENT qt_gui OPTIONAL)

  INSTALL_EXTERNAL_LIB(FREETYPE_LIBRARY freetype)
  INSTALL_EXTERNAL_LIB(GLEW_LIBRARY glew)
  INSTALL_EXTERNAL_LIB(JPEG_LIBRARY jpeg)

  #Hack PNG_LIBRARY is not the library but the framework.
  IF(IS_DIRECTORY ${PNG_LIBRARY})
    FILE(GLOB PNG_PATH "${PNG_LIBRARY}/libpng*")
  ELSE()
    SET(PNG_PATH ${PNG_LIBRARY})
  ENDIF()
  INSTALL_EXTERNAL_LIB(PNG_PATH png)
ENDIF()
