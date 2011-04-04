IF(LINUX)
  FIND_PACKAGE(Doxygen)
  IF(DOXYGEN_FOUND)
    SET(TULIP_LIB_DESC "Tulip Graph Library")
    SET(TULIP_LIB_NAME "tulip")
    CONFIGURE_FILE("doxygen/tulip-lib.doxygen.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/tulip-lib.doxygen" @ONLY)
    SET(TULIP_LIB_DESC "Tulip Open GL Library")
    SET(TULIP_LIB_NAME "tulip-ogl")
    CONFIGURE_FILE("doxygen/tulip-lib.doxygen.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/tulip-ogl-lib.doxygen" @ONLY)
    SET(TULIP_LIB_DESC "Tulip Qt Library")
    SET(TULIP_LIB_NAME "tulip-qt")
    CONFIGURE_FILE("doxygen/tulip-lib.doxygen.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/tulip-qt-lib.doxygen" @ONLY)
    EXEC_PROGRAM(date ARGS "+%Y" OUTPUT_VARIABLE THIS_YEAR)
    CONFIGURE_FILE("doxygen/tulip.html.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/tulip.html")
    CONFIGURE_FILE("doxygen/header.html.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/header.html")
    CONFIGURE_FILE("doxygen/footer.html.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/footer.html")
    CONFIGURE_FILE("doxygen/header-allPlugins.html.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/header-allPlugins.html")
    CONFIGURE_FILE("doxygen/tulip-plugins.doxygen.in" "${CMAKE_CURRENT_BINARY_DIR}/doxygen/tulip-plugins.doxygen")
    CONFIGURE_FILE("developerHandbook/gen-devPdf.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/developerHandbook/gen-devPdf.sh")
    CONFIGURE_FILE("userHandbook/gen-userPdf.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/userHandbook/gen-userPdf.sh")

    include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

    add_subdirectory(common)
    add_subdirectory(doxygen)
    add_subdirectory(userHandbook)
    add_subdirectory(developerHandbook)

    ADD_CUSTOM_TARGET(tulip_qhcp ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tulip.qhcp ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhcp)
    ADD_CUSTOM_TARGET(tulip_qhp sh ${PROJECT_SOURCE_DIR}/docs/gen-tulipqhp.sh ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhp)
    ADD_CUSTOM_TARGET(tulip_qch qhelpgenerator ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhp -o ${CMAKE_CURRENT_BINARY_DIR}/tulip.qch)
    ADD_CUSTOM_TARGET(doc qcollectiongenerator ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhcp -o ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhc WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    ADD_DEPENDENCIES(tulip_qhp common_copy dev_toc dev_keywords dox_toc dox_keywords user_toc user_keywords allPlugins-html tulip-libs-html)
    ADD_DEPENDENCIES(tulip_qch tulip_qhp)
    ADD_DEPENDENCIES(doc tulip_qch tulip_qhcp)
  ENDIF()
ELSE(LINUX)
  MESSAGE("Doxygen not found: unable to generate documentation")
ENDIF(LINUX)


ADD_CUSTOM_TARGET(doc_generation ALL
${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhc
COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/tulip.qch
COMMENT "Cheking if documentation files are present")


INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tulip.qhc ${CMAKE_CURRENT_BINARY_DIR}/tulip.qch
  DESTINATION ${TulipShareInstallDir}/tulip
  COMPONENT tulip_doc)
