set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libleidenalg library")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/vtraag/libleidenalg")
set(CPACK_PACKAGE_VENDOR "Vincent Traag")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

#############################################################################
## Configuration of the source package
#############################################################################

# Set source package name and format
set(CPACK_SOURCE_PACKAGE_FILE_NAME "libleidenalg-${CMAKE_PROJECT_VERSION}")
set(CPACK_SOURCE_GENERATOR "TGZ")

# Declare what to include in the source tarball. Unfortunately we can only
# declare full directories here, not individual files.
set(
    CPACK_SOURCE_INSTALLED_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/etc/cmake;/etc/cmake"
    "${CMAKE_SOURCE_DIR}/include;/include"
    "${CMAKE_SOURCE_DIR}/src;/src"
)

# CPack is pretty dumb as it can only copy full directories (sans the ignored
# files) to the target tarball by default. We need to install also the
# correct version file.
set(CPACK_INSTALL_SCRIPT "${CMAKE_SOURCE_DIR}/etc/cmake/cpack_install_script.cmake")

# Ignore the build and all hidden folders
set(
    CPACK_SOURCE_IGNORE_FILES
    "\\\\..*/"
    "${CMAKE_SOURCE_DIR}/build"
)

#############################################################################
## Now we can include CPack
#############################################################################

include(CPack)
