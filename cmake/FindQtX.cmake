# If CMake does not automatically find Qt5 , the root directory
# of the Qt5 installation must be provided in the CMAKE_PREFIX_PATH variable.

# Unset related CMake variables in order to change the Qt5 version (by modifying
# the root Qt5 directory through the CMAKE_PREFIX_PATH variable) without having
# to delete the current CMake cache

UNSET(Qt5Core_FOUND CACHE)
UNSET(Qt5Gui_FOUND CACHE)
UNSET(Qt5Widgets_FOUND CACHE)
UNSET(Qt5Xml_FOUND CACHE)
UNSET(Qt5XmlPatterns_FOUND CACHE)
UNSET(Qt5Network_FOUND CACHE)
UNSET(Qt5WebKit_FOUND CACHE)
UNSET(Qt5WebKitWidgets_FOUND CACHE)
UNSET(Qt5Multimedia_FOUND CACHE)
UNSET(Qt5MultimediaWidgets_FOUND CACHE)
UNSET(Qt5Positioning_FOUND CACHE)
UNSET(Qt5PrintSupport_FOUND CACHE)
UNSET(Qt5Qml_FOUND CACHE)
UNSET(Qt5Quick_FOUND CACHE)
UNSET(Qt5Sensors_FOUND CACHE)
UNSET(Qt5DBus_FOUND CACHE)
UNSET(Qt5WebEngineWidgets_FOUND CACHE)
UNSET(Qt5WebChannel_FOUND CACHE)

UNSET(Qt5Core_DIR CACHE)
UNSET(Qt5Gui_DIR CACHE)
UNSET(Qt5Widgets_DIR CACHE)
UNSET(Qt5Xml_DIR CACHE)
UNSET(Qt5XmlPatterns_DIR CACHE)
UNSET(Qt5Network_DIR CACHE)
UNSET(Qt5WebKit_DIR CACHE)
UNSET(Qt5WebKitWidgets_DIR CACHE)
UNSET(Qt5Multimedia_DIR CACHE)
UNSET(Qt5MultimediaWidgets_DIR CACHE)
UNSET(Qt5Positioning_DIR CACHE)
UNSET(Qt5PrintSupport_DIR CACHE)
UNSET(Qt5Qml_DIR CACHE)
UNSET(Qt5Quick_DIR CACHE)
UNSET(Qt5Sensors_DIR CACHE)
UNSET(Qt5DBus_DIR CACHE)
UNSET(Qt5WebEngineWidgets_DIR CACHE)
UNSET(Qt5WebChannel_DIR CACHE)

SET(QT_HAS_WEBKIT FALSE)
SET(QT_HAS_WEBENGINE FALSE)

# Macro used to workaround a small issue with QtWebkit components on MSYS2:
# when compling in RelWithDebInfo mode, Qt debug libraries are selected instead
# of the release one (this should only happen when compiling in Debug mode)
MACRO(SETUP_QT_LIBRARIES QtModule LIBRARIES)
  IF(MINGW)
    GET_TARGET_PROPERTY(Qt5${QtModule}_INCLUDE_DIRS Qt5::${QtModule} INTERFACE_INCLUDE_DIRECTORIES)
    IF(CMAKE_DEBUG_MODE)
      GET_TARGET_PROPERTY(Qt5${QtModule}_LIBRARIES Qt5::${QtModule} LOCATION_DEBUG)
    ELSE(CMAKE_DEBUG_MODE)
      GET_TARGET_PROPERTY(Qt5${QtModule}_LIBRARIES Qt5::${QtModule} LOCATION_RELEASE)
    ENDIF(CMAKE_DEBUG_MODE)
  ENDIF(MINGW)
  SET(${LIBRARIES} ${${LIBRARIES}} ${Qt5${QtModule}_LIBRARIES} ${Qt5${QtModule}_LIBRARIES})
ENDMACRO(SETUP_QT_LIBRARIES)

FIND_PACKAGE(Qt5Widgets 5.6 REQUIRED)
FIND_PACKAGE(Qt5Network 5.6 REQUIRED)

SET(QT_FULL_VERSION ${Qt5Widgets_VERSION_STRING})
# Qt5Widgets_VERSION_STRING has been deprecated in favor of Qt5Widgets_VERSION since
# a few releases of Qt5 and seems to have been removed in some Linux distributions
# (experienced on KDE Neon Developer edition with Qt 5.9)
IF("${QT_FULL_VERSION}" STREQUAL "")
  SET(QT_FULL_VERSION ${Qt5Widgets_VERSION})
