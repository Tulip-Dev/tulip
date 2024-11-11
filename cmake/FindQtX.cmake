OPTION(TULIP_USE_QT6 "Use Qt6 instead of Qt5" OFF)
IF(TULIP_USE_QT6)
  SET(QT Qt6)
  SET(QT_MIN 6.5)
  UNSET(Qt6OpenGLWidgets_DIR CACHE)
ELSE()
  SET(QT Qt5)
  SET(QT_MIN 5.15)
ENDIF(TULIP_USE_QT6)

STRING(TOLOWER ${QT} QTDIR)

# If CMake does not automatically find Qt5 or Qt6 , the root directory
# of the Qt installation must be provided in the CMAKE_PREFIX_PATH variable.

# Unset related CMake variables in order to change the Qt version (by modifying
# the root Qt directory through the CMAKE_PREFIX_PATH variable) without having
# to delete the current CMake cache

UNSET(${QT}Core_FOUND CACHE)
UNSET(${QT}Gui_FOUND CACHE)
UNSET(${QT}Widgets_FOUND CACHE)
UNSET(${QT}Network_FOUND CACHE)
UNSET(${QT}PrintSupport_FOUND CACHE)
UNSET(${QT}DBus_FOUND CACHE)
UNSET(${QT}Core_DIR CACHE)
UNSET(${QT}Gui_DIR CACHE)
UNSET(${QT}Widgets_DIR CACHE)
UNSET(${QT}Network_DIR CACHE)
UNSET(${QT}PrintSupport_DIR CACHE)
UNSET(${QT}DBus_DIR CACHE)

FIND_PACKAGE(${QT}Widgets ${QT_MIN} REQUIRED)
FIND_PACKAGE(${QT}Network ${QT_MIN} REQUIRED)
IF(${QT} STREQUAL Qt6)
  FIND_PACKAGE(Qt6Core5Compat ${QT_MIN} REQUIRED)
  FIND_PACKAGE(Qt6OpenGLWidgets ${QT_MIN} REQUIRED)
  SET(QT6_LIBRARIES ${Qt6Core5Compat_LIBRARIES} ${Qt6OpenGLWidgets_LIBRARIES})
ENDIF()

SET(QT_FULL_VERSION ${${QT}Widgets_VERSION_STRING})
# QtWidgets_VERSION_STRING has been deprecated in favor of Qt5Widgets_VERSION since
# a few releases of Qt5 and seems to have been removed in some Linux distributions
# (experienced on KDE Neon Developer edition with Qt 5.9)
IF("${QT_FULL_VERSION}" STREQUAL "")
  SET(QT_FULL_VERSION ${${QT}Widgets_VERSION})
ENDIF("${QT_FULL_VERSION}" STREQUAL "")
STRING(REGEX MATCH "[0-9]\\.[0-9]+" QT_VERSION "${QT_FULL_VERSION}")

IF(NOT "${QT_VERSION}" STREQUAL "${LAST_FOUND_QT_VERSION}")
  MESSAGE(STATUS "Found ${QT} (version ${QT_FULL_VERSION})")
ENDIF(NOT "${QT_VERSION}" STREQUAL "${LAST_FOUND_QT_VERSION}")

SET(LAST_FOUND_QT_VERSION "${QT_VERSION}" CACHE INTERNAL "")

IF(WIN32)
  GET_TARGET_PROPERTY(QtCore_location ${QT}::Core LOCATION)
  GET_FILENAME_COMPONENT(QT_BINARY_DIR ${QtCore_location} PATH)
  SET(QT_BINARY_DIR ${QT_BINARY_DIR}/../bin)
  # Standard Qt installation
  IF(EXISTS ${QT_BINARY_DIR}/../plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../plugins)
  # MSYS2 special case
  ELSEIF(EXISTS ${QT_BINARY_DIR}/../share/${QTDIR}/plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../share/${QTDIR}/plugins)
  ENDIF()
ELSEIF(APPLE)
  GET_TARGET_PROPERTY(QtCore_location ${QT}::Core LOCATION)
  GET_FILENAME_COMPONENT(QT_FRAMEWORKS_DIR ${QtCore_location} PATH)
  SET(QT_FRAMEWORKS_DIR ${QT_FRAMEWORKS_DIR}/..)
  SET(QT_BINARY_DIR ${QT_FRAMEWORKS_DIR}/../bin)
