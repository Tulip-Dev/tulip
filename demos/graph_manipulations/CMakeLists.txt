SET(APPLICATION_NAME graphmanipulations)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(APPLICATION_SRCS graphmanipulations.cpp )
ADD_EXECUTABLE(${APPLICATION_NAME} ${APPLICATION_SRCS})
TARGET_LINK_LIBRARIES(${APPLICATION_NAME} ${LibTulipCoreName})

INSTALL(TARGETS ${APPLICATION_NAME} DESTINATION demos)
