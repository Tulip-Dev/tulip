DISABLE_COMPILER_WARNINGS()

ADD_CORE_FILES(CMakeLists.txt)

ADD_SUBDIRECTORY(gzstream)
ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(libqhull)
ADD_SUBDIRECTORY(yajl)

IF(NOT BUILD_CORE_ONLY)
ADD_SUBDIRECTORY(ftgl)
IF(NOT QUAZIP_FOUND)
  ADD_SUBDIRECTORY(quazip)
ENDIF(NOT QUAZIP_FOUND)
ADD_SUBDIRECTORY(qxt)

IF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
  IF(SIP_OK AND NOT SIP_FOUND)
    ADD_SUBDIRECTORY(sip-${SIP_VERSION_THIRDPARTY})
  ENDIF(SIP_OK AND NOT SIP_FOUND)
ENDIF(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
ENDIF(NOT BUILD_CORE_ONLY)

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

    # base runtime
    INSTALL_EXTERNAL_LIB("libgcc_s_*.dll" runtime)
    INSTALL_EXTERNAL_LIB("libstdc++-*.dll" runtime)

    # install libintl and its dependency libiconv if found in the MinGW build environment
    IF(LIBINTL_FOUND)
      INSTALL_EXTERNAL_LIB("libintl*.dll" intl)
      INSTALL_EXTERNAL_LIB("*iconv.dll" iconv)
    ENDIF(LIBINTL_FOUND)

    # install pthread lib in all cases as it is a frequent dependency
    INSTALL_EXTERNAL_LIB("*pthread*.dll" runtime)

    # OpenMP runtime
    IF(OPENMP_FOUND)
      INSTALL_EXTERNAL_LIB("libgomp-*.dll" runtime)
    ENDIF(OPENMP_FOUND)

    # dependencies for old MinGW compilers
    IF(GCXX_VERSION VERSION_LESS 4.8)
      INSTALL_EXTERNAL_LIB("mingwm*.dll" runtime)
      INSTALL_EXTERNAL_LIB("libiconv-2.dll" iconv)
    ENDIF(GCXX_VERSION VERSION_LESS 4.8)

  ENDIF(MINGW)

  INSTALL_EXTERNAL_LIB_IF_NEEDED(${ZLIB_LIBRARY} zlib)

IF(NOT BUILD_CORE_ONLY)
  INSTALL_EXTERNAL_LIB_IF_NEEDED(${GLEW_LIBRARY} glew)
  INSTALL_EXTERNAL_LIB_IF_NEEDED(${FREETYPE_LIBRARY} freetype)
  INSTALL_EXTERNAL_LIB_IF_NEEDED(${JPEG_LIBRARY} jpeg)
  INSTALL_EXTERNAL_LIB_IF_NEEDED(${PNG_LIBRARY} png)

  IF(USE_QT4)
    INSTALL_EXTERNAL_LIB("QtCore4.dll" qt)
    INSTALL_EXTERNAL_LIB("QtGui4.dll" qt)
    INSTALL_EXTERNAL_LIB("QtNetwork4.dll" qt)
    INSTALL_EXTERNAL_LIB("QtOpenGL4.dll" qt)
    INSTALL_EXTERNAL_LIB("QtXml4.dll" qt)
    INSTALL_EXTERNAL_LIB("QtWebKit4.dll" qt)

    IF(CMAKE_DEBUG_MODE OR MSVC)
      INSTALL_EXTERNAL_LIB("QtCored4.dll" qt)
      INSTALL_EXTERNAL_LIB("QtGuid4.dll" qt)
      INSTALL_EXTERNAL_LIB("QtNetworkd4.dll" qt)
      INSTALL_EXTERNAL_LIB("QtOpenGLd4.dll" qt)
      INSTALL_EXTERNAL_LIB("QtXmld4.dll" qt)
      INSTALL_EXTERNAL_LIB("QtWebKitd4.dll" qt)
    ENDIF(CMAKE_DEBUG_MODE OR MSVC)

    IF(QT_PHONON_LIBRARY)
      INSTALL_EXTERNAL_LIB("phonon4.dll" qt)
      IF(CMAKE_DEBUG_MODE OR MSVC)
        INSTALL_EXTERNAL_LIB("phonond4.dll" qt)
      ENDIF(CMAKE_DEBUG_MODE OR MSVC)
    ENDIF(QT_PHONON_LIBRARY)
  ENDIF(USE_QT4)

  IF(USE_QT5)
    INSTALL_EXTERNAL_LIB("Qt5Core.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Gui.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Network.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5OpenGL.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Xml.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5WebKit.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5WebKitWidgets.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Widgets.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Multimedia.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5MultimediaWidgets.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5PrintSupport.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Sensors.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Sql.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Quick.dll" qt)
    INSTALL_EXTERNAL_LIB("Qt5Qml.dll" qt)

    IF(CMAKE_DEBUG_MODE OR MSVC)
      INSTALL_EXTERNAL_LIB("Qt5Cored.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Guid.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Networkd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5OpenGLd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Xmld.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5WebKitd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5WebKitWidgetsd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Widgetsd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Multimediad.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5MultimediaWidgetsd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5PrintSupportd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Sensorsd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Sqld.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Quickd.dll" qt)
      INSTALL_EXTERNAL_LIB("Qt5Qmld.dll" qt)
    ENDIF(CMAKE_DEBUG_MODE OR MSVC)

    INSTALL_EXTERNAL_LIB("icuin*.dll" qt)
    INSTALL_EXTERNAL_LIB("icuuc*.dll" qt)
    INSTALL_EXTERNAL_LIB("icudt*.dll" qt)

    IF(QT_VERSION VERSION_LESS 5.2)
      INSTALL_EXTERNAL_LIB("Qt5V8.dll" qt)
      IF(CMAKE_DEBUG_MODE OR MSVC)
        INSTALL_EXTERNAL_LIB("Qt5V8d.dll" qt)
      ENDIF(CMAKE_DEBUG_MODE OR MSVC)
    ELSE(QT_VERSION VERSION_LESS 5.2)
      INSTALL_EXTERNAL_LIB("Qt5Positioning.dll" qt)
      IF(CMAKE_DEBUG_MODE OR MSVC)
        INSTALL_EXTERNAL_LIB("Qt5Positioningd.dll" qt)
      ENDIF(CMAKE_DEBUG_MODE OR MSVC)
    ENDIF(QT_VERSION VERSION_LESS 5.2)

    IF(QT_VERSION VERSION_GREATER 5.3)
      INSTALL_EXTERNAL_LIB("Qt5WebChannel.dll" qt)
      IF(CMAKE_DEBUG_MODE OR MSVC)
        INSTALL_EXTERNAL_LIB("Qt5WebChanneld.dll" qt)
      ENDIF(CMAKE_DEBUG_MODE OR MSVC)
    ENDIF(QT_VERSION VERSION_GREATER 5.3)


  ENDIF(USE_QT5)
ENDIF(NOT BUILD_CORE_ONLY)

ENDIF(WIN32)
