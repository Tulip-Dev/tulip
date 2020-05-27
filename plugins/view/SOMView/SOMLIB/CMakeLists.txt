FILE(GLOB LIB_SRCS "*.cpp")

INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${CMAKE_CURRENT_SOURCE_DIR})
ADD_LIBRARY(som STATIC ${LIB_SRCS})
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(som PROPERTIES POSITION_INDEPENDENT_CODE ON)
TARGET_LINK_LIBRARIES(som ${LibTulipCoreName} ${LibTulipOGLName})
