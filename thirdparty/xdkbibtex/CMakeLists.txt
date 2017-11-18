INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)

IF(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

DISABLE_COMPILER_WARNINGS()

SET(XDKBIBTEX_SRCS
  base.C
  entry.C
  file.C
  formatter.C
  value.C
  parsedbibfile.C
  author.C
  bibcommandlexer.cpp
  bibcommandparser.cpp
  bibfilelexer.cpp
  bibfileparser.cpp
  valuelexer.cpp
  valueparser.cpp)

ADD_LIBRARY(xdkbibtex STATIC ${XDKBIBTEX_SRCS})
TARGET_LINK_LIBRARIES(xdkbibtex antlr)
