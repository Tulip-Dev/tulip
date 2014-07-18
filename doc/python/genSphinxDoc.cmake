# Turn back the CMAKE_LIBRARY_PATH string into a list
STRING(REPLACE "," ";" CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}")

# On windows, we need to add the paths to all the dll dependencies of the tulip Python modules
# in the PATH environment variable, otherwise the import of the tulip modules will fail
IF(WIN32)

  IF(MSVC)
    SET(DEPENDENCIES_PATHS "${BIN_DIR}/../../library/tulip-core/src/Debug"
                           "${BIN_DIR}/../../library/tulip-gui/src/Debug"
                           "${BIN_DIR}/../../library/tulip-ogl/src/Debug"
                           "${BIN_DIR}/../../library/tulip-ogdf/src/Debug"
                           "${BIN_DIR}/../../library/tulip-python/src/Debug"
                           "${BIN_DIR}/../../thirdparty/gzstream/Debug"
                           "${BIN_DIR}/../../thirdparty/qxt/Debug"
                           "${BIN_DIR}/../../thirdparty/quazip/Debug"
                           "${BIN_DIR}/../../thirdparty/ftgl/Debug"
                           "${BIN_DIR}/../../thirdparty/OGDF/Debug"
                           "${BIN_DIR}/../../thirdparty/yajl/src/Debug"
                           "${BIN_DIR}/../../library/tulip-core/src/Release"
                           "${BIN_DIR}/../../library/tulip-gui/src/Release"
                           "${BIN_DIR}/../../library/tulip-ogl/src/Release"
                           "${BIN_DIR}/../../library/tulip-ogdf/src/Release"
                           "${BIN_DIR}/../../library/tulip-python/src/Release"
                           "${BIN_DIR}/../../thirdparty/gzstream/Release"
                           "${BIN_DIR}/../../thirdparty/qxt/Release"
                           "${BIN_DIR}/../../thirdparty/quazip/Release"
                           "${BIN_DIR}/../../thirdparty/ftgl/Release"
                           "${BIN_DIR}/../../thirdparty/OGDF/Release"
                           "${BIN_DIR}/../../thirdparty/yajl/src/Release"
                           "${BIN_DIR}/../../library/tulip-core/src/RelWithDebInfo"
                           "${BIN_DIR}/../../library/tulip-gui/src/RelWithDebInfo"
                           "${BIN_DIR}/../../library/tulip-ogl/src/RelWithDebInfo"
                           "${BIN_DIR}/../../library/tulip-ogdf/src/RelWithDebInfo"
                           "${BIN_DIR}/../../library/tulip-python/src/RelWithDebInfo"
                           "${BIN_DIR}/../../thirdparty/gzstream/RelWithDebInfo"
                           "${BIN_DIR}/../../thirdparty/qxt/RelWithDebInfo"
                           "${BIN_DIR}/../../thirdparty/quazip/RelWithDebInfo"
                           "${BIN_DIR}/../../thirdparty/ftgl/RelWithDebInfo"
                           "${BIN_DIR}/../../thirdparty/OGDF/RelWithDebInfo"
                           "${BIN_DIR}/../../thirdparty/yajl/src/RelWithDebInfo"
                           "${BIN_DIR}/../../library/tulip-core/src/MinSizeRel"
                           "${BIN_DIR}/../../library/tulip-gui/src/MinSizeRel"
                           "${BIN_DIR}/../../library/tulip-ogl/src/MinSizeRel"
                           "${BIN_DIR}/../../library/tulip-ogdf/src/MinSizeRel"
                           "${BIN_DIR}/../../library/tulip-python/src/MinSizeRel"
                           "${BIN_DIR}/../../thirdparty/gzstream/MinSizeRel"
                           "${BIN_DIR}/../../thirdparty/qxt/MinSizeRel"
                           "${BIN_DIR}/../../thirdparty/quazip/MinSizeRel"
                           "${BIN_DIR}/../../thirdparty/ftgl/MinSizeRel"
                           "${BIN_DIR}/../../thirdparty/OGDF/MinSizeRel"
                           "${BIN_DIR}/../../thirdparty/yajl/src/MinSizeRel"
                           "${CMAKE_LIBRARY_PATH}"
                           "${QT_BIN_DIR}")
  ELSE(MSVC)
    SET(DEPENDENCIES_PATHS "${BIN_DIR}/../../library/tulip-core/src"
                           "${BIN_DIR}/../../library/tulip-gui/src"
                           "${BIN_DIR}/../../library/tulip-ogl/src"
                           "${BIN_DIR}/../../library/tulip-ogdf/src/"
                           "${BIN_DIR}/../../library/tulip-python/src/"
                           "${BIN_DIR}/../../thirdparty/gzstream"
                           "${BIN_DIR}/../../thirdparty/qxt"
                           "${BIN_DIR}/../../thirdparty/quazip"
                           "${BIN_DIR}/../../thirdparty/ftgl"
                           "${BIN_DIR}/../../thirdparty/OGDF"
                           "${BIN_DIR}/../../thirdparty/yajl/src"
                           "${CMAKE_LIBRARY_PATH}"
                           "${QT_BIN_DIR}")
  ENDIF(MSVC)

  SET(ENV{PATH} "${DEPENDENCIES_PATHS};$ENV{PATH}")

