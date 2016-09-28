FILE(GLOB LIB_SRCS "*.cpp")

<<<<<<< HEAD
INCLUDE_DIRECTORIES(${TulipCoreInclude} ${TulipCoreBuildInclude} ${TulipOGLESInclude} ${CMAKE_CURRENT_SOURCE_DIR})
=======
INCLUDE_DIRECTORIES(${TulipCoreBuildInclude} ${TulipCoreInclude} ${TulipOGLInclude} ${CMAKE_CURRENT_SOURCE_DIR})
>>>>>>> 530af02... ensure Tulip build include folders are searched before the source include ones : prevent possible bad compilation surprises when accidentally running the cmake command in the tulip root source folder
IF(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)
ADD_LIBRARY(som STATIC ${LIB_SRCS})
TARGET_LINK_LIBRARIES(som ${LibTulipCoreName} ${LibTulipOGLESName})
