SET(SRCS
  src/main.cpp
  src/TulipPerspectiveMainWindow.cpp
)

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_CPP(MOC_SRCS
  include/TulipPerspectiveMainWindow.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude} ${OPENGL_INCLUDE_DIR} include/)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

IF(WIN32)

STRING(TIMESTAMP CURRENT_YEAR "%Y")

INCLUDE(generate_product_version)
generate_product_version(
   VersionFiles
   NAME "Tulip Perspective launcher"
   ICON ${PROJECT_SOURCE_DIR}/library/tulip-gui/resources/icons/tulip.ico
   VERSION_MAJOR ${TulipMajorVersion}
   VERSION_MINOR ${TulipMinorVersion}
   VERSION_PATCH ${TulipReleaseVersion}
   COMPANY_NAME "LaBRI, University of Bordeaux"
   COMPANY_COPYRIGHT "(C) Copyright ${CURRENT_YEAR} David Auber and the Tulip development Team"
   ORIGINAL_FILENAME "tulip_perspective.exe"
)

SET(SRCS ${SRCS} ${VersionFiles})

ENDIF(WIN32)

ADD_EXECUTABLE(tulip_perspective ${SRCS} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(tulip_perspective crash_handling ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS tulip_perspective
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)

IF(WIN32)
  SET(TULIP_FIXUP_BUNDLE ON CACHE BOOL "Enable / disable the gathering and copy of Tulip DLL dependencies into the install bin folder")
  # That simple install command does the magic of gathering all required dll dependencies
  # in order to run Tulip and copy them to the bin install folder (thanks to the CMake module named BundleUtilities)
  # In the case where a dll dependency is not automatically found, you must add its path to the CMAKE_LIBRARY_PATH variable.
  IF(TULIP_FIXUP_BUNDLE)
    # copy required Qt plugins
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/imageformats DESTINATION ${TulipBinInstallDir})
    INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/platforms DESTINATION ${TulipBinInstallDir})
    IF(EXISTS ${QT_PLUGINS_DIR}/styles)
      INSTALL(DIRECTORY ${QT_PLUGINS_DIR}/styles DESTINATION ${TulipBinInstallDir})
    ENDIF(EXISTS ${QT_PLUGINS_DIR}/styles)

    # then schedule a call to FIXUP_BUNDLE at the end of the CMake install process
    # this process is configured here instead in the root CMakeLists.txt as
    # CMake inserts install commands before the ones defined in sub directories
    # and we need Tulip to be fully installed before running the bundle fixing process
    SET(APP "\${CMAKE_INSTALL_PREFIX}/bin/tulip_perspective.exe")
    SET(LIB_DIRS ${QT_BINARY_DIR} ${CMAKE_LIBRARY_PATH})
    STRING(REPLACE ";" "\;" LIB_DIRS "${LIB_DIRS}")
    GET_PROPERTY(FIXUP_BUNDLE_LIBS GLOBAL PROPERTY FIXUP_BUNDLE_LIBS)
    INSTALL(CODE
            "INCLUDE(BundleUtilities)
             FIXUP_BUNDLE(\"${APP}\" \"${FIXUP_BUNDLE_LIBS}\" \"${LIB_DIRS}\")"
    )
    # When generating Tulip installers using MSYS2, we need to bundle OpenSSL
    # libraries in order for the Geographic view to work correctly.
    # As Qt5 does not link directly to OpenSSL for license issue and thus use
    # a dynamic loading approach, we are forced to use the following trick to copy
    # the needed dlls into the Tulip install folder.
    IF(MINGW AND EXISTS ${MINGW_BIN_PATH}/openssl.exe)
      INSTALL(CODE
              "INCLUDE(BundleUtilities)
               FILE(COPY \"${MINGW_BIN_PATH}/openssl.exe\" DESTINATION \"\${CMAKE_INSTALL_PREFIX}/bin/\")
               FIXUP_BUNDLE(\"\${CMAKE_INSTALL_PREFIX}/bin/openssl.exe\" \"\" \"${LIB_DIRS}\")
               FILE(REMOVE \"\${CMAKE_INSTALL_PREFIX}/bin/openssl.exe\")"
      )
    ENDIF(MINGW AND EXISTS ${MINGW_BIN_PATH}/openssl.exe)
  ENDIF(TULIP_FIXUP_BUNDLE)
ENDIF(WIN32)
