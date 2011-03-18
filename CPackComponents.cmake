SET(CPACK_COMPONENT_GROUP_THIRDPARTY_DESCRIPTION "Thirdparty libraries.")
SET(CPACK_COMPONENT_GROUP_LIBRARIES_DESCRIPTION "Tulip framework dynamic libraries.")
SET(CPACK_COMPONENT_GROUP_HEADERS_DESCRIPTION "Tulip framework C++ headers.")
SET(CPACK_COMPONENT_GROUP_SOFTWARE_DESCRIPTION "Tulip softwares.")
SET(CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION "Tulip base plugins set.")
SET(CPACK_COMPONENT_GROUP_EXTRAS_DESCRIPTION "Tulip extra files and documentation.")

#thirdparty
DEFINE_COMPONENT(ftgl "FTGL" "A library to render freetype fonts in openGL scenes." "" ${THIRDPARTY_GROUP_NAME})
DEFINE_COMPONENT(qscintilla2 "QScintilla 2" "A library to provide source code edition facility in Qt." "" ${THIRDPARTY_GROUP_NAME})
DEFINE_COMPONENT(gzstream "Gzstream" "A library to provide zlib facilities in a C++ iostream." "" ${THIRDPARTY_GROUP_NAME})
DEFINE_COMPONENT(siplib "SIP library" "A library providing python bindings for C++." "libtulip;libtulip_ogl;libtulip_qt" ${THIRDPARTY_GROUP_NAME})

# library/tulip
DEFINE_COMPONENT(libtulip "Core library" "Tulip core library provides a framework for huge graph manipulation." "gzstream" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_dev "Core library - Development files" "Tulip core library provides a framework for huge graph manipulation." "libtulip" ${HEADERS_GROUP_NAME})

# library/tulip-ogl
DEFINE_COMPONENT(libtulip_ogl "OpenGL bindings" "Tulip-ogl provides opengl bindings for 3D visualization of graphs created with the Tulip core library. " "libtulip;ftgl" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogl_dev "OpenGL bindings - Development files" "Tulip OpenGL provides opengl bindings for 3D visualization of graphs created with the Tulip core library." "libtulip_ogl" ${HEADERS_GROUP_NAME})

#library/tulip-qt
DEFINE_COMPONENT(libtulip_qt "Qt bindings" "Tulip Qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip;libtulip_ogl" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_qt_dev "Qt bindings - Development files" "Tulip-qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip_qt" ${HEADERS_GROUP_NAME})

#library/tulip-ogdf
DEFINE_COMPONENT(libtulip_ogdf "OGDF library bridge" "Tulip OGDF provides a bridge to use the powerful OGDF library inside of Tulip." "" ${LIBS_GROUP_NAME})

# plugins/*
DEFINE_COMPONENT(libtulip_plugins "Core plugins" "Plugins for Tulip core" "libtulip" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogl_plugins "OpenGL plugins" "Plugins for Tulip OpenGL" "libtulip;libtulip_ogl" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_qt_plugins "Qt plugins" "Plugins for Tulip Qt" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(tulip_plugins_spreadsheetview "Spreadsheet view" "Spreadsheet view plugin" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(tulip_plugins_pythonscriptview "Python view" "Python view plugin" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogdf_plugins "OGDF plugins" "Plugins using the Tulip OGDF bridge" "libtulip" ${PLUGINS_GROUP_NAME})

# software/pluginsmanager
DEFINE_COMPONENT(tulip_pluginsmanager "Plugins manager" "A software to manage plugins installation from external sources." "libtulip;libtulip_ogl;libtulip_qt" ${SOFTWARE_GROUP_NAME})
DEFINE_COMPONENT(tulip_app "Tulip software" "The main Tulip software. Provides a complete interface and a set of tools to easily create, manage and visualize huge graphs in 3D scenes." "libtulip;libtulip_ogl;libtulip_qt;tulip_pluginsmanager" ${SOFTWARE_GROUP_NAME})
DEFINE_COMPONENT(tulip_textures "Additional textures" "Additional textures for the Tulip software" "" ${EXTRAS_GROUP_NAME})

# doc/*
IF(GENERATE_DOC)
  DEFINE_COMPONENT(tulip_doc "Framework documentation" "Manuals and doxygen for the Tulip framework." "libtulip" ${EXTRAS_GROUP_NAME})
ENDIF()

IF(LINUX)
  SET(META_DEPS "ftgl;qscintilla2;gzstream;siplib;libtulip;libtulip_ogl;libtulip_qt;tulip_app;tulip_pluginsmanager;tulip_textures;libtulip_plugins;libtulip_qt_plugins;libtulip_ogl_plugins;tulip_plugins_spreadsheetview")
  # meta package (Linux only)
  IF(GENERATE_DOC)
    SET(META_DEPS "${META_DEPS};tulip_doc")
  ENDIF()
  DEFINE_COMPONENT(tulip "Meta package" "Meta package containing tulip application, libraries, documentation and base plugins" "${META_DEPS}" ${EXTRAS_GROUP_NAME})
ENDIF()
