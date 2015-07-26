file(GLOB SRCS "*.c" "*.cpp")

# Must be added to enable export macro
ADD_DEFINITIONS(-DQUAZIP_BUILD)

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

QTX_WRAP_CPP(MOC_SRCS quazipfile.h quaziodevice.h quagzipfile.h)
set(SRCS ${SRCS} ${MOC_SRCS})

add_library(${QuazipLibrary} SHARED ${SRCS})
set_target_properties(${QuazipLibrary} PROPERTIES VERSION 1.0.0 SOVERSION 1)
# Link against ZLIB_LIBRARIES if needed (on Windows this variable is empty)
target_link_libraries(${QuazipLibrary} ${QT_LIBRARIES} ${ZLIB_LIBRARIES})

INSTALL(TARGETS ${QuazipLibrary}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT quazip
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT quazip
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT quazip)

IF(ACTIVATE_PYTHON_WHEELS_TARGETS)
COPY_TARGET_LIBRARY_POST_BUILD(${QuazipLibrary} ${TULIPGUI_PYTHON_NATIVE_FOLDER})
ENDIF(ACTIVATE_PYTHON_WHEELS_TARGETS)
