# files coming from https://github.com/vtraag/libleidenalg
# are stored in libleidenalg subdirectory
SET(LLA_DIR libleidenalg)
FILE(GLOB LLA_SRCS "${LLA_DIR}/src/*Partition.cpp" ${LLA_DIR}/src/Optimiser.cpp)
FILE(GLOB LLA_HEADERS "${LLA_DIR}/include/*Partition.h" ${LLA_DIR}/include/Optimiser.h)

# copy the original headers
FILE(COPY ${LLA_HEADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${LLA_DIR}/include)

# copy the tulip adapted version of GraphHelper.h
ADD_CUSTOM_COMMAND(OUTPUT ${LLA_DIR}/include/GraphHelper.h
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/TulipGraphHelper.h ${LLA_DIR}/include/GraphHelper.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/TulipGraphHelper.h
  VERBATIM)

# hide compilation warnings coming from libleidenalg source code
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast -Wno-unused-parameter -Wno-unused-but-set-variable")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${LLA_DIR}/include ${TulipCoreBuildInclude} ${TulipCoreInclude})

# build the plugin lib with the tulip adapted version
# of GraphHelper.[h|cpp] files
ADD_LIBRARY(Leiden-${Tulip_VERSION} SHARED ${LLA_DIR}/include/GraphHelper.h LeidenClustering.cpp TulipGraphHelper.cpp ${LLA_SRCS})

TARGET_LINK_LIBRARIES(Leiden-${Tulip_VERSION} ${LibTulipCoreName})

TULIP_INSTALL_PLUGIN(Leiden-${Tulip_VERSION} ${TulipPluginsInstallDir})
