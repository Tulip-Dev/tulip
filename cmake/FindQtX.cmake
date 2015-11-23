SET(USE_QT5_IF_INSTALLED OFF CACHE BOOL "Use Qt5 to build the project if it is installed.")

SET(Qt5Widgets_FOUND false)
SET(Qt5OpenGL_FOUND false)
SET(Qt5Xml_FOUND false)
SET(Qt5XmlPatterns_FOUND false)
SET(Qt5Network_FOUND false)
SET(Qt5WebKit_FOUND false)
SET(Qt5WebKitWidgets_FOUND false)

# If requested, use Qt5 to build the project
# In order for CMake to find and configure a Qt5 build, the root directory
# of the Qt5 installation must be provided in the CMAKE_PREFIX_PATH variable

# Also, when building Tulip Python wheels, force the use of Qt4 (more lightweight, easier to deploy)
IF(USE_QT5_IF_INSTALLED AND NOT ACTIVATE_PYTHON_WHEELS_TARGETS)

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

    FIND_PACKAGE(Qt5Widgets)
    FIND_PACKAGE(Qt5OpenGL)
    FIND_PACKAGE(Qt5Xml)
    FIND_PACKAGE(Qt5XmlPatterns)
    FIND_PACKAGE(Qt5Network)
    FIND_PACKAGE(Qt5WebKit)
    FIND_PACKAGE(Qt5WebKitWidgets)
    IF(APPLE)
        FIND_PACKAGE(Qt5DBus)
        FIND_PACKAGE(Qt5PrintSupport)
    ENDIF(APPLE)

ENDIF(USE_QT5_IF_INSTALLED AND NOT ACTIVATE_PYTHON_WHEELS_TARGETS)

 # Qt5 and all the required modules are present, do global setup
IF(${Qt5Widgets_FOUND} AND ${Qt5OpenGL_FOUND} AND
   ${Qt5Xml_FOUND} AND ${Qt5XmlPatterns_FOUND} AND ${Qt5Network_FOUND} AND
   ${Qt5WebKit_FOUND} AND ${Qt5WebKitWidgets_FOUND})

    SET(USE_QT5 true)
    SET(QT_VERSION ${Qt5Widgets_VERSION_STRING})
    STRING(REGEX MATCH "[0-9]\\.[0-9]" QT_VERSION "${QT_VERSION}")

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
    INCLUDE_DIRECTORIES(${Qt5Xml_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt5XmlPatterns_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt5Network_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt5WebKit_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${Qt5WebKitWidgets_INCLUDE_DIRS})

    ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5OpenGL_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5Xml_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5XmlPatterns_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5Network_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5WebKit_DEFINITIONS})
    ADD_DEFINITIONS(${Qt5WebKitWidgets_DEFINITIONS})

    SET(QT_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Xml_LIBRARIES} ${Qt5XmlPatterns_LIBRARIES}
                     ${Qt5Network_LIBRARIES} ${Qt5WebKit_LIBRARIES} ${Qt5WebKitWidgets_LIBRARIES})
    IF(APPLE)
        SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Qt5DBus_LIBRARIES})
    ENDIF(APPLE)

     # define aliases for Qt macros in order to build the project
    MACRO(QTX_WRAP_CPP outfiles )
      QT5_WRAP_CPP(${outfiles} ${ARGN})
    ENDMACRO()

    MACRO(QTX_WRAP_UI outfiles )
      QT5_WRAP_UI(${outfiles} ${ARGN})
    ENDMACRO()

    MACRO(QTX_ADD_RESOURCES outfiles )
      QT5_ADD_RESOURCES(${outfiles} ${ARGN})
    ENDMACRO()

# Use Qt4 otherwise
ELSE(${Qt5Widgets_FOUND} AND ${Qt5OpenGL_FOUND} AND
     ${Qt5Xml_FOUND} AND ${Qt5XmlPatterns_FOUND} AND ${Qt5Network_FOUND} AND
     ${Qt5WebKit_FOUND} AND ${Qt5WebKitWidgets_FOUND})

    IF(USE_QT5_IF_INSTALLED AND NOT ACTIVATE_PYTHON_WHEELS_TARGETS)
      MESSAGE("Qt 5 required components or the CMake modules to locate them have not been found.")
      MESSAGE("Falling back to Qt 4.")
    ENDIF(USE_QT5_IF_INSTALLED AND NOT ACTIVATE_PYTHON_WHEELS_TARGETS)

    FIND_PACKAGE(Qt4 4.7.0 REQUIRED)
    SET(USE_QT4 true)
    SET(QT_USE_QTOPENGL true)
    SET(QT_USE_QTXML true)
    SET(QT_USE_QTXMLPATTERNS true)
    SET(QT_USE_QTNETWORK true)
    SET(QT_USE_QTWEBKIT true)
    SET(QT_USE_QTTEST false)
    SET(QT_USE_QTDBUS false)
    INCLUDE(${QT_USE_FILE})

    SET(QT_VERSION "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")
	
    IF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS AND ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} VERSION_GREATER 4.7)
        MESSAGE(SEND_ERROR "When building Tulip python wheels on MacOS, Qt version must be less than 4.8."
                           "Indeed we need universal binaries (i386;x86_64) to build a binary wheel and Qt 4.7 "
                           "is the last Qt version to provide bundles with universal binaries.")
    ENDIF(APPLE AND ACTIVATE_PYTHON_WHEELS_TARGETS AND ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} VERSION_GREATER 4.7)

     # define aliases for Qt macros
    MACRO(QTX_WRAP_CPP outfiles )
      QT4_WRAP_CPP(${outfiles} ${ARGN})
    ENDMACRO()

    MACRO(QTX_WRAP_UI outfiles )
      QT4_WRAP_UI(${outfiles} ${ARGN})
    ENDMACRO()

    MACRO(QTX_ADD_RESOURCES outfiles )
      QT4_ADD_RESOURCES(${outfiles} ${ARGN})
    ENDMACRO()

ENDIF(${Qt5Widgets_FOUND} AND ${Qt5OpenGL_FOUND} AND
      ${Qt5Xml_FOUND} AND ${Qt5XmlPatterns_FOUND} AND ${Qt5Network_FOUND} AND
      ${Qt5WebKit_FOUND} AND ${Qt5WebKitWidgets_FOUND})
