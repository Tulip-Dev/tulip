#

SET(CPACK_COMPONENT_GROUP_THIRDPARTY_DESCRIPTION "Thirdparty libraries.")
SET(CPACK_COMPONENT_GROUP_LIBRARIES_DESCRIPTION "Tulip framework dynamic libraries.")
SET(CPACK_COMPONENT_GROUP_HEADERS_DESCRIPTION "Tulip framework C++ headers.")
SET(CPACK_COMPONENT_GROUP_SOFTWARE_DESCRIPTION "Tulip softwares.")
SET(CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION "Tulip base plugins set.")
SET(CPACK_COMPONENT_GROUP_EXTRAS_DESCRIPTION "Tulip extra files and documentation.")

IF(WIN32) # Windows-specific components

# Those variables point to component dependencies specific to a Win32 environment.
# To avoid declaring common components 2 times (since dependencies list will be different for Mac OS and Win32), we use those variables that will be void if those components do not exist
  SET(W32_BASE_SYSTEM "base_system")
  SET(W32_LIBXML2 "libxml2")
  SET(W32_PYTHON "python")

  DEFINE_COMPONENT(base_system
    "Base system"
    "Core runtime and libraries."
    ""
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(libxml2
    "Libxml2"
    "Libxml2 is the XML C parser and toolkit developed for the Gnome project."
    "base_system"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(python
    "python"
    "Python is a programming language that lets you work more quickly and integrate your systems more effectively."
    ""
    ${THIRDPARTY_GROUP_NAME})

ENDIF(WIN32)

IF(WIN32 OR APPLE) # Components shared between Mac OS and Win32 plateforms

  DEFINE_COMPONENT(dbus
    "DBus"
    "D-Bus is a message bus system, a simple way for applications to talk to one another."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

#-------------------------------------
# Thirdparty
#-------------------------------------
  DEFINE_COMPONENT(zlib
    "zlib" "A Massively Spiffy Yet Delicately Unobtrusive Compression Library"
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(freetype
    "FreeType"
    "A Free, High-Quality, and Portable Font Engine."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(glew
    "Glew"
    "The OpenGL Extension Wrangler Library."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(jpeg
    "libjpeg"
    "IJG is an informal group that writes and distributes a widely used free library for JPEG image compression."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(png
    "libpng"
    "libpng is the official PNG reference library."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(qt_core
    "QtCore"
    "The QtCore module contains core non-GUI functionality."
    "zlib"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(qt_gui
    "QtGui"
    "The QtGui module extends QtCore with GUI functionality."
    "qt_core"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(qt_network
    "QtNetwork"
    "The QtNetwork module provides classes to make network programming easier and portable."
    "qt_core"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(qt_opengl
    "QtOpenGL"
    "The QtOpenGL module offers classes that make it easy to use OpenGL in Qt applications."
    "qt_gui"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(qt_xml
    "QtXml"
    "The QtXml module provides C++ implementations of SAX and DOM."
    "qt_core"
    ${THIRDPARTY_GROUP_NAME})


  DEFINE_COMPONENT(ogdf
    "OGDF"
    "OGDF is a self-contained C++ class library for the automatic layout of diagrams."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(qscintilla2
    "QScintilla"
    "QScintilla is a port to Qt of Neil Hodgson's Scintilla C++ editor control."
    "qt_core"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(ftgl
    "FTGL"
    "FTGL is a free cross-platform Open Source C++ library that uses Freetype2 to simplify rendering fonts in OpenGL applications."
    "${W32_BASE_SYSTEM}"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(gzstream
    "Gzstream"
    "Gzstream is a small C++ library, basically just a wrapper, that provides the functionality of the zlib C-library in a C++ iostream."
    "zlib"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(quazip
    "QuaZIP"
    "QuaZIP is a simple C++ wrapper over Gilles Vollant's ZIP/UNZIP package that can be used to access ZIP archives."
    "qt_core"
    ${THIRDPARTY_GROUP_NAME})

  DEFINE_COMPONENT(sip
    "SIP"
    "SIP is a tool for quickly writing Python modules that interface with C++ and C libraries."
    "${W32_PYTHON}"
    ${THIRDPARTY_GROUP_NAME})

#-------------------------------------
# Tulip libraries
#-------------------------------------
  DEFINE_COMPONENT(tulip_core
    "TulipCore"
    "TulipCore offers base features for advanced graph manipulation."
    "zlib;qt_core"
    ${LIBS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_core_dev
    "TulipCore (headers)"
    "Headers for the TulipCore library."
    "tulip_core"
    ${HEADERS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_core_plugins
    "TulipCore (plugins)"
    "Plugins additions related to the TulipCore library"
    "tulip_core;qt_network"
    ${PLUGINS_GROUP_NAME})


  DEFINE_COMPONENT(tulip_ogl
    "TulipOGL"
    "TulipOGL provides bridges between the TulipCore library and OpenGL to allow 3D visualization of Graph objects."
    "tulip_core;qt_xml;qt_core;glew;jpeg;png;freetype;ftgl;${W32_LIBXML2}"
    ${LIBS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_ogl_dev
    "TulipOGL (headers)"
    "Headers for the TulipOGL library."
    "tulip_ogl"
    ${HEADERS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_ogl_plugins
    "TulipOGL (plugins)"
    "Plugins additions related to the TulipOGL library."
    "tulip_ogl"
    ${PLUGINS_GROUP_NAME})


  DEFINE_COMPONENT(tulip_gui
    "TulipGUI"
    "TulipGUI provides graphical interface features (widgets, plugins) for the Tulip plateform."
    "qt_core;qt_gui;quazip;qt_xml;qt_network;tulip_ogl;tulip_core"
    ${LIBS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_gui_dev
    "TulipGUI (header)"
    "Headers for the TulipGUI library."
    "tulip_gui"
    ${HEADERS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_gui_plugins
    "TulipGUI (plugins)"
    "Plugins additions related to the TulipGUI library."
    "tulip_gui"
    ${PLUGINS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_gui_pythonview
    "Python script view"
    "The python script view is a plugin providing a full Pyhton editor able to modify graphs visualized inside Tulip."
    "tulip_gui"
    ${PLUGINS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_gui_spreadview
    "Spreadsheet view"
    "The spreadsheet view display a tabular view of a graph."
    "tulip_gui"
    ${PLUGINS_GROUP_NAME})


  DEFINE_COMPONENT(tulip_ogdf
    "TulipOGDF"
    "A bridge between OGDF and Tulip libraries."
    "ogdf;tulip_core"
    ${LIBS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_ogdf_dev
    "TulipOGDF (headers)"
    "Headers for the TulipOGDF library."
    "tulip_ogdf"
    ${HEADERS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_ogdf_plugins
    "TulipOGDF (plugins)"
    "Plugins additions related to TulipOGDF"
    "tulip_ogdf"
    ${PLUGINS_GROUP_NAME})

  DEFINE_COMPONENT(tulip3_compat
    "Tulip 3 compatibility layer"
    "Classes kept from Tulip3 for compatibility purposes."
    "tulip_core;tulip_ogl;tulip_gui"
    ${LIBS_GROUP_NAME})

  DEFINE_COMPONENT(tulip3_compat_dev
    "Tulip 3 compatiblity layer (headers)"
    "Headers for the Tulip 3 compatibility layer."
    "tulip3_compat"
    ${HEADERS_GROUP_NAME})

  DEFINE_COMPONENT(tulip_python
    "TulipPython"
    "The TulipPython library provides Python bidings for TulipCore, TulipGUI and TulipGUI"
    "sip;tulip_core;tulip_ogl;tulip_gui"
    ${LIBS_GROUP_NAME})


  DEFINE_COMPONENT(tulip_app
    "Tulip"
    "A full GUI application including TulipCore, TulipOGL and TulipGUI features."
    "tulip_core;tulip_ogl;tulip_gui;tulip_python;tulip_core_plugins;tulip_ogl_plugins;tulip_gui_plugins;tulip_gui;spreadview;tulip_gui_pythonview;${W32_DBUS}"
    ${SOFTWARE_GROUP_NAME})

ENDIF(WIN32 OR APPLE)

IF(WIN32) # NSIS-specific behavior for installation types

  SET(CPACK_COMPONENT_BASE_SYSTEM_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_LIBXML2_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_PYTHON_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_ZLIB_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_FREETYPE_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_GLEW_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_JPEG_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_PNG_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QT_CORE_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QT_GUI_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QT_NETWORK_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QT_OPENGL_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QT_XML_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_OGDF_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QSCINTILLA2_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_FTGL_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_GZSTREAM_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_QUAZIP_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_SIP_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_TULIP_CORE_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_CORE_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_CORE_PLUGINS_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP_OGL_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_OGL_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_OGL_PLUGINS_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP_GUI_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_GUI_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_GUI_PLUGINS_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_TULIP_GUI_PYTHONVIEW_INSTALL_TYPES Full)
  SET(CPACK_COMPONENT_TULIP_GUI_SPREADVIEW_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP_OGDF_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_OGDF_DEV_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP_OGDF_PLUGINS_INSTALL_TYPES Full)

  SET(CPACK_COMPONENT_TULIP3_COMPAT_INSTALL_TYPES Full Framework)
  SET(CPACK_COMPONENT_TULIP3_COMPAT_DEV_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_TULIP_PYTHON_INSTALL_TYPES Full Framework)

  SET(CPACK_COMPONENT_TULIP_APP_INSTALL_TYPES Full)

ENDIF(WIN32)
