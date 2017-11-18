/* $ANTLR 2.7.6 (20060903): "bibfile.g" -> "bibfileparser.cpp"$ */
#include "bibfileparser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>

// Will be included before parser methods implementations
// in parser.cpp
#include "bibcommandparser.hpp"
#include <iostream>

using namespace std;

bibfileparser::bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k) {
}

bibfileparser::bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1) {
}

bibfileparser::bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k) {
}

bibfileparser::bibfileparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1) {
}

bibfileparser::bibfileparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1) {
}

void bibfileparser::parse() {

  {
    // ( ... )*
    for (;;) {
      switch ( LA(1)) {
      case COMMENT: {
        comment();
        break;
      }

      case ARROBAS: {
        bibcommand();
        break;
      }

      default: {
        goto _loop3;
      }
      }
    }

_loop3:
    ;
  } // ( ... )*
  match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);

  pbf_->file()->setComment(pbf_->currentComment());
  pbf_->clearCurrentComment();

}

void bibfileparser::comment() {
  ANTLR_USE_NAMESPACE(antlr)RefToken  c = ANTLR_USE_NAMESPACE(antlr)nullToken;

  c = LT(1);
  match(COMMENT);

  pbf_->addToCurrentComment(c->getText());

}

void bibfileparser::bibcommand() {

  match(ARROBAS);

  bibcommandparser p(getInputState());
  p.setParsedBibFile(pbf_);
  p.parse();

}

void bibfileparser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& ) {
}
const char* bibfileparser::tokenNames[] = {
  "<0>",
  "EOF",
  "<2>",
  "NULL_TREE_LOOKAHEAD",
  "COMMENT",
  "ARROBAS",
  "ARROBASCOMMENT",
  "ARROBAS_OR_COMMENT",
  0
};



