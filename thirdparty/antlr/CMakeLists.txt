INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/..)

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
# allow the static library to be used to build shared libs
SET_TARGET_PROPERTIES(antlr PROPERTIES POSITION_INDEPENDENT_CODE ON)
