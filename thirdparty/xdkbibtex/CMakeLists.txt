INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)

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
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(xdkbibtex PROPERTIES POSITION_INDEPENDENT_CODE ON)
TARGET_LINK_LIBRARIES(xdkbibtex antlr)
