/* $ANTLR 2.7.6 (20060903): "value.g" -> "valuelexer.cpp"$ */
#include "valuelexer.hpp"
#include <antlr/CharBuffer.hpp>
#include <antlr/TokenStreamException.hpp>
#include <antlr/TokenStreamIOException.hpp>
#include <antlr/TokenStreamRecognitionException.hpp>
#include <antlr/CharStreamException.hpp>
#include <antlr/CharStreamIOException.hpp>
#include <antlr/NoViableAltForCharException.hpp>

#line 173 "value.g"

// Will be included before lexer methods implementations
// in lexer.cpp
#include <iostream>

using namespace std;

#line 20 "valuelexer.cpp"
valuelexer::valuelexer(ANTLR_USE_NAMESPACE(std)istream& in)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in),false) {
  initLiterals();
}

valuelexer::valuelexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(ib,false) {
  initLiterals();
}

valuelexer::valuelexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(state,false) {
  initLiterals();
}

void valuelexer::initLiterals() {
}

ANTLR_USE_NAMESPACE(antlr)RefToken valuelexer::nextToken() {
  ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;

  for (;;) {
    ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
    int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
    resetText();

    try {   // for lexical and char stream error handling
      switch ( LA(1)) {
      case 0x7b /* '{' */ : {
        mLBRACE(true);
        theRetToken=_returnToken;
        break;
      }

      case 0x7d /* '}' */ : {
        mRBRACE(true);
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

      default:
        if ((LA(1) == 0x5c /* '\\' */ ) && (_tokenSet_0.member(LA(2)))) {
          mTOKEN(true);
          theRetToken=_returnToken;
        }
        else if ((_tokenSet_0.member(LA(1))) && (true)) {
          mLETTER(true);
          theRetToken=_returnToken;
        }
        else {
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

void valuelexer::mLBRACE(bool _createToken) {
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

void valuelexer::mRBRACE(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = RBRACE;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('}' /* charlit */ );

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void valuelexer::mTOKEN(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = TOKEN;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  _saveIndex = text.length();
  match('\\' /* charlit */ );
  text.erase(_saveIndex);
  {
    if ((_tokenSet_1.member(LA(1)))) {
      {
        // ( ... )+
        int _cnt17=0;

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

          default: {
            if ( _cnt17>=1 ) {
              goto _loop17;
            }
            else {
              throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
            }
          }
          }

          _cnt17++;
        }

_loop17:
        ;
      }  // ( ... )+
    }
    else if ((_tokenSet_2.member(LA(1)))) {
      {
        match(_tokenSet_2);
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

void valuelexer::mLETTER(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = LETTER;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    match(_tokenSet_0);
  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void valuelexer::mWS(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = WS;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    // ( ... )+
    int _cnt24=0;

    for (;;) {
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
#line 202 "value.g"
        newline();
#line 284 "valuelexer.cpp"
        break;
      }

      default: {
        if ( _cnt24>=1 ) {
          goto _loop24;
        }
        else {
          throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
        }
      }
      }

      _cnt24++;
    }

_loop24:
    ;
  }  // ( ... )+

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}


const unsigned long valuelexer::_tokenSet_0_data_[] = { 4294957560UL, 4294967294UL, 4294967295UL, 3623878655UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8
const ANTLR_USE_NAMESPACE(antlr)BitSet valuelexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long valuelexer::_tokenSet_1_data_[] = { 0UL, 67043328UL, 0UL, 134217726UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet valuelexer::_tokenSet_1(_tokenSet_1_data_,10);
const unsigned long valuelexer::_tokenSet_2_data_[] = { 4294957560UL, 4227923966UL, 4294967295UL, 3489660929UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8
const ANTLR_USE_NAMESPACE(antlr)BitSet valuelexer::_tokenSet_2(_tokenSet_2_data_,16);

