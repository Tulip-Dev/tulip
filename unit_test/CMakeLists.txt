add_subdirectory(library)

IF(TULIP_PLUGIN_TESTS)
  add_subdirectory(plugins)
ENDIF()