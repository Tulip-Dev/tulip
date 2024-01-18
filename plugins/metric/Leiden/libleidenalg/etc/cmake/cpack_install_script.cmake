# Custom CPack install script that allows us to whitelist files to be copied
# to the tarball from the root directory, instead of copying the entire root
# directory recursively

if(CPACK_SOURCE_INSTALLED_DIRECTORIES)

    # Generate a version file in the build folder if we don't have one in the
    # source folder
    if(EXISTS "${SOURCE_DIR}/VERSION")
        set(VERSION_FILE "${SOURCE_DIR}/VERSION")
    else()
        execute_process(
            COMMAND "${CMAKE_COMMAND}"
            --build "${CPACK_PACKAGE_DIRECTORY}"
            --target versionfile
            RESULT_VARIABLE EXIT_CODE
        )
        message(INFO "Created VERSION file at ${CPACK_PACKAGE_DIRECTORY}/VERSION")
        if(NOT EXIT_CODE EQUAL 0)
            message(FATAL_ERROR "Failed to determine the version number of igraph that is being packaged.")
        endif()
        set(VERSION_FILE "${CPACK_PACKAGE_DIRECTORY}/VERSION")
    endif()

    list(GET CPACK_BUILD_SOURCE_DIRS 0 SOURCE_DIR)
    
    file(
        INSTALL
        "${SOURCE_DIR}/CMakeLists.txt"
        "${SOURCE_DIR}/LICENSE"
        "${SOURCE_DIR}/CHANGELOG"
        "${SOURCE_DIR}/libleidenalgConfig.cmake.in"
        "${VERSION_FILE}"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
      )
endif()
