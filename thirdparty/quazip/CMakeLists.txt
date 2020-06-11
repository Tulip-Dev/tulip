FILE(GLOB SRCS "*.c" "*.cpp")

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})

QTX_SET_INCLUDES_AND_DEFINITIONS()

QTX_WRAP_CPP(MOC_SRCS quazipfile.h quaziodevice.h quagzipfile.h)
SET(SRCS ${SRCS} ${MOC_SRCS})

ADD_LIBRARY(${QuazipLibrary} STATIC ${SRCS})
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(${QuazipLibrary} PROPERTIES POSITION_INDEPENDENT_CODE ON)
# Link against ZLIB_LIBRARIES if needed (on Windows this variable is empty)
TARGET_LINK_LIBRARIES(${QuazipLibrary} ${QT_LIBRARIES} ${ZLIB_LIBRARIES})
