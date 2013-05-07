INSTALL(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-2.4.api
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-2.5.api
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-2.6.api
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-2.7.api
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-3.1.api
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-3.2.api
        ${CMAKE_CURRENT_SOURCE_DIR}/Python-3.3.api
        ${CMAKE_CURRENT_SOURCE_DIR}/PyQt4.api
        DESTINATION ${TulipShareInstallDir}/apiFiles
        COMPONENT tulip_python)
