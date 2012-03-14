ADD_SUBDIRECTORY(gzstream)
ADD_SUBDIRECTORY(ftgl)
ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(quazip)
ADD_SUBDIRECTORY(yajl)
ADD_SUBDIRECTORY(qt_models_tests)
ADD_SUBDIRECTORY(qxt)

IF(APPLE)
  SET(DBUS_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/dbus/dbus/lib/libdbus-1.3.dylib)
  SET(DBUS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dbus/libdbus)
ELSEIF(WIN32)
  SET(DBUS_LIBRARY dbus-1)
  SET(DBUS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dbus/libdbus)
ENDIF(APPLE)

IF(WIN32 OR APPLE)
  ADD_SUBDIRECTORY(dbus)
  IF(WIN32)
    ADD_SUBDIRECTORY(QtDBus)
  ENDIF(WIN32)
ENDIF(WIN32 OR APPLE)

IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
IF(SIP_OK AND NOT SIP_FOUND)
ADD_SUBDIRECTORY(sip-4.13.2)
ENDIF(SIP_OK AND NOT SIP_FOUND)
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


  IF(MINGW)
    INSTALL_EXTERNAL_LIB("mingwm*.dll" runtime)
    INSTALL_EXTERNAL_LIB("libgcc_s_dw2-1.dll" runtime)
    INSTALL_EXTERNAL_LIB("libstdc++-*.dll" runtime)
    INSTALL_EXTERNAL_LIB("*pthread*.dll" runtime)
    INSTALL_EXTERNAL_LIB("libgomp-*.dll" runtime)
  ENDIF()
  INSTALL_EXTERNAL_LIB("iconv.dll" iconv)
  INSTALL_EXTERNAL_LIB("zlib1.dll" zlib)
  INSTALL_EXTERNAL_LIB("freetype*.dll" freetype)
  INSTALL_EXTERNAL_LIB("QtCore4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtGui4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtNetwork4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtOpenGL4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtXml4.dll" qt)
  INSTALL_EXTERNAL_LIB("glew*.dll" glew)
  INSTALL_EXTERNAL_LIB("jpeg*.dll" jpeg)
  INSTALL_EXTERNAL_LIB("libpng*.dll" png)
  INSTALL_EXTERNAL_LIB("python2*.dll" python)

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

  FIND_LIBRARY_GLOB_DIR(QTDBUS_LIBFILE "QtDBus.framework/QtDBus" "/Library/Frameworks")
  INSTALL_EXTERNAL_LIB(QTDBUS_LIBFILE dbus)

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
