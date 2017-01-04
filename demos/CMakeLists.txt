SET(TULIP_BUILD_DEMOS OFF CACHE BOOL "Build demo executables and libraries and install them in a demo folder")

IF(TULIP_BUILD_DEMOS)
ADD_SUBDIRECTORY(graph_display)
ADD_SUBDIRECTORY(clique_import)
ADD_SUBDIRECTORY(graph_manipulations)
ENDIF(TULIP_BUILD_DEMOS)
