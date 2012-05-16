# Installation of DBus configuration files
IF(WIN32)
  ADD_SUBDIRECTORY(libdbus/cmake)	
ELSEIF(APPLE)
  SET(ENV_FLAGS "CFLAGS=-arch ${CMAKE_OSX_ARCHITECTURES}" "LDFLAGS=-arch ${CMAKE_OSX_ARCHITECTURES}")
  ADD_CUSTOM_TARGET(libdbus ALL
    COMMAND env ${ENV_FLAGS} ./configure --mandir=${CMAKE_CURRENT_BINARY_DIR}/dbus/man --libexecdir=${CMAKE_CURRENT_BINARY_DIR}/dbus/sbin --disable-static --with-dbus-user=messagebus --with-dbus-daemondir=${CMAKE_CURRENT_BINARY_DIR}/dbus/bin --enable-console-owner-file --disable-asserts --disable-tests --enable-checks --enable-kqueue --disable-launchd --disable-userdb-cache --disable-ansi --with-xml=expat --disable-xml-docs --disable-dependency-tracking --without-x ac_cv_func_poll=no --prefix=${CMAKE_CURRENT_BINARY_DIR}/dbus
    COMMAND env ${ENV_FLAGS} make clean
    COMMAND env ${ENV_FLAGS} make -j2
    COMMAND env ${ENV_FLAGS} make -j2 install
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libdbus)

  INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/lib/libdbus-1.dylib
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/lib/libdbus-1.3.dylib
  DESTINATION ${TulipLibInstallDir} COMPONENT dbus)
  INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/bin/dbus-daemon
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/bin/dbus-launch
  PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE WORLD_READ WORLD_EXECUTE
  DESTINATION ${TulipBinInstallDir} COMPONENT dbus)
  INSTALL(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/etc/dbus-1/session.conf
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/etc/dbus-1/system.conf
  DESTINATION ${TulipBinInstallDir}/../etc/dbus-1/ COMPONENT dbus)
  INSTALL(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/dbus/etc/dbus-1/session.d
  DESTINATION ${TulipBinInstallDir}/../etc/dbus-1/ COMPONENT dbus)

ENDIF()
