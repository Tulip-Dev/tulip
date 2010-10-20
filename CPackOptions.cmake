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
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "${deps}")
  ENDIF()
ENDMACRO()

# Check what kind of CPack generator we are using
IF("${CPACK_GENERATOR}" STREQUAL "DEB")
  SET(CURRENT_COMPONENT "ALL")

  FOREACH(ITC ${CPACK_COMPONENTS_ALL})
    STRING(TOUPPER ${ITC} CN)
    SET(DEPS "")
    FOREACH(ITD ${CPACK_COMPONENT_${CN}_DEPENDS})
      SET(DEPS "${DEPS} ${ITD} (=${VERSION})")
    ENDFOREACH()
    STRING(REPLACE ") " "), " DEB_DEPS "${DEPS}")
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
