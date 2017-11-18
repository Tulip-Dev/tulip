/* $ANTLR 2.7.6 (20060903): "value.g" -> "valueparser.cpp"$ */
#include "valueparser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
#line 37 "value.g"

// Will be included before parser methods implementations
// in parser.cpp
#include "value.h"
#include <iostream>

using namespace std;
using namespace xdkbib;

void valueparser::setSplitWord(const string s)
#if __cplusplus < 201103L
throw(invalid_argument)
#endif
{
  try {
    splitText_.clear();
    splitText_.readFrom(s);

    if (splitText_.nbWords() > 1) {
      throw invalid_argument(s+" is not a single word");
    }

    splitWord_ = splitText_.words_[0];
  }
  catch (parsing_error e) {
    throw invalid_argument(s+" is not a valid Text");
  }
}

#line 35 "valueparser.cpp"
valueparser::valueparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k) {
}

valueparser::valueparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,2) {
}

valueparser::valueparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k) {
}

valueparser::valueparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,2) {
}

valueparser::valueparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(state,2) {
}

void valueparser::parse() {

  {
    switch ( LA(1)) {
    case WS:
    case LETTER:
    case LBRACE:
    case TOKEN: {
      text();
      break;
    }

    case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE: {
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }
  match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
}

void valueparser::text() {

  {
    switch ( LA(1)) {
    case WS: {
      match(WS);
      break;
    }

    case LETTER:
    case LBRACE:
    case TOKEN: {
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }
  word();
  {
    // ( ... )*
    for (;;) {
      if ((LA(1) == WS)) {
        match(WS);
        {
          switch ( LA(1)) {
          case LETTER:
          case LBRACE:
          case TOKEN: {
            word();
            break;
          }

          case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
          case WS:
          case RBRACE: {
            break;
          }

          default: {
            throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
          }
          }
        }
      }
      else {
        goto _loop7;
      }

    }

_loop7:
    ;
  } // ( ... )*
}

void valueparser::word() {
#line 91 "value.g"

  Word* w;
  Letter* l;

#line 149 "valueparser.cpp"

#line 97 "value.g"

  w = new Word();

#line 155 "valueparser.cpp"
  {
    // ( ... )+
    int _cnt10=0;

    for (;;) {
      if ((LA(1) == LETTER || LA(1) == LBRACE || LA(1) == TOKEN)) {
        l=letter();
#line 100 "value.g"
        w->letters_.push_back(l);
#line 163 "valueparser.cpp"
      }
      else {
        if ( _cnt10>=1 ) {
          goto _loop10;
        }
        else {
          throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
        }
      }

      _cnt10++;
    }

_loop10:
    ;
  }  // ( ... )+
#line 101 "value.g"

  // Here we check if we cannot subsplit the word
  if (splitWord_ && splitWord_->nbLetters()) {
    unsigned int start = 0;

    for (int i=0; i+splitWord_->nbLetters()<=w->nbLetters(); ++i) {
      bool b = true;

      for (unsigned int l=0; l<splitWord_->nbLetters() & b; ++l) {
        b &= ((*w)[i+l]->content() ==
              (*splitWord_)[l]->content());
      }

      // TODO: remove empty words (yes,no,as an option?)
      if (b) {
        if (start != i) {
          Word* wk = new Word();

          for (unsigned int l=start; l<i; ++l) {
            wk->letters_.push_back(w->letters_[l]);
          }

          textStack_.top()->words_.push_back(wk);
        }

        textStack_.top()->words_.push_back(splitWord_->clone());

        start = (i += splitWord_->nbLetters());
      }
    }

    if (start < w->nbLetters()) {
      Word* wk = new Word();

      for (unsigned int l=start; l<w->nbLetters(); ++l) {
        wk->letters_.push_back(w->letters_[l]);
      }

      textStack_.top()->words_.push_back(wk);

    }

    w->letters_.clear();
    delete w;
  }
  else {
    textStack_.top()->words_.push_back(w);
  }

#line 213 "valueparser.cpp"
}

xdkbib::Letter*  valueparser::letter() {
#line 141 "value.g"
  xdkbib::Letter* l;
#line 219 "valueparser.cpp"
  ANTLR_USE_NAMESPACE(antlr)RefToken  c = ANTLR_USE_NAMESPACE(antlr)nullToken;
  ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;

  switch ( LA(1)) {
  case LETTER: {
    c = LT(1);
    match(LETTER);
#line 143 "value.g"

    l = new SingleLetter(c->getText()[0]);

#line 232 "valueparser.cpp"
    break;
  }

  case TOKEN: {
    t = LT(1);
    match(TOKEN);
#line 163 "value.g"

    l = new TokenLetter(t->getText());

#line 243 "valueparser.cpp"
    break;
  }

  default:
    if ((LA(1) == LBRACE) && (_tokenSet_0.member(LA(2)))) {
      match(LBRACE);
#line 147 "value.g"

      PseudoLetter* pl = new PseudoLetter();
      textStack_.push(pl->text_);
      l = pl;

#line 255 "valueparser.cpp"
      text();
#line 153 "value.g"

      textStack_.pop();

#line 261 "valueparser.cpp"
      match(RBRACE);
    }
    else if ((LA(1) == LBRACE) && (LA(2) == RBRACE)) {
      match(LBRACE);
#line 158 "value.g"

      l = new PseudoLetter();

#line 270 "valueparser.cpp"
      match(RBRACE);
    }
    else {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
  }

  return l;
}

void valueparser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& ) {
}
const char* valueparser::tokenNames[] = {
  "<0>",
  "EOF",
  "<2>",
  "NULL_TREE_LOOKAHEAD",
  "WS",
  "LETTER",
  "LBRACE",
  "RBRACE",
  "TOKEN",
  0
};

const unsigned long valueparser::_tokenSet_0_data_[] = { 368UL, 0UL, 0UL, 0UL };
// WS LETTER LBRACE TOKEN
const ANTLR_USE_NAMESPACE(antlr)BitSet valueparser::_tokenSet_0(_tokenSet_0_data_,4);


