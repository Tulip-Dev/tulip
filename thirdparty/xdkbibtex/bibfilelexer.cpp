/* $ANTLR 2.7.6 (20060903): "bibfile.g" -> "bibfilelexer.cpp"$ */
#include "bibfilelexer.hpp"
#include <antlr/CharBuffer.hpp>
#include <antlr/TokenStreamException.hpp>
#include <antlr/TokenStreamIOException.hpp>
#include <antlr/TokenStreamRecognitionException.hpp>
#include <antlr/CharStreamException.hpp>
#include <antlr/CharStreamIOException.hpp>
#include <antlr/NoViableAltForCharException.hpp>

#line 89 "bibfile.g"

// Will be included before lexer methods implementations
// in lexer.cpp
#include <iostream>

using namespace std;

#line 20 "bibfilelexer.cpp"
bibfilelexer::bibfilelexer(ANTLR_USE_NAMESPACE(std)istream& in)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in),false) {
  initLiterals();
}

bibfilelexer::bibfilelexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(ib,false) {
  initLiterals();
}

bibfilelexer::bibfilelexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
  : ANTLR_USE_NAMESPACE(antlr)CharScanner(state,false) {
  initLiterals();
}

void bibfilelexer::initLiterals() {
}

ANTLR_USE_NAMESPACE(antlr)RefToken bibfilelexer::nextToken() {
  ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;

  for (;;) {
    ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
    int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
    resetText();

    try {   // for lexical and char stream error handling
      if ((_tokenSet_0.member(LA(1)))) {
        mCOMMENT(true);
        theRetToken=_returnToken;
      }
      else if ((LA(1) == 0x40 /* '@' */ )) {
        mARROBAS_OR_COMMENT(true);
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

void bibfilelexer::mCOMMENT(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = COMMENT;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  {
    {
      // ( ... )+
      int _cnt10=0;

      for (;;) {
        if ((_tokenSet_1.member(LA(1)))) {
          {
            match(_tokenSet_1);
          }
        }
        else if ((LA(1) == 0xa /* '\n' */ )) {
          match('\n' /* charlit */ );

          if ( inputState->guessing==0 ) {
#line 114 "bibfile.g"
            newline();
#line 108 "bibfilelexer.cpp"
          }
        }
        else {
          if ( _cnt10>=1 ) {
            goto _loop10;
          }
          else {
            throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
          }
        }

        _cnt10++;
      }

_loop10:
      ;
    }  // ( ... )+
  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibfilelexer::mARROBAS(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = ARROBAS;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match('@' /* charlit */ );

  if ( inputState->guessing==0 ) {
#line 118 "bibfile.g"

    pbf_->selector()->push("cmnd");

#line 139 "bibfilelexer.cpp"
  }

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibfilelexer::mARROBASCOMMENT(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = ARROBASCOMMENT;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;

  match("@comment");

  if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
    _token = makeToken(_ttype);
    _token->setText(text.substr(_begin, text.length()-_begin));
  }

  _returnToken = _token;
  _saveIndex=0;
}

void bibfilelexer::mARROBAS_OR_COMMENT(bool _createToken) {
  int _ttype;
  ANTLR_USE_NAMESPACE(antlr)RefToken _token;
  ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
  _ttype = ARROBAS_OR_COMMENT;
  ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
#line 125 "bibfile.g"

  int m = mark();

#line 171 "bibfilelexer.cpp"

  bool synPredMatched15 = false;

  if (((LA(1) == 0x40 /* '@' */ ))) {
    int _m15 = mark();
    synPredMatched15 = true;
    inputState->guessing++;

    try {
      {
        match("@comment");
      }
    }
    catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
      synPredMatched15 = false;
    }

    rewind(_m15);
    inputState->guessing--;
  }

  if ( synPredMatched15 ) {
    mARROBASCOMMENT(false);

    if ( inputState->guessing==0 ) {
#line 130 "bibfile.g"

      commit();
      _ttype = COMMENT;

#line 197 "bibfilelexer.cpp"
    }
  }
  else {
    bool synPredMatched17 = false;

    if (((LA(1) == 0x40 /* '@' */ ))) {
      int _m17 = mark();
      synPredMatched17 = true;
      inputState->guessing++;

      try {
        {
          match('@' /* charlit */ );
        }
      }
      catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
        synPredMatched17 = false;
      }

      rewind(_m17);
      inputState->guessing--;
    }

    if ( synPredMatched17 ) {
      mARROBAS(false);

      if ( inputState->guessing==0 ) {
#line 135 "bibfile.g"

        rewind(m);
        _ttype = ARROBAS;

#line 225 "bibfilelexer.cpp"
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


const unsigned long bibfilelexer::_tokenSet_0_data_[] = { 4294967288UL, 4294967295UL, 4294967294UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7
const ANTLR_USE_NAMESPACE(antlr)BitSet bibfilelexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long bibfilelexer::_tokenSet_1_data_[] = { 4294966264UL, 4294967295UL, 4294967294UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7
const ANTLR_USE_NAMESPACE(antlr)BitSet bibfilelexer::_tokenSet_1(_tokenSet_1_data_,16);

