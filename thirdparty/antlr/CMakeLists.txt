INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)

IF(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF(UNIX)

DISABLE_COMPILER_WARNINGS()

SET(ANTLR_SRCS
  ANTLRUtil.cpp
  ASTFactory.cpp
  ASTNULLType.cpp
  ASTRefCount.cpp
  BaseAST.cpp
  BitSet.cpp
  CharBuffer.cpp
  CharScanner.cpp
  CommonAST.cpp
  CommonASTWithHiddenTokens.cpp
  CommonHiddenStreamToken.cpp
  CommonToken.cpp
  InputBuffer.cpp
  LLkParser.cpp
  MismatchedCharException.cpp
  MismatchedTokenException.cpp
  NoViableAltException.cpp
  NoViableAltForCharException.cpp
  Parser.cpp
  RecognitionException.cpp
  String.cpp
  TokenBuffer.cpp
  Token.cpp
  TokenRefCount.cpp
  TokenStreamBasicFilter.cpp
  TokenStreamHiddenTokenFilter.cpp
  TokenStreamRewriteEngine.cpp
  TokenStreamSelector.cpp
  TreeParser.cpp)

ADD_LIBRARY(antlr STATIC ${ANTLR_SRCS})
