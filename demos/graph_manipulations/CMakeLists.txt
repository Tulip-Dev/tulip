SET(APPLICATION_NAME graphmanipulations)

include_directories(${TULIP_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(APPLICATION_SRCS graphmanipulations.cpp )
ADD_EXECUTABLE(${APPLICATION_NAME} ${APPLICATION_SRCS})
TARGET_LINK_LIBRARIES(${APPLICATION_NAME} ${TULIP_LIBRARIES} ${QT_LIBRARIES})

INSTALL(TARGETS ${APPLICATION_NAME} DESTINATION demos)
