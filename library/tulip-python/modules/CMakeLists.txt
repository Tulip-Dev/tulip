IF(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py DESTINATION ${TulipBinInstallDir}/python COMPONENT tulip_python)
ELSE(WIN32)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/tulipplugins.py DESTINATION ${TulipLibInstallDir}/python COMPONENT tulip_python)
ENDIF()