ENDIF("${QT_FULL_VERSION}" STREQUAL "")
STRING(REGEX MATCH "[0-9]\\.[0-9]+" QT_VERSION "${QT_FULL_VERSION}")

IF(NOT "${QT_VERSION}" STREQUAL "${LAST_FOUND_QT_VERSION}")
  MESSAGE(STATUS "Found Qt5 (version ${QT_FULL_VERSION})")
ENDIF(NOT "${QT_VERSION}" STREQUAL "${LAST_FOUND_QT_VERSION}")

SET(LAST_FOUND_QT_VERSION "${QT_VERSION}" CACHE INTERNAL "")

IF(WIN32)
  GET_TARGET_PROPERTY(QtCore_location Qt5::Core LOCATION)
  GET_FILENAME_COMPONENT(QT_BINARY_DIR ${QtCore_location} PATH)
  SET(QT_BINARY_DIR ${QT_BINARY_DIR}/../bin)
  # Standard Qt5 installation
  IF(EXISTS ${QT_BINARY_DIR}/../plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../plugins)
  # MSYS2 special case
  ELSEIF(EXISTS ${QT_BINARY_DIR}/../share/qt5/plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../share/qt5/plugins)
  ENDIF()
ELSEIF(APPLE)
  GET_TARGET_PROPERTY(QtCore_location Qt5::Core LOCATION)
  GET_FILENAME_COMPONENT(QT_FRAMEWORKS_DIR ${QtCore_location} PATH)
  SET(QT_FRAMEWORKS_DIR ${QT_FRAMEWORKS_DIR}/..)
  SET(QT_BINARY_DIR ${QT_FRAMEWORKS_DIR}/../bin)
ELSE()
  SET(QT_BINARY_DIR "${_qt5Gui_install_prefix}/bin")
  IF(NOT EXISTS ${QT_BINARY_DIR})
    IF(EXISTS /usr/lib/x86_64-linux-gnu/qt5/bin)
      SET(QT_BINARY_DIR /usr/lib/x86_64-linux-gnu/qt5/bin)
    ELSEIF(EXISTS /usr/lib/i386-linux-gnu/qt5/bin)
      SET(QT_BINARY_DIR /usr/lib/i386-linux-gnu/qt5/bin)
    ENDIF()
  ENDIF()
  IF(EXISTS ${QT_BINARY_DIR}/qmake)
    SET(QT_QMAKE_EXECUTABLE ${QT_BINARY_DIR}/qmake)
  ELSEIF(EXISTS ${QT_BINARY_DIR}/qmake-qt5)
    SET(QT_QMAKE_EXECUTABLE ${QT_BINARY_DIR}/qmake-qt5)
  ENDIF()
  # Standard Qt5 installation
  IF(EXISTS ${QT_BINARY_DIR}/../plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../plugins)
  ENDIF()
ENDIF()

SET(QT_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES})

GET_FILENAME_COMPONENT(QT_CMAKE_DIR "${Qt5Core_DIR}" DIRECTORY)

# On Apple platform, we need to link against Qt5DBus and Qt5PrintSupport
# when using the official Qt5 bundle provided by qt.io (dylibs dependencies side effect).
# However, those modules are not necessarily present when using Qt5 from
# Homebrew or MacPorts, so handle those special cases here.
IF(APPLE)
  SET(QT_DBUS_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5DBus")
  SET(QT_PRINTSUPPORT_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5PrintSupport")
  IF(EXISTS ${QT_DBUS_CMAKE_DIR} AND EXISTS ${QT_PRINTSUPPORT_CMAKE_DIR})
    FIND_PACKAGE(Qt5DBus 5.5)
    FIND_PACKAGE(Qt5PrintSupport 5.5)
    IF(${Qt5DBus_FOUND} AND ${Qt5PrintSupport_FOUND})
      SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Qt5DBus_LIBRARIES})
    ENDIF(${Qt5DBus_FOUND} AND ${Qt5PrintSupport_FOUND})
  ENDIF(EXISTS ${QT_DBUS_CMAKE_DIR} AND EXISTS ${QT_PRINTSUPPORT_CMAKE_DIR})
ENDIF(APPLE)

