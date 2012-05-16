ADD_SUBDIRECTORY(gzstream)
ADD_SUBDIRECTORY(ftgl)
ADD_SUBDIRECTORY(OGDF)
ADD_SUBDIRECTORY(quazip)
ADD_SUBDIRECTORY(yajl)
ADD_SUBDIRECTORY(qt_models_tests)
ADD_SUBDIRECTORY(qxt)

IF(WIN32)
  SET(DBUS_LIBRARY dbus-1)
  SET(DBUS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dbus/libdbus)
  ADD_SUBDIRECTORY(dbus)
  ADD_SUBDIRECTORY(QtDBus)
ENDIF(WIN32)

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
  INSTALL_EXTERNAL_LIB("libiconv-2.dll" iconv)
  INSTALL_EXTERNAL_LIB("zlib1.dll" zlib)
  INSTALL_EXTERNAL_LIB("freetype*.dll" freetype)
  INSTALL_EXTERNAL_LIB("QtCore4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtGui4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtNetwork4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtOpenGL4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtXml4.dll" qt)
  INSTALL_EXTERNAL_LIB("QtWebKit4.dll" qt)
  INSTALL_EXTERNAL_LIB("glew*.dll" glew)
  INSTALL_EXTERNAL_LIB("jpeg*.dll" jpeg)
  INSTALL_EXTERNAL_LIB("libpng*.dll" png)
  INSTALL_EXTERNAL_LIB("python2*.dll" python)
  INSTALL_EXTERNAL_LIB("libintl*.dll" intl)

ENDIF()
