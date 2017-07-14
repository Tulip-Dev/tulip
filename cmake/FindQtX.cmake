SET(TULIP_USE_QT5 OFF CACHE BOOL "Use Qt5 to build the project.")

SET(Qt5Widgets_FOUND false)
SET(Qt5OpenGL_FOUND false)
SET(Qt5Xml_FOUND false)
SET(Qt5XmlPatterns_FOUND false)
SET(Qt5Network_FOUND false)

SET(QT_HAS_WEBKIT FALSE)
SET(QT_HAS_WEBENGINE FALSE)

# If requested, use Qt5 to build the project.
# If CMake does not automatically find Qt5 , the root directory
# of the Qt5 installation must be provided in the CMAKE_PREFIX_PATH variable.

# Also, when building Tulip Python wheels, force the use of Qt4 (more lightweight, easier to deploy)
IF(TULIP_USE_QT5 AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

  # Unset related CMake variables in order to change the Qt5 version (by modifying the root Qt5 directory through the CMAKE_PREFIX_PATH variable)
  # without having to delete the current CMake cache
  UNSET(Qt5Core_FOUND CACHE)
  UNSET(Qt5Gui_FOUND CACHE)
  UNSET(Qt5Widgets_FOUND CACHE)
  UNSET(Qt5OpenGL_FOUND CACHE)
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
  UNSET(Qt5OpenGL_DIR CACHE)
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

  FIND_PACKAGE(Qt5Widgets)
  FIND_PACKAGE(Qt5OpenGL)
  FIND_PACKAGE(Qt5Network)

ENDIF(TULIP_USE_QT5 AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

 # Qt5 and all the required modules are present, do global setup
IF(${Qt5Widgets_FOUND} AND ${Qt5OpenGL_FOUND} AND ${Qt5Network_FOUND})

  SET(USE_QT5 true)
  STRING(REGEX MATCH "[0-9]\\.[0-9]" QT_VERSION "${Qt5Widgets_VERSION_STRING}")
  # Qt5Widgets_VERSION_STRING has been deprecated in favor of Qt5Widgets_VERSION since a few releases of Qt5
  # and seems to have been removed in some Linux distributions (experienced on Kde Neon Developer edition that now uses Qt 5.9)
  IF(NOT QT_VERSION MATCHES "[0-9]\\.[0-9]")
    STRING(REGEX MATCH "[0-9]\\.[0-9]" QT_VERSION "${Qt5Widgets_VERSION}")
  ENDIF(NOT QT_VERSION MATCHES "[0-9]\\.[0-9]")

  # Qt 5.7 requires C++11 support
  IF(QT_VERSION VERSION_GREATER 5.7 OR QT_VERSION VERSION_EQUAL 5.7)
    IF(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    ELSE(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
      IF((CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1) OR CLANG)

          SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
          IF(CLANG AND APPLE)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
          ENDIF(CLANG AND APPLE)
      ENDIF((CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.1) OR CLANG)
    ENDIF(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
  ENDIF(QT_VERSION VERSION_GREATER 5.7 OR QT_VERSION VERSION_EQUAL 5.7)

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
  ENDIF()

  INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Qt5OpenGL_INCLUDE_DIRS})
  INCLUDE_DIRECTORIES(${Qt5Network_INCLUDE_DIRS})

  ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
  ADD_DEFINITIONS(${Qt5OpenGL_DEFINITIONS})
  ADD_DEFINITIONS(${Qt5Network_DEFINITIONS})

  SET(QT_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Network_LIBRARIES})

  GET_FILENAME_COMPONENT(QT_CMAKE_DIR "${Qt5Core_DIR}" DIRECTORY)

  # On Apple platform, we need to link against Qt5DBus and Qt5PrintSupport
  # when using the official Qt5 bundle provided by qt.io (dylibs dependencies side effect).
  # However, those modules are not necessarily present when using Qt5 from Homebrew or MacPorts,
  # so handle those special cases here.
  IF(APPLE)
    SET(QT_DBUS_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5DBus")
    SET(QT_PRINTSUPPORT_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5PrintSupport")
    IF(EXISTS ${QT_DBUS_CMAKE_DIR} AND EXISTS ${QT_PRINTSUPPORT_CMAKE_DIR})
      FIND_PACKAGE(Qt5DBus)
      FIND_PACKAGE(Qt5PrintSupport)
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
      IF(NOT Qt5WebKit_INCLUDE_DIRS)
        GET_TARGET_PROPERTY(Qt5WebKit_INCLUDE_DIRS Qt5::WebKit INTERFACE_INCLUDE_DIRECTORIES)
        GET_TARGET_PROPERTY(Qt5WebKitWidgets_INCLUDE_DIRS Qt5::WebKitWidgets INTERFACE_INCLUDE_DIRECTORIES)
        GET_TARGET_PROPERTY(Qt5WebKit_LIBRARIES Qt5::WebKit LOCATION)
        GET_TARGET_PROPERTY(Qt5WebKitWidgets_LIBRARIES Qt5::WebKitWidgets LOCATION)
      ENDIF(NOT Qt5WebKit_INCLUDE_DIRS)
      INCLUDE_DIRECTORIES(${Qt5WebKit_INCLUDE_DIRS})
      INCLUDE_DIRECTORIES(${Qt5WebKitWidgets_INCLUDE_DIRS})
      ADD_DEFINITIONS(${Qt5WebKit_DEFINITIONS})
      ADD_DEFINITIONS(${Qt5WebKitWidgets_DEFINITIONS})
      SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5WebKit_LIBRARIES} ${Qt5WebKitWidgets_LIBRARIES})
    ENDIF(${Qt5WebKit_FOUND} AND ${Qt5WebKitWidgets_FOUND})
  ENDIF(EXISTS ${QT_WEBKIT_WIDGETS_CMAKE_DIR})

  # If Qt5 is not bundled with WebKit then check if its installation
  # provides WebEngine (new web module since Qt 5.4)
  # and setup its use if it is the case.
  SET(QT_WEBENGINE_WIDGETS_CMAKE_DIR "${QT_CMAKE_DIR}/Qt5WebEngineWidgets")
  IF(NOT QT_HAS_WEBKIT AND EXISTS ${QT_WEBENGINE_WIDGETS_CMAKE_DIR})
    FIND_PACKAGE(Qt5WebEngineWidgets)
    FIND_PACKAGE(Qt5WebChannel)
    IF(${Qt5WebEngineWidgets_FOUND} AND ${Qt5WebChannel_FOUND})
      SET(QT_HAS_WEBENGINE TRUE)
      INCLUDE_DIRECTORIES(${Qt5WebEngineWidgets_INCLUDE_DIRS})
      INCLUDE_DIRECTORIES(${Qt5WebChannel_INCLUDE_DIRS})
      ADD_DEFINITIONS(${Qt5WebEngineWidgets_DEFINITIONS})
      ADD_DEFINITIONS(${Qt5WebChannel_DEFINITIONS})
      SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5WebEngineWidgets_LIBRARIES} ${Qt5WebChannel_LIBRARIES})
    ENDIF(${Qt5WebEngineWidgets_FOUND} AND ${Qt5WebChannel_FOUND})
  ENDIF(NOT QT_HAS_WEBKIT AND EXISTS ${QT_WEBENGINE_WIDGETS_CMAKE_DIR})

   # define aliases for Qt macros in order to build the project
  MACRO(QTX_WRAP_CPP outfiles)
    QT5_WRAP_CPP(${outfiles} ${ARGN})
  ENDMACRO()

  MACRO(QTX_WRAP_UI outfiles)
    QT5_WRAP_UI(${outfiles} ${ARGN})
  ENDMACRO()

  MACRO(QTX_ADD_RESOURCES outfiles)
    QT5_ADD_RESOURCES(${outfiles} ${ARGN})
  ENDMACRO()

  # with MinGW, remove the -fPIC compiler option as it is not needed and generates a lot of warnings
  IF(MINGW)
    STRING(REPLACE "-fPIC" "" Qt5Widgets_EXECUTABLE_COMPILE_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
  ENDIF(MINGW)

# Use Qt4 otherwise
ELSE(${Qt5Widgets_FOUND} AND ${Qt5OpenGL_FOUND} AND ${Qt5Network_FOUND})

  IF(TULIP_USE_QT5 AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)
    MESSAGE("Qt 5 required components or the CMake modules to locate them have not been found.")
    MESSAGE("Falling back to Qt 4.")
  ENDIF(TULIP_USE_QT5 AND NOT TULIP_ACTIVATE_PYTHON_WHEELS_TARGETS)

  FIND_PACKAGE(Qt4 4.6.0 REQUIRED)
  SET(USE_QT4 true)
  SET(QT_USE_QTOPENGL true)
  SET(QT_USE_QTXML false)
  SET(QT_USE_QTXMLPATTERNS false)
  SET(QT_USE_QTNETWORK true)
  SET(QT_USE_QTWEBKIT true)
  SET(QT_USE_QTTEST false)
  SET(QT_USE_QTDBUS false)
  INCLUDE(${QT_USE_FILE})

  IF(QT_QTWEBKIT_LIBRARY)
    SET(QT_HAS_WEBKIT TRUE)
  ENDIF(QT_QTWEBKIT_LIBRARY)

  SET(QT_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

   # define aliases for Qt macros
  MACRO(QTX_WRAP_CPP outfiles)
    QT4_WRAP_CPP(${outfiles} ${ARGN})
  ENDMACRO()

  MACRO(QTX_WRAP_UI outfiles)
    QT4_WRAP_UI(${outfiles} ${ARGN})
  ENDMACRO()

  MACRO(QTX_ADD_RESOURCES outfiles)
    QT4_ADD_RESOURCES(${outfiles} ${ARGN})
  ENDMACRO()

ENDIF(${Qt5Widgets_FOUND} AND ${Qt5OpenGL_FOUND} AND ${Qt5Network_FOUND})
