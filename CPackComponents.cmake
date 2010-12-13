SET(CPACK_COMPONENT_GROUP_LIBRARIES_DESCRIPTION "Tulip framework dynamic libraries.")
SET(CPACK_COMPONENT_GROUP_HEADERS_DESCRIPTION "Tulip framework C++ headers.")
SET(CPACK_COMPONENT_GROUP_SOFTWARE_DESCRIPTION "Tulip softwares.")
SET(CPACK_COMPONENT_GROUP_PLUGINS_DESCRIPTION "Tulip base plugins set.")
SET(CPACK_COMPONENT_GROUP_EXTRAS_DESCRIPTION "Tulip extra files and documentation.")

# library/tulip
DEFINE_COMPONENT(libtulip "Tulip framework" "Tulip core library provides a framework for huge graph manipulation." "" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_dev "Tulip headers" "Tulip core library provides a framework for huge graph manipulation." "libtulip" ${HEADERS_GROUP_NAME})

# library/tulip-ogl
DEFINE_COMPONENT(libtulip_ogl "Tulip - OpenGL bindings" "Tulip-ogl provides opengl bindings for 3D visualization of graphs created with the Tulip core library. " "libtulip" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogl_dev "Tulip OpenGL - Development files" "Tulip OpenGL provides opengl bindings for 3D visualization of graphs created with the Tulip core library." "libtulip_ogl" ${HEADERS_GROUP_NAME})

#library/tulip-qt
DEFINE_COMPONENT(libtulip_qt "Tulip - Qt bindings" "Tulip Qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip;libtulip_ogl" ${LIBS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_qt_dev "Tulip Qt - Development files" "Tulip-qt provides binding for the Qt4 framework. Helping user to design user interfaces to integrate Tulip 3D visualizations into a GUI." "libtulip_qt" ${HEADERS_GROUP_NAME})

# plugins/*
DEFINE_COMPONENT(libtulip_plugins "Tulip plugins" "Plugins for Tulip core" "libtulip" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_ogl_plugins "Tulip OpenGL plugins" "Plugins for Tulip OpenGL" "libtulip;libtulip_ogl" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(libtulip_qt_plugins "Tulip Qt plugins" "Plugins for Tulip Qt" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})
DEFINE_COMPONENT(tulip_plugins_spreadsheetview "Spreadsheet view" "Spreadsheet view plugin" "libtulip;libtulip_ogl;libtulip_qt" ${PLUGINS_GROUP_NAME})

# software/pluginsmanager
DEFINE_COMPONENT(tulip_pluginsmanager "Plugins manager" "A software to manage plugins installation from external sources." "libtulip;libtulip_ogl;libtulip_qt" ${SOFTWARE_GROUP_NAME})
DEFINE_COMPONENT(tulip_app "Tulip software" "The main Tulip software. Provides a complete interface and a set of tools to easily create, manage and visualize huge graphs in 3D scenes." "libtulip;libtulip_ogl;libtulip_qt;tulip_pluginsmanager" ${SOFTWARE_GROUP_NAME})
DEFINE_COMPONENT(tulip_textures "Additional textures" "Additional textures for the Tulip software" "" ${EXTRAS_GROUP_NAME})

# doc/*
DEFINE_COMPONENT(tulip_doc "Documentation" "Manuals and doxygen for the Tulip framework." "" ${EXTRAS_GROUP_NAME})

IF(LINUX)
  # meta package (Linux only)
  DEFINE_COMPONENT(tulip "Meta package" "Meta package containing tulip application, libraries, documentation and base plugins" "libtulip;libtulip_ogl;libtulip_qt;tulip_app;tulip_pluginsmanager;tulip_doc;tulip_textures;libtulip_plugins;libtulip_qt_plugins;libtulip_ogl_plugins;tulip_plugins_spreadsheetview" ${EXTRAS_GROUP_NAME})
ENDIF()