ELSE()
  SET(QT_BINARY_DIR "${_qt5Gui_install_prefix}/bin")
  IF(NOT EXISTS ${QT_BINARY_DIR})
    IF(EXISTS /usr/lib/x86_64-linux-gnu/${QTDIR}/bin)
      SET(QT_BINARY_DIR /usr/lib/x86_64-linux-gnu/${QTDIR}/bin)
    ENDIF()
  ENDIF()
  IF(EXISTS ${QT_BINARY_DIR}/qmake)
    SET(QT_QMAKE_EXECUTABLE ${QT_BINARY_DIR}/qmake)
  ELSEIF(EXISTS ${QT_BINARY_DIR}/qmake-${QTDIR})
    SET(QT_QMAKE_EXECUTABLE ${QT_BINARY_DIR}/qmake-${QTDIR})
  ENDIF()
  # Standard Qt5 installation
  IF(EXISTS ${QT_BINARY_DIR}/../plugins)
    SET(QT_PLUGINS_DIR ${QT_BINARY_DIR}/../plugins)
  ENDIF()
ENDIF()

SET(QT_LIBRARIES ${${QT}Widgets_LIBRARIES} ${${QT}Network_LIBRARIES} ${QT6_LIBRARIES})

GET_FILENAME_COMPONENT(QT_CMAKE_DIR "${${QT}Core_DIR}" DIRECTORY)

# On Apple platform, we need to link against Qt5DBus and Qt5PrintSupport
# when using the official Qt5 bundle provided by qt.io (dylibs dependencies side effect).
# However, those modules are not necessarily present when using Qt5 from
# Homebrew or MacPorts, so handle those special cases here.
IF(APPLE)
  SET(QT_DBUS_CMAKE_DIR "${QT_CMAKE_DIR}/${QT}DBus")
  SET(QT_PRINTSUPPORT_CMAKE_DIR "${QT_CMAKE_DIR}/${QT}PrintSupport")
  IF(EXISTS ${QT_DBUS_CMAKE_DIR} AND EXISTS ${QT_PRINTSUPPORT_CMAKE_DIR})
    FIND_PACKAGE(${QT}DBus ${QT_MIN})
    FIND_PACKAGE(${QT}PrintSupport ${QT_MIN})
    IF(${${QT}DBus_FOUND} AND ${${QT}PrintSupport_FOUND})
      SET(QT_LIBRARIES ${QT_LIBRARIES} ${${QT}PrintSupport_LIBRARIES} ${${QT}DBus_LIBRARIES})
    ENDIF()
  ENDIF()
ENDIF(APPLE)

MACRO(QTX_SET_INCLUDES_AND_DEFINITIONS)
  INCLUDE_DIRECTORIES(${${QT}Widgets_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${${QT}Network_INCLUDE_DIRS})
  ADD_COMPILE_DEFINITIONS(${${QT}Widgets_COMPILE_DEFINITIONS})
  ADD_COMPILE_DEFINITIONS(${${QT}Network_COMPILE_DEFINITIONS})
  IF(${QT} STREQUAL Qt6)
    INCLUDE_DIRECTORIES(${Qt6Core5Compat_INCLUDE_DIRS})
    ADD_COMPILE_DEFINITIONS(${Qt6Core5Compat_COMPILE_DEFINITIONS})
    INCLUDE_DIRECTORIES(${Qt6OpenGLWidgets_INCLUDE_DIRS})
    ADD_COMPILE_DEFINITIONS(${Qt6OpenGLWidgets_COMPILE_DEFINITIONS})
  ENDIF()
ENDMACRO()

# Define aliases for Qt macros in order to build the project
IF(${QT} STREQUAL Qt5)
  MACRO(QTX_WRAP_CPP outfiles)
    QT5_WRAP_CPP(${outfiles} ${ARGN})
  ENDMACRO()
ELSE()
  MACRO(QTX_WRAP_CPP outfiles)
    QT_WRAP_CPP(${outfiles} ${ARGN})
  ENDMACRO()
ENDIF()

IF(${QT} STREQUAL Qt5)
  MACRO(QTX_WRAP_UI outfiles)
    QT5_WRAP_UI(${outfiles} ${ARGN})
  ENDMACRO()
ELSE()
  MACRO(QTX_WRAP_UI outfiles)
    QT_WRAP_UI(${outfiles} ${ARGN})
  ENDMACRO()
ENDIF()

IF(${QT} STREQUAL Qt5)
MACRO(QTX_ADD_RESOURCES outfiles)
  QT5_ADD_RESOURCES(${outfiles} ${ARGN})
ENDMACRO()
ELSE()
  MACRO(QTX_ADD_RESOURCES outfiles)
    QT_ADD_RESOURCES(${outfiles} ${ARGN})
  ENDMACRO()
ENDIF()
