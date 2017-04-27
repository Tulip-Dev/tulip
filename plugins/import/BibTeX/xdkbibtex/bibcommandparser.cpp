/* $ANTLR 2.7.6 (20060903): "bibcommand.g" -> "bibcommandparser.cpp"$ */
#include "bibcommandparser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>

// Will be included before parser methods implementations
// in parser.cpp
#include "bibcommandlexer.hpp"
#include <sstream>
#include <iostream>

using namespace std;
using namespace xdkbib;

void bibcommandparser::setBraceStartsValue(bool b) {
  bibcommandlexer* l =
    dynamic_cast<bibcommandlexer*>(pbf_->selector()->getCurrentStream());

  if (l) {
    l->setBraceStartsValue(b);
  }
  else {
    cout<<"cannot cast to bibcommandlexer"<<endl;
  }
}
void bibcommandparser::addPart(ValuePartType t,const string& v) {
  currentValueParts_.push_back(ValuePart(t,v));
}

bibcommandparser::bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k) {
}

bibcommandparser::bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,2) {
}

bibcommandparser::bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k) {
}

bibcommandparser::bibcommandparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,2) {
}

bibcommandparser::bibcommandparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
  : ANTLR_USE_NAMESPACE(antlr)LLkParser(state,2) {
}

