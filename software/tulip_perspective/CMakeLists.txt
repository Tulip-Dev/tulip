SET(SRCS
  src/main.cpp
  src/TulipPerspectiveMainWindow.cpp
)

QTX_WRAP_CPP(MOC_SRCS
  include/TulipPerspectiveMainWindow.h
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../crash_handling)

INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLInclude} ${TulipGUIInclude} include/)

IF(USE_QT5)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
ENDIF(USE_QT5)

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

TARGET_LINK_LIBRARIES(tulip_perspective crash_handling)

TARGET_LINK_LIBRARIES(tulip_perspective ${LibTulipCoreName} ${LibTulipOGLName} ${LibTulipGUIName} ${QT_LIBRARIES})

INSTALL(TARGETS tulip_perspective
  RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT tulip_app
  LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app
  ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT tulip_app)

IF(WIN32)
  # That simple install command does the magic of gathering all required dll dependencies
  # in order to run Tulip and copy them to the bin install folder (thanks to the CMake module named BundleUtilities)
  # In the case where a dll dependency is not automatically found, you must add its path to the CMAKE_LIBRARY_PATH variable.
  SET(APP "${CMAKE_INSTALL_PREFIX}/bin/tulip_perspective.exe")
  SET(LIB_DIRS ${QT_BINARY_DIR} ${CMAKE_LIBRARY_PATH})
  STRING(REPLACE ";" "\;" LIB_DIRS "${LIB_DIRS}")
  INSTALL(CODE
    "INCLUDE(BundleUtilities)
     FIXUP_BUNDLE(\"${APP}\"   \"\"   \"${LIB_DIRS}\")"
  )
ENDIF(WIN32)
