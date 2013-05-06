SET(APPLICATION_NAME graphmanipulations)

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${TulipGUIInclude})

##----------------------------------------------------------------------------------------------------------------------------
SET(APPLICATION_SRCS graphmanipulations.cpp )
ADD_EXECUTABLE(${APPLICATION_NAME} ${APPLICATION_SRCS})
TARGET_LINK_LIBRARIES(${APPLICATION_NAME} ${TulipLibs} ${QT_LIBRARIES})

INSTALL(TARGETS ${APPLICATION_NAME} DESTINATION demos)
