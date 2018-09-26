FILE(GLOB SRCS "*.c" "*.cpp")

# Must be added to enable export macro
ADD_DEFINITIONS(-DQUAZIP_BUILD)

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_CPP(MOC_SRCS quazipfile.h quaziodevice.h quagzipfile.h)
SET(SRCS ${SRCS} ${MOC_SRCS})

ADD_LIBRARY(${QuazipLibrary} SHARED ${SRCS})
SET_TARGET_PROPERTIES(${QuazipLibrary} PROPERTIES VERSION 1.0.0 SOVERSION 1)
# Link against ZLIB_LIBRARIES if needed (on Windows this variable is empty)
TARGET_LINK_LIBRARIES(${QuazipLibrary} ${QT_LIBRARIES} ${ZLIB_LIBRARIES})

INSTALL(TARGETS ${QuazipLibrary}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT quazip
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT quazip
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT quazip)
