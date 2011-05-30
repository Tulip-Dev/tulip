SET(DBUS_DISABLE_ASSERTS ON)
SET(DBUS_ENABLE_VERBOSE_MODE OFF)
SET(DBUS_BUILD_TESTS OFF)

SET(CMAKE_DBUS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dbus-1.4.8/cmake)
SET(CMAKE_DBUS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/dbus-1.4.8/cmake)
ADD_SUBDIRECTORY(dbus-1.4.8/cmake)

# Installation of DBus configuration files
INSTALL(FILES
  etc/dbus-1/session.conf
  etc/dbus-1/system.conf
DESTINATION etc/dbus-1/
COMPONENT dbus)
