/* $ANTLR 2.7.6 (20060903): "bibcommand.g" -> "bibcommandlexer.cpp"$ */
#include "bibcommandlexer.hpp"
#include <antlr/CharBuffer.hpp>
#include <antlr/TokenStreamException.hpp>
#include <antlr/TokenStreamIOException.hpp>
#include <antlr/TokenStreamRecognitionException.hpp>
#include <antlr/CharStreamException.hpp>
#include <antlr/CharStreamIOException.hpp>
#include <antlr/NoViableAltForCharException.hpp>


// Will be included before lexer methods implementations
// in lexer.cpp
#include <sstream>
#include <iostream>

using namespace std;

bibcommandlexer::bibcommandlexer(ANTLR_USE_NAMESPACE(std)istream& in)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in),false) {
  initLiterals();
}

bibcommandlexer::bibcommandlexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(ib,false) {
  initLiterals();
}

bibcommandlexer::bibcommandlexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(state,false) {
  initLiterals();
}

void bibcommandlexer::initLiterals() {
}

ANTLR_USE_NAMESPACE(antlr)RefToken bibcommandlexer::nextToken() {
  ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;

  for (;;) {
    ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
    int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
    resetText();

    try {   // for lexical and char stream error handling
      switch ( LA(1)) {
      case 0x23 /* '#' */ : {
        mPOUND(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x28 /* '(' */ : {
        mLPARENTHESIS(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x29 /* ')' */ : {
        mRPARENTHESIS(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x7d /* '}' */ : {
        mRBRACE(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x2c /* ',' */ : {
        mCOMA(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x3d /* '=' */ : {
        mEQUAL(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x61 /* 'a' */ :
      case 0x62 /* 'b' */ :
      case 0x63 /* 'c' */ :
      case 0x64 /* 'd' */ :
      case 0x65 /* 'e' */ :
      case 0x66 /* 'f' */ :
      case 0x67 /* 'g' */ :
      case 0x68 /* 'h' */ :
      case 0x69 /* 'i' */ :
      case 0x6a /* 'j' */ :
      case 0x6b /* 'k' */ :
      case 0x6c /* 'l' */ :
      case 0x6d /* 'm' */ :
      case 0x6e /* 'n' */ :
      case 0x6f /* 'o' */ :
      case 0x70 /* 'p' */ :
      case 0x71 /* 'q' */ :
      case 0x72 /* 'r' */ :
      case 0x73 /* 's' */ :
      case 0x74 /* 't' */ :
      case 0x75 /* 'u' */ :
      case 0x76 /* 'v' */ :
      case 0x77 /* 'w' */ :
      case 0x78 /* 'x' */ :
      case 0x79 /* 'y' */ :
      case 0x7a /* 'z' */ : {
        mNAME(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x30 /* '0' */ :
      case 0x31 /* '1' */ :
      case 0x32 /* '2' */ :
      case 0x33 /* '3' */ :
      case 0x34 /* '4' */ :
      case 0x35 /* '5' */ :
      case 0x36 /* '6' */ :
      case 0x37 /* '7' */ :
      case 0x38 /* '8' */ :
      case 0x39 /* '9' */ : {
        mNUMBER(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x22 /* '\"' */ : {
        mSTRING(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x7b /* '{' */ : {
        mLBRACE_OR_VALUE(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x9 /* '\t' */ :
      case 0xa /* '\n' */ :
      case 0xd /* '\r' */ :
      case 0x20 /* ' ' */ : {
        mWS(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x40 /* '@' */ : {
        mSTRINGDEF_OR_PREAMBLE_OR_TYPE(true);
        theRetToken=_returnToken;
        break;
      }

      default: {
        if (LA(1)==EOF_CHAR) {
          uponEOF();
          _returnToken = makeToken(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
        }
        else {
          throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
        }
      }
      }

      if ( !_returnToken )
        goto tryAgain; // found SKIP token

      _ttype = _returnToken->getType();
      _ttype = testLiteralsTable(_ttype);
      _returnToken->setType(_ttype);
      return _returnToken;
    }
    catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& e) {
      throw ANTLR_USE_NAMESPACE(antlr)TokenStreamRecognitionException(e);
    }
    catch (ANTLR_USE_NAMESPACE(antlr)CharStreamIOException& csie) {
      throw ANTLR_USE_NAMESPACE(antlr)TokenStreamIOException(csie.io);
    }
    catch (ANTLR_USE_NAMESPACE(antlr)CharStreamException& cse) {
      throw ANTLR_USE_NAMESPACE(antlr)TokenStreamException(cse.getMessage());
    }

tryAgain:
    ;
  }
}

void bibcommandlexer::mPOUND(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = POUND;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('#' /* charlit */ );

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mLPARENTHESIS(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = LPARENTHESIS;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('(' /* charlit */ );

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mRPARENTHESIS(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = RPARENTHESIS;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match(')' /* charlit */ );

  if ( inputState->guessing==0 ) {

    braceStartsValue_ = false;
    pbf_->selector()->pop();

  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mRBRACE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = RBRACE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('}' /* charlit */ );

  if ( inputState->guessing==0 ) {

    braceStartsValue_ = false;
    pbf_->selector()->pop();

  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mCOMA(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = COMA;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match(',' /* charlit */ );

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mEQUAL(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = EQUAL;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('=' /* charlit */ );

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mNAME(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = NAME;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    matchRange('a','z');
  }
  {
    // ( ... )*
    for (;;) {
      switch ( LA(1)) {
      case 0x61 /* 'a' */ :
      case 0x62 /* 'b' */ :
      case 0x63 /* 'c' */ :
      case 0x64 /* 'd' */ :
      case 0x65 /* 'e' */ :
      case 0x66 /* 'f' */ :
      case 0x67 /* 'g' */ :
      case 0x68 /* 'h' */ :
      case 0x69 /* 'i' */ :
      case 0x6a /* 'j' */ :
      case 0x6b /* 'k' */ :
      case 0x6c /* 'l' */ :
      case 0x6d /* 'm' */ :
      case 0x6e /* 'n' */ :
      case 0x6f /* 'o' */ :
      case 0x70 /* 'p' */ :
      case 0x71 /* 'q' */ :
      case 0x72 /* 'r' */ :
      case 0x73 /* 's' */ :
      case 0x74 /* 't' */ :
      case 0x75 /* 'u' */ :
      case 0x76 /* 'v' */ :
      case 0x77 /* 'w' */ :
      case 0x78 /* 'x' */ :
      case 0x79 /* 'y' */ :
      case 0x7a /* 'z' */ : {
        matchRange('a','z');
        break;
      }

      case 0x30 /* '0' */ :
      case 0x31 /* '1' */ :
      case 0x32 /* '2' */ :
      case 0x33 /* '3' */ :
      case 0x34 /* '4' */ :
      case 0x35 /* '5' */ :
      case 0x36 /* '6' */ :
      case 0x37 /* '7' */ :
      case 0x38 /* '8' */ :
      case 0x39 /* '9' */ : {
        matchRange('0','9');
        break;
      }

      case 0x5f /* '_' */ : {
        match('_' /* charlit */ );
        break;
      }

      case 0x2d /* '-' */ : {
        match('-' /* charlit */ );
        break;
      }

      case 0x27 /* '\'' */ : {
        match('\'' /* charlit */ );
        break;
      }

      case 0x3a /* ':' */ : {
        match(':' /* charlit */ );
        break;
      }

      case 0x2e /* '.' */ : {
        match('.' /* charlit */ );
        break;
      }

      case 0x2f /* '/' */ : {
        match('/' /* charlit */ );
        break;
      }

      case 0x2b /* '+' */ : {
        match('+' /* charlit */ );
        break;
      }

      default: {
        goto _loop35;
      }
      }
    }

_loop35:
    ;
  } // ( ... )*

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mNUMBER(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = NUMBER;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    // ( ... )+
    int _cnt38=0;

    for (;;) {
      if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
        matchRange('0','9');
      }
      else {
        if ( _cnt38>=1 ) {
          goto _loop38;
        }
        else {
          throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
        }
      }

      _cnt38++;
    }

_loop38:
    ;
  }  // ( ... )+

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mESC(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = ESC;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('\\' /* charlit */ );
  {
    if ((_tokenSet_0.member(LA(1)))) {
      {
        match(_tokenSet_0);
      }
    }
    else if ((LA(1) == 0x22 /* '\"' */ )) {
      match('\"' /* charlit */ );

      if ( inputState->guessing==0 ) {

        const char* msg0 =
          "BibTeX compliance does not allow escaped double quotes (\\\") "
          "in double quoted field values (field = \"a value\"). Use braces"
          " instead (field = {a \" in value} or field=\" a {\"} in value\")";
        const char* msg1 =
          "Strict BibTeX would not accept escaped double quotes in "
          "double quoted field values";
        int l = getLine();
        int c = getColumn();

        switch (pbf_->quoteCompliance()) {
        case Strict :
          throw xdkbib::parsing_error(getFilename(),msg0,l,c);
          break;

        case Warning:
          pbf_->issueWarning(msg1,getFilename(),l);
          break;
        }

      }
    }
    else {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
    }

  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mESC_WITH_QUOTE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = ESC_WITH_QUOTE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('\\' /* charlit */ );
  {
    if ((_tokenSet_0.member(LA(1)))) {
      {
        match(_tokenSet_0);
      }
    }
    else if ((LA(1) == 0x22 /* '\"' */ )) {
      match('\"' /* charlit */ );
    }
    else {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
    }

  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mSTRING_INTERNAL_WITH_QUOTES(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = STRING_INTERNAL_WITH_QUOTES;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    // ( ... )*
    for (;;) {
      bool synPredMatched49 = false;

      if (((LA(1) == 0x5c /* '\\' */ ))) {
        int _m49 = mark();
        synPredMatched49 = true;
        inputState->guessing++;

        try {
          {
            match('\\' /* charlit */ );
            {
              match(_tokenSet_1);
            }
          }
        }
        catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
          synPredMatched49 = false;
        }

        rewind(_m49);
        inputState->guessing--;
      }

      if ( synPredMatched49 ) {
        mESC_WITH_QUOTE(false);
      }
      else if ((LA(1) == 0xa /* '\n' */  || LA(1) == 0xd /* '\r' */  || LA(1) == 0x5c /* '\\' */ )) {
        {
          switch ( LA(1)) {
          case 0xd /* '\r' */ : {
            match('\r' /* charlit */ );

            if ( inputState->guessing==0 ) {
              newline();
            }

            break;
          }

          case 0xa /* '\n' */ : {
            match('\n' /* charlit */ );

            if ( inputState->guessing==0 ) {
              newline();
            }

            break;
          }

          case 0x5c /* '\\' */ : {
            match('\\' /* charlit */ );
            match('\n' /* charlit */ );

            if ( inputState->guessing==0 ) {
              newline();
            }

            break;
          }

          default: {
            throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
          }
          }
        }
      }
      else if ((_tokenSet_2.member(LA(1)))) {
        {
          match(_tokenSet_2);
        }
      }
      else if ((LA(1) == 0x7b /* '{' */ )) {
        match('{' /* charlit */ );
        mSTRING_INTERNAL_WITH_QUOTES(false);
        match('}' /* charlit */ );
      }
      else {
        goto _loop52;
      }

    }

_loop52:
    ;
  } // ( ... )*

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mSTRING_INTERNAL(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = STRING_INTERNAL;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
  ANTLR_USE_NAMESPACE(antlr)RefToken t;

  {
    // ( ... )*
    for (;;) {
      bool synPredMatched57 = false;

      if (((LA(1) == 0x5c /* '\\' */ ))) {
        int _m57 = mark();
        synPredMatched57 = true;
        inputState->guessing++;

        try {
          {
            match('\\' /* charlit */ );
            {
              match(_tokenSet_1);
            }
          }
        }
        catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
          synPredMatched57 = false;
        }

        rewind(_m57);
        inputState->guessing--;
      }

      if ( synPredMatched57 ) {
        mESC(false);
      }
      else if ((LA(1) == 0xa /* '\n' */  || LA(1) == 0xd /* '\r' */  || LA(1) == 0x5c /* '\\' */ )) {
        {
          switch ( LA(1)) {
          case 0xd /* '\r' */ : {
            match('\r' /* charlit */ );

            if ( inputState->guessing==0 ) {
              newline();
            }

            break;
          }

          case 0xa /* '\n' */ : {
            match('\n' /* charlit */ );

            if ( inputState->guessing==0 ) {
              newline();
            }

            break;
          }

          case 0x5c /* '\\' */ : {
            match('\\' /* charlit */ );
            match('\n' /* charlit */ );

            if ( inputState->guessing==0 ) {
              newline();
            }

            break;
          }

          default: {
            throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
          }
          }
        }
      }
      else if ((_tokenSet_3.member(LA(1)))) {
        {
          match(_tokenSet_3);
        }
      }
      else if ((LA(1) == 0x7b /* '{' */ )) {
        match('{' /* charlit */ );
        mSTRING_INTERNAL_WITH_QUOTES(false);
        match('}' /* charlit */ );
      }
      else {
        goto t;
      }

    }

t:
    ;
  } // ( ... )*

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mSTRING(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = STRING;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
  ANTLR_USE_NAMESPACE(antlr)RefToken t;

  _saveIndex = text.length();
  match('\"' /* charlit */ );
  text.erase(_saveIndex);
  mSTRING_INTERNAL(true);
  t=_returnToken;
  _saveIndex = text.length();
  match('\"' /* charlit */ );
  text.erase(_saveIndex);

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mLBRACE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = LBRACE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('{' /* charlit */ );

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mVALUE_INTERNAL(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = VALUE_INTERNAL;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    // ( ... )*
    for (;;) {
      switch ( LA(1)) {
      case 0xa /* '\n' */ : {
        match('\n' /* charlit */ );

        if ( inputState->guessing==0 ) {
          newline();
        }

        break;
      }

      case 0x7b /* '{' */ : {
        mVALUE(false);
        break;
      }

      default:
        if ((_tokenSet_4.member(LA(1)))) {
          {
            {
              match(_tokenSet_4);
            }
          }
        }
        else {
          goto _loop67;
        }
      }
    }

_loop67:
    ;
  } // ( ... )*

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mVALUE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = VALUE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    match('{' /* charlit */ );
    mVALUE_INTERNAL(false);
    match('}' /* charlit */ );
  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mLBRACE_OR_VALUE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = LBRACE_OR_VALUE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
  ANTLR_USE_NAMESPACE(antlr)RefToken t;

  if (((LA(1) == 0x7b /* '{' */ ))&&( braceStartsValue_ )) {
    mVALUE(true);
    t=_returnToken;

    if ( inputState->guessing==0 ) {

      _ttype = VALUE;
      string s = t->getText().substr(1,t->getText().size()-2);
      {
        text.erase(_begin);
        text += s;
      };

    }
  }
  else if ((LA(1) == 0x7b /* '{' */ )) {
    mLBRACE(false);

    if ( inputState->guessing==0 ) {

      _ttype = LBRACE;

    }
  }
  else {
    throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mWS(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = WS;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    switch ( LA(1)) {
    case 0x20 /* ' ' */ : {
      match(' ' /* charlit */ );
      break;
    }

    case 0x9 /* '\t' */ : {
      match('\t' /* charlit */ );
      break;
    }

    case 0xa /* '\n' */ :
    case 0xd /* '\r' */ : {
      {
        switch ( LA(1)) {
        case 0xa /* '\n' */ : {
          match('\n' /* charlit */ );
          break;
        }

        case 0xd /* '\r' */ : {
          match("\r\n");
          break;
        }

        default: {
          throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
        }
        }
      }

      if ( inputState->guessing==0 ) {
        newline();
      }

      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
    }
    }
  }

  if ( inputState->guessing==0 ) {

    _ttype = ANTLR_USE_NAMESPACE(antlr)Token::SKIP;

  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mTYPE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = TYPE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  _saveIndex = text.length();
  match('@' /* charlit */ );
  text.erase(_saveIndex);
  {
    // ( ... )+
    int _cnt76=0;

    for (;;) {
      if (((LA(1) >= 0x61 /* 'a' */  && LA(1) <= 0x7a /* 'z' */ ))) {
        matchRange('a','z');
      }
      else {
        if ( _cnt76>=1 ) {
          goto _loop76;
        }
        else {
          throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
        }
      }

      _cnt76++;
    }

_loop76:
    ;
  }  // ( ... )+

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mSTRINGDEF(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = STRINGDEF;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match("@string");

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mPREAMBLE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = PREAMBLE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match("@preamble");

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibcommandlexer::mSTRINGDEF_OR_PREAMBLE_OR_TYPE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = STRINGDEF_OR_PREAMBLE_OR_TYPE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
  ANTLR_USE_NAMESPACE(antlr)RefToken t;

  bool synPredMatched81 = false;

  if (((LA(1) == 0x40 /* '@' */ ))) {
    int _m81 = mark();
    synPredMatched81 = true;
    inputState->guessing++;

    try {
      {
        match("@string");
      }
    }
    catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
      synPredMatched81 = false;
    }

    rewind(_m81);
    inputState->guessing--;
  }

  if ( synPredMatched81 ) {
    mSTRINGDEF(false);

    if ( inputState->guessing==0 ) {

      _ttype = STRINGDEF;

    }
  }
  else {
    bool synPredMatched83 = false;

    if (((LA(1) == 0x40 /* '@' */ ))) {
      int _m83 = mark();
      synPredMatched83 = true;
      inputState->guessing++;

      try {
        {
          match("@preamble");
        }
      }
      catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
        synPredMatched83 = false;
      }

      rewind(_m83);
      inputState->guessing--;
    }

    if ( synPredMatched83 ) {
      mPREAMBLE(false);

      if ( inputState->guessing==0 ) {

        _ttype = PREAMBLE;

      }
    }
    else {
      bool synPredMatched85 = false;

      if (((LA(1) == 0x40 /* '@' */ ))) {
        int _m85 = mark();
        synPredMatched85 = true;
        inputState->guessing++;

        try {
          {
            match('@' /* charlit */ );
          }
        }
        catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
          synPredMatched85 = false;
        }

        rewind(_m85);
        inputState->guessing--;
      }

      if ( synPredMatched85 ) {
        mTYPE(true);
        t=_returnToken;

        if ( inputState->guessing==0 ) {

          _ttype = TYPE;

        }
      }
      else {
        throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
      }
    }
  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}


const unsigned long bibcommandlexer::_tokenSet_0_data_[] = { 4294966264UL, 4294967291UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xd 0xe 0xf 0x10 0x11 0x12 0x13
// 0x14 0x15 0x16 0x17 0x18 0x19
const ANTLR_USE_NAMESPACE(antlr)BitSet bibcommandlexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long bibcommandlexer::_tokenSet_1_data_[] = { 4294966264UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xd 0xe 0xf 0x10 0x11 0x12 0x13
// 0x14 0x15 0x16 0x17 0x18 0x19
const ANTLR_USE_NAMESPACE(antlr)BitSet bibcommandlexer::_tokenSet_1(_tokenSet_1_data_,16);
const unsigned long bibcommandlexer::_tokenSet_2_data_[] = { 4294958072UL, 4294967295UL, 4026531839UL, 3623878655UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xe 0xf 0x10 0x11 0x12 0x13 0x14
// 0x15 0x16 0x17 0x18 0x19
const ANTLR_USE_NAMESPACE(antlr)BitSet bibcommandlexer::_tokenSet_2(_tokenSet_2_data_,16);
const unsigned long bibcommandlexer::_tokenSet_3_data_[] = { 4294958072UL, 4294967291UL, 4026531839UL, 3623878655UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xe 0xf 0x10 0x11 0x12 0x13 0x14
// 0x15 0x16 0x17 0x18 0x19
const ANTLR_USE_NAMESPACE(antlr)BitSet bibcommandlexer::_tokenSet_3(_tokenSet_3_data_,16);
const unsigned long bibcommandlexer::_tokenSet_4_data_[] = { 4294966264UL, 4294967295UL, 4294967295UL, 3623878655UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xd 0xe 0xf 0x10 0x11 0x12 0x13
// 0x14 0x15 0x16 0x17 0x18 0x19
const ANTLR_USE_NAMESPACE(antlr)BitSet bibcommandlexer::_tokenSet_4(_tokenSet_4_data_,16);

