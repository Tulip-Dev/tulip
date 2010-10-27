# A one-string version number
SET(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

# Build the package file name from the component base name
MACRO(GET_PACKAGE_NAME basename)
  # Replacing occurences of _ in package name (not supported in debian web repositories) by -
  STRING(REPLACE "_" "-" FILE_PREFIX ${basename})
  STRING(TOUPPER ${basename} BN)
  SET(${BN}_PACKAGE "${FILE_PREFIX}-${VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
ENDMACRO()

# Get the CMake package build informations
IF("${CPACK_INSTALL_CMAKE_PROJECTS}" MATCHES "([^\\;]*)\\;([^\\;]*)\\;([^\\;]*)\\;([^\\;]*)")
  SET(BUILD_DIR "${CMAKE_MATCH_1}")
  SET(PROJECT_NAME "${CMAKE_MATCH_2}")
  SET(PROJECT_COMPONENT "${CMAKE_MATCH_3}")
  SET(PACKAGE_DIR "${CMAKE_MATCH_4}")
ENDIF()

# Prepare DEB specific variables
MACRO(PREPARE_DEB basename deps)
  GET_PACKAGE_NAME(${basename})
  STRING(TOUPPER ${basename} TMP)
  IF(NOT EXISTS "${BUILD_DIR}/${${TMP}_PACKAGE}.deb")
    SET(CURRENT_COMPONENT ${basename})
    SET(CURRENT_PACKAGE ${${TMP}_PACKAGE})
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CPACK_COMPONENT_${TMP}_DISPLAY_NAME}")
    SET(CPACK_PACKAGE_DESCRIPTION "${CPACK_COMPONENT_${TMP}_DESCRIPTION}")
    IF(NOT "${deps}" STREQUAL "")
      SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${deps}")
    ENDIF()
  ENDIF()
ENDMACRO()

# Check what kind of CPack generator we are using
IF("${CPACK_GENERATOR}" STREQUAL "DEB")
  # Set external dependencies for deb packages
  SET(LIBTULIP_EXTERNAL_DEB_DEPS "zlib1g, libstdc++6, libc6, libgomp1, libgcc1")
  SET(LIBTULIP_OGL_EXTERNAL_DEB_DEPS "libglew1.5, libglu1-mesa, libgl1-mesa-glx, libxml2, libjpeg62, libpng12-0, libfreetype6, zlib1g, libstdc++6, libc6, libgomp1, libgcc1, libxmu6, libxi6, libxext6, libx11-6, libxt6, libxcb1, libsm6, libice6, libxau6, libxdmcp6, libuuid1")
  SET(LIBTULIP_QT_EXTERNAL_DEB_DEPS "libqtcore4, libqtgui4, libqt4-opengl, zlib1g, libglew1.5, libglu1-mesa, libgl1-mesa-glx, libxml2, libjpeg62, libpng12-0, libfreetype6, libstdc++6, libc6, libgomp1, libgcc1, libglib2.0-0, libqt4-dbus, libfontconfig1, libaudio2, libglib2.0-0, libsm6, libice6, libxrender1, libxext6, libx11-6, libxmu6, libxi6, libpcre3, libqt4-xml, libexpat1, libxt6, libxau6, libuuid1, libxcb1, libxdmcp6")
  SET(TULIP_PLUGINSMANAGER_EXTERNAL_DEB_DEPS "libqtcore4, libqtgui4, libqt4-xml, libqt4-network, zlib1g, libglew1.5, libglu1-mesa, libgl1-mesa-glx, libxml2, libjpeg62, libpng12-0, libfreetype6, libqt4-opengl, libstdc++6, libc6, libgomp1, libgcc1, libglib2.0-0, libqt4-dbus, libfontconfig1, libaudio2, libglib2.0-0, libsm6, libice6, libxrender1, libxext6, libx11-6, libxmu6, libxi6, libc6, libpcre3, libexpat1, libxt6, libxau6, libuuid1, libxcb1, libxdmcp6")
  SET(TULIP_APP_EXTERNAL_DEB_DEPS "libfreetype6, libqtgui4, libqtcore4, libqt4-xml, libqt4-opengl, libglu1-mesa, libgl1-mesa-glx, libxml2, libjpeg62, zlib1g, libglew1.5, libpng12-0, libqt4-network, libstdc++6, libc6, libgomp1, libgcc1, libqt4-dbus, libfontconfig1, libaudio2, libglib2.0-0, libsm6, libice6, libxrender1, libxext6, libx11-6, libglib2.0-0, libxmu6, libxi6, libexpat1, libxt6, libxau6, libpcre3, libuuid1, libxcb1, libxdmcp6")

  SET(CURRENT_COMPONENT "ALL")

  FOREACH(ITC ${CPACK_COMPONENTS_ALL})
    STRING(TOUPPER ${ITC} CN)
    SET(DEPS "")
    FOREACH(ITD ${CPACK_COMPONENT_${CN}_DEPENDS})
      STRING(REPLACE "_" "-" DEP_NAME ${ITD})
      SET(DEPS "${DEPS} ${DEP_NAME} (=${VERSION})")
    ENDFOREACH()
    STRING(REPLACE ") " "), " DEB_DEPS "${DEPS}")

    IF(NOT ${DEB_DEPS} STREQUAL "" AND ${CN}_EXTERNAL_DEB_DEPS)
      SET(DEB_DEPS "${DEB_DEPS}, ")
    ENDIF()
    IF(${CN}_EXTERNAL_DEB_DEPS)
      SET(DEB_DEPS "${DEB_DEPS}${${CN}_EXTERNAL_DEB_DEPS}")
    ENDIF()

    PREPARE_DEB("${ITC}" "${DEB_DEPS}")
  ENDFOREACH()

  IF(${CURRENT_COMPONENT} STREQUAL "ALL")
    MESSAGE(FATAL_ERROR "Some .deb packages from an older build were not deleted.")
  ENDIF()

  SET(CPACK_INSTALL_CMAKE_PROJECTS "${BUILD_DIR};${CURRENT_COMPONENT};${CURRENT_COMPONENT};${PACKAGE_DIR}")

  # Replacing occurences of _ in package name (not supported in debian web repositories) by -
  STRING(REPLACE "_" "-" CURRENT_COMPONENT ${CURRENT_COMPONENT})
  SET(CPACK_PACKAGE_NAME ${CURRENT_COMPONENT})
  SET(CPACK_PACKAGE_FILE_NAME ${CURRENT_PACKAGE})
ENDIF()