ENDIF(WIN32)

# Add the paths of the tulip Python modules in the PYTHONPATH environement variable
# in order for the interpreter to import them
IF(MSVC)
  SET(PYTHON_PATHS "${BIN_DIR}/../../library/tulip-python/bindings/stl/Debug"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-core/Debug"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-ogl/Debug"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-gui/Debug"
                   "${BIN_DIR}/../../library/tulip-python/bindings/stl/Release"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-core/Release"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-ogl/Release"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-gui/Release"
                   "${BIN_DIR}/../../library/tulip-python/bindings/stl/RelWithDebInfo"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-core/RelWithDebInfo"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-ogl/RelWithDebInfo"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-gui/RelWithDebInfo"
                   "${BIN_DIR}/../../library/tulip-python/bindings/stl/MinSizeRel"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-core/MinSizeRel"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-ogl/MinSizeRel"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-gui/MinSizeRel"
                   "${SRC_DIR}/../../library/tulip-python/modules")
ELSE(MSVC)
  SET(PYTHON_PATHS "${BIN_DIR}/../../library/tulip-python/bindings/stl"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-core"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-ogl"
                   "${BIN_DIR}/../../library/tulip-python/bindings/tulip-gui"
                   "${SRC_DIR}/../../library/tulip-python/modules")
ENDIF(MSVC)

# Add the path of the sip Python module if we compile it from thirdparty as
# the tulip modules depend on it
IF(NOT SYSTEM_SIP)
  IF(MSVC)
    SET(PYTHON_PATHS "${BIN_DIR}/../../thirdparty/sip-${SIP_VERSION}/siplib/Debug"
                     "${BIN_DIR}/../../thirdparty/sip-${SIP_VERSION}/siplib/Release"
                     "${BIN_DIR}/../../thirdparty/sip-${SIP_VERSION}/siplib/RelWithDebInfo"
                     "${BIN_DIR}/../../thirdparty/sip-${SIP_VERSION}/siplib/MinSizeRel"
                     "${PYTHON_PATHS}")
  ELSE(MSVC)
    SET(PYTHON_PATHS "${BIN_DIR}/../../thirdparty/sip-${SIP_VERSION}/siplib"
                     "${PYTHON_PATHS}")
  ENDIF(MSVC)
ENDIF(NOT SYSTEM_SIP)

# On non windows systems, the character separating the paths in an environement variable
# is a ':' not a ';' (generated by CMake when turning a list into string)
IF(NOT WIN32)
  STRING(REPLACE ";" ":" PYTHON_PATHS "${PYTHON_PATHS}")
ENDIF(NOT WIN32)

SET(ENV{PYTHONPATH} "${PYTHON_PATHS}")

# Fix an encoding issue on Mac OS
IF(APPLE)
  SET(ENV{LC_ALL} "en_EN.UTF-8")
ENDIF(APPLE)

# Everything is now set up, we can generate the documenation
EXECUTE_PROCESS(COMMAND ${SPHINX_EXECUTABLE} -b html -E -d ${BIN_DIR}/doctrees ${SOURCE_DIR} ${BIN_DIR}/html)