void bibcommandparser::parse() {

  string i;
  string s;



  setBraceStartsValue(false);

  {
    switch ( LA(1)) {
    case TYPE: {
      entry();
      break;
    }

    case STRINGDEF: {
      stringdef();
      break;
    }

    case PREAMBLE: {
      preamble();
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }
}

void bibcommandparser::entry() {
  ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;

  string k;


  t = LT(1);
  match(TYPE);
  {
    switch ( LA(1)) {
    case LBRACE: {
      {
        match(LBRACE);

        // allow a key of any type
        // so catch MisMatchedTokenException when looking for a comma
        // in case of a key beginning by a number of containing slashes
        while(true) {
          try {
            k +=key();
            match(COMA);
            break;
          }
          catch (const ANTLR_USE_NAMESPACE(antlr)MismatchedTokenException& e) {
          }
        }

        int l = t->getLine();
#ifdef PARSER_DEBUG_MODE
        cout<<"PARSER_DEBUG:parsing entry "<<k<<"["<<l<<"]"<<endl;
#endif
        setBraceStartsValue(true);
        currentEntry_ = pbf_->file()->addEntry(Entry(t->getText(),
                                               k,l));

        fields();
        match(RBRACE);
      }
      break;
    }

    case LPARENTHESIS: {
      {
        match(LPARENTHESIS);
        k=key();
        match(COMA);

        int l = t->getLine();
        setBraceStartsValue(true);
        currentEntry_ = pbf_->file()->addEntry(Entry(t->getText(),
                                               k,l));

        fields();
        match(RPARENTHESIS);
      }
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }

  currentEntry_->setComment(pbf_->currentComment());
  pbf_->clearCurrentComment();

}

void bibcommandparser::stringdef() {

  string i;

  match(STRINGDEF);

  setBraceStartsValue(false);

  {
    switch ( LA(1)) {
    case LPARENTHESIS: {
      {
        match(LPARENTHESIS);
        i=id();
        match(EQUAL);
        fieldValue();
        match(RPARENTHESIS);

        pbf_->file()->defineString(i,
                                   currentValueParts_.begin(),
                                   currentValueParts_.end());

      }
      break;
    }

    case LBRACE: {
      {
        match(LBRACE);
        i=id();
        match(EQUAL);
        fieldValue();
        match(RBRACE);

        pbf_->file()->defineString(i,
                                   currentValueParts_.begin(),
                                   currentValueParts_.end());

      }
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }
}

void bibcommandparser::preamble() {

  match(PREAMBLE);

  setBraceStartsValue(true);

  {
    switch ( LA(1)) {
    case LBRACE: {
      {
        match(LBRACE);
        fieldValue();
        match(RBRACE);

        bool newline = true;

        for (list<ValuePart>::const_iterator
             iter = currentValueParts_.begin();
             iter != currentValueParts_.end(); ++iter) {
          pbf_->file()->addToPreamble(*iter,newline);
          newline = false;
        }

      }
      break;
    }

    case LPARENTHESIS: {
      {
        match(LPARENTHESIS);
        fieldValue();
        match(RPARENTHESIS);

        bool newline = true;

        for (list<ValuePart>::const_iterator
             iter = currentValueParts_.begin();
             iter != currentValueParts_.end(); ++iter) {
          pbf_->file()->addToPreamble(*iter,newline);
          newline = false;
        }

      }
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }
}

std::string  bibcommandparser::id() {
  std::string v;
  ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;

  t = LT(1);
  match(NAME);

  v = string(t->getText());

  return v;
}

void bibcommandparser::fieldValue() {


  currentValueParts_.clear();

  fieldValuePart();
  {
    // ( ... )*
    for (;;) {
      if ((LA(1) == POUND)) {
        match(POUND);
        fieldValuePart();
      }
      else {
        goto _loop22;
      }

    }

_loop22:
    ;
  } // ( ... )*
}

std::string  bibcommandparser::key() {
  std::string v;
  ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
  ANTLR_USE_NAMESPACE(antlr)RefToken  s = ANTLR_USE_NAMESPACE(antlr)nullToken;

  switch ( LA(1)) {
  case NAME: {
    t = LT(1);
    match(NAME);

    v = string(t->getText());

    break;
  }

  case NUMBER: {
    s = LT(1);
    match(NUMBER);

    v = string(s->getText());

    break;
  }

  default: {
    throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
  }
  }

  return v;
}

void bibcommandparser::fields() {

  field();
  {
    // ( ... )*
    for (;;) {
      if ((LA(1) == COMA) && (LA(2) == NAME)) {
        match(COMA);
        field();
      }
      else {
        goto _loop17;
      }

    }

_loop17:
    ;
  } // ( ... )*
  {
    switch ( LA(1)) {
    case COMA: {
      match(COMA);
      break;
    }

    case RPARENTHESIS:
    case RBRACE: {
      break;
    }

    default: {
      throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
    }
    }
  }
}

void bibcommandparser::field() {
  ANTLR_USE_NAMESPACE(antlr)RefToken  e = ANTLR_USE_NAMESPACE(antlr)nullToken;

  string i;


  i=id();
  e = LT(1);
  match(EQUAL);
  fieldValue();

#ifdef PARSER_DEBUG_MODE
  cout<<"PARSER_DEBUG:  parsing field "<<i<<"["<<l<<"]"<<endl;
#endif
  int l = e->getLine();
  FieldHandle h = currentEntry_->field(i);

  if (h.isMissing()) {
    currentEntry_->addField(i,l).append(currentValueParts_.begin(),currentValueParts_.end());
  }
  else {
    int l = e->getLine();
    ostringstream os;
    os<<"multiple field "<<i<<" in "<<currentEntry_->key()
      <<" -> extra ignored";
    pbf_->issueWarning(os.str(),getFilename(),l);
  }

}

void bibcommandparser::fieldValuePart() {
  ANTLR_USE_NAMESPACE(antlr)RefToken  t0 = ANTLR_USE_NAMESPACE(antlr)nullToken;
  ANTLR_USE_NAMESPACE(antlr)RefToken  t1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
  ANTLR_USE_NAMESPACE(antlr)RefToken  t2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
  ANTLR_USE_NAMESPACE(antlr)RefToken  t3 = ANTLR_USE_NAMESPACE(antlr)nullToken;

  switch ( LA(1)) {
  case NAME: {
    t0 = LT(1);
    match(NAME);

    addPart(String,t0->getText());

    // Issue a warning if the name is not defined although it
    // does not prevent us from creating a value part.
    try {
      (void) pbf_->file()->stringText(t0->getText());
    }
    catch (range_error& e) {
      int l = t0->getLine();
      ostringstream os;
      os<<"string name \""<<t0->getText()<<"\" is undefined";
      pbf_->issueWarning(os.str(),getFilename(),l);
      addPart(String,t0->getText());
    }

    break;
  }

  case STRING: {
    t1 = LT(1);
    match(STRING);
    addPart(Quoted,t1->getText());
    break;
  }

  case VALUE: {
    t2 = LT(1);
    match(VALUE);
    addPart(Braced,t2->getText());
    break;
  }

  case NUMBER: {
    t3 = LT(1);
    match(NUMBER);
    addPart(Number,t3->getText());
    break;
  }

  default: {
    throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
  }
  }
}

void bibcommandparser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& ) {
}
const char* bibcommandparser::tokenNames[] = {
  "<0>",
  "EOF",
  "<2>",
  "NULL_TREE_LOOKAHEAD",
  "STRINGDEF",
  "LPARENTHESIS",
  "EQUAL",
  "RPARENTHESIS",
  "LBRACE",
  "RBRACE",
  "PREAMBLE",
  "TYPE",
  "COMA",
  "POUND",
  "NAME",
  "STRING",
  "VALUE",
  "NUMBER",
  "ESC",
  "ESC_WITH_QUOTE",
  "STRING_INTERNAL_WITH_QUOTES",
  "STRING_INTERNAL",
  "VALUE_INTERNAL",
  "LBRACE_OR_VALUE",
  "WS",
  "STRINGDEF_OR_PREAMBLE_OR_TYPE",
  0
};



