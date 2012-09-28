CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(%ProjectName:l%)
SET(APPLICATION_NAME %ProjectName:l%)

FIND_PACKAGE( Qt4 REQUIRED)
FIND_PACKAGE( TULIP REQUIRED)
include(${QT_USE_FILE})

include_directories(${TULIP_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

##----------------------------------------------------------------------------------------------------------------------------
SET(APPLICATION_SRCS %ProjectName:l%.%CppSourceSuffix% )
ADD_EXECUTABLE(${APPLICATION_NAME} ${APPLICATION_SRCS})
TARGET_LINK_LIBRARIES(${APPLICATION_NAME} ${TULIP_LIBRARIES})
