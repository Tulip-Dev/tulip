FILE(GLOB tulip_LIB_SRCS "*.cpp")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_TULIP_LIB_DIR=\"\\\"${CMAKE_INSTALL_PREFIX}/lib/\\\"\"")
INCLUDE_DIRECTORIES(../include/ ${TulipBuildInclude} ${PROJECT_SOURCE_DIR} ${ZLIB_INCLUDE_DIR})

ADD_LIBRARY (${LibTulipName} SHARED ${tulip_LIB_SRCS} ../include/tulip/TulipRelease.h)
TARGET_LINK_LIBRARIES(${LibTulipName} gzstream )

INSTALL(TARGETS ${LibTulipName}
       RUNTIME DESTINATION ${TulipBinInstallDir} COMPONENT libtulip
       LIBRARY DESTINATION ${TulipLibInstallDir} COMPONENT libtulip
       ARCHIVE DESTINATION ${TulipLibInstallDir} COMPONENT libtulip)