# Check if the Qt5 installation is bundled with WebKit (deprecated since Qt 5.5)
# and setup its use if it is the case.
SET(QT_WEBKIT_WIDGETS_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5WebKitWidgets")
IF(EXISTS ${QT_WEBKIT_WIDGETS_CMAKE_DIR})
  FIND_PACKAGE(Qt5WebKit)
  FIND_PACKAGE(Qt5WebKitWidgets)
  IF(${Qt5WebKit_FOUND} AND ${Qt5WebKitWidgets_FOUND})
    SET(QT_HAS_WEBKIT TRUE)
    SET(QT_WEB_COMPONENT "QtWebKit")
    SETUP_QT_LIBRARIES(WebKit QT_WEB_LIBRARIES)
    SETUP_QT_LIBRARIES(WebKitWidgets QT_WEB_LIBRARIES)
  ENDIF(${Qt5WebKit_FOUND} AND ${Qt5WebKitWidgets_FOUND})
ENDIF(EXISTS ${QT_WEBKIT_WIDGETS_CMAKE_DIR})

# If Qt5 is not bundled with WebKit then check if its installation
# provides WebEngine (new web module since Qt 5.4) and setup its use.
SET(QT_WEBENGINE_WIDGETS_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5WebEngineWidgets")
IF(NOT QT_HAS_WEBKIT AND EXISTS ${QT_WEBENGINE_WIDGETS_CMAKE_DIR})
  FIND_PACKAGE(Qt5WebEngineWidgets 5.5)
  FIND_PACKAGE(Qt5WebChannel 5.5)
  IF(${Qt5WebEngineWidgets_FOUND} AND ${Qt5WebChannel_FOUND})
    SET(QT_HAS_WEBENGINE TRUE)
    SET(QT_WEB_COMPONENT "QtWebEngine")
    SET(QT_WEB_LIBRARIES ${Qt5WebEngineWidgets_LIBRARIES} ${Qt5WebChannel_LIBRARIES})
  ENDIF(${Qt5WebEngineWidgets_FOUND} AND ${Qt5WebChannel_FOUND})
ENDIF(NOT QT_HAS_WEBKIT AND EXISTS ${QT_WEBENGINE_WIDGETS_CMAKE_DIR})

IF(NOT "${QT_WEB_COMPONENT}" STREQUAL "${LAST_FOUND_QT_WEB_COMPONENT}")
  MESSAGE(STATUS "Found ${QT_WEB_COMPONENT}")
ENDIF(NOT "${QT_WEB_COMPONENT}" STREQUAL "${LAST_FOUND_QT_WEB_COMPONENT}")

SET(LAST_FOUND_QT_WEB_COMPONENT "${QT_WEB_COMPONENT}" CACHE INTERNAL "")

MACRO(QTX_SET_INCLUDES_AND_DEFINITIONS)
  INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Qt5Network_INCLUDE_DIRS})
  ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
  ADD_DEFINITIONS(${Qt5Network_DEFINITIONS})
ENDMACRO()

MACRO(QTXWEB_SET_INCLUDES_AND_DEFINITIONS)
  IF(QT_HAS_WEBKIT)
    INCLUDE_DIRECTORIES(${Qt5WebKit_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt5WebKitWidgets_INCLUDE_DIRS})
    ADD_DEFINITIONS(${Qt5WebKit_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5WebKitWidgets_DEFINITIONS})
    ADD_DEFINITIONS(-DQT_HAS_WEBKIT)
  ENDIF()
  IF(QT_HAS_WEBENGINE)
    INCLUDE_DIRECTORIES(${Qt5WebEngineWidgets_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt5WebChannel_INCLUDE_DIRS})
    ADD_DEFINITIONS(${Qt5WebEngineWidgets_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5WebChannel_DEFINITIONS})
    ADD_DEFINITIONS(-DQT_HAS_WEBENGINE)
  ENDIF()
ENDMACRO(QTXWEB_SET_INCLUDES_AND_DEFINITIONS)

# Define aliases for Qt macros in order to build the project
MACRO(QTX_WRAP_CPP outfiles)
  QT5_WRAP_CPP(${outfiles} ${ARGN})
ENDMACRO()

MACRO(QTX_WRAP_UI outfiles)
  QT5_WRAP_UI(${outfiles} ${ARGN})
ENDMACRO()

MACRO(QTX_ADD_RESOURCES outfiles)
  QT5_ADD_RESOURCES(${outfiles} ${ARGN})
ENDMACRO()

# With MinGW, remove the -fPIC compiler option as it is not needed ansd
# generates a lot of warnings
IF(MINGW)
  STRING(REPLACE "-fPIC" "" Qt5Widgets_EXECUTABLE_COMPILE_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
ENDIF(MINGW)
