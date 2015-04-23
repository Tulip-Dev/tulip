IF(PYTHONINTERP_FOUND AND SIP_OK)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in ${CMAKE_CURRENT_SOURCE_DIR}/conf.py)

# On windows, it is required that the paths to the dll dependencies (e.g. zlib, libpng, ...) are contained
# in the CMAKE_LIBRARY_PATH variable (especially with Visual Studio).
# Some warnings will be output when configuring the whole project if it is not the case.
# Otherwise, the Python API documentation will not be generated as the import of the tulip modules will fail.

# Turn CMAKE_LIBRARY_PATH list into a string in order to pass all its contents to the CMake command generating the doc,
# only the first list entry is transferred otherwise.
STRING(REPLACE ";" "," CMAKE_LIBRARY_PATH_STR "${CMAKE_LIBRARY_PATH}")

ADD_CUSTOM_TARGET(pythonBindingsDoc ALL ${CMAKE_COMMAND} -DSPHINX_EXECUTABLE=${SPHINX_EXECUTABLE} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                                                         -DBIN_DIR=${CMAKE_CURRENT_BINARY_DIR} -DSRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                                                         -DSYSTEM_SIP=${SYSTEM_SIP} -DSIP_VERSION=${SIP_VERSION_STR} -DQT_BIN_DIR=${QT_BINARY_DIR}
                                                         -DMSVC=${MSVC} -DAPPLE=${APPLE} -DCMAKE_LIBRARY_PATH=${CMAKE_LIBRARY_PATH_STR}
                                                         -P ${CMAKE_CURRENT_SOURCE_DIR}/genSphinxDoc.cmake
                                    DEPENDS ${LibTulipCorePythonBindingsName} ${LibTulipOGLPythonBindingsName} ${LibTulipGUIPythonBindingsName} ${LibStlPythonBindingsName} VERBATIM)

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "doc;doctrees") 

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html 
        DESTINATION ${TulipShareInstallDir}/doc/tulip-python
        COMPONENT tulip_python_doc)

ENDIF(PYTHONINTERP_FOUND AND SIP_OK)
