// Copyright (C) 2004 Xavier Decoret <Xavier.Decoret@imag.fr>

// This program is free software; you can redistribute it and/or 
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation; either 
// version 2 of the License, or (at your option) any later 
// version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

// Things that gets included in Lexer.hpp and Parser.hpp
header {
    // Will be included before the class definitions
    // in parser.hpp and lexer.hpp
    #include "parsedbibfile.h"
    #include "base.h"
    #include <iostream>
    #include <list>

    class bibcommandparser;
}		
options {
    language="Cpp";
    genHashLines = true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
//%%%%%%        PARSER                              %%%%%%%%%%    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

{
    // Will be included before parser methods implementations
    // in parser.cpp
    #include "bibcommandlexer.hpp"
    #include <sstream>
    #include <iostream>
    
    using namespace std;
    using namespace xdkbib;

    void bibcommandparser::setBraceStartsValue(bool b)
        {
            bibcommandlexer* l =
                dynamic_cast<bibcommandlexer*>(pbf_->selector()->getCurrentStream());
            if (l)
                {
                    l->setBraceStartsValue(b);
                }
            else
                {
                    cout<<"cannot cast to bibcommandlexer"<<endl;
                }
        }
    void bibcommandparser::addPart(ValuePartType t,const string& v)
        {
            currentValueParts_.push_back(ValuePart(t,v));
        }
}
class bibcommandparser extends Parser;
options {
    k=2;
    buildAST = false;
    defaultErrorHandler=false;
}
{
    // Will be included before parser methods declarations
    // *inside* class definition in parser.hpp
    private:
      ParsedBibFile* pbf_;
      xdkbib::Entry* currentEntry_;
      std::list<xdkbib::ValuePart> currentValueParts_;
    public:
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
    protected:
      void setBraceStartsValue(bool b);
      void addPart(xdkbib::ValuePartType t,const std::string& v);
}
parse
{
    string i;
    string s;
}
    :
        {
            setBraceStartsValue(false);
        }
        (entry | stringdef | preamble)
    ;
stringdef
{
    string i;
}
    : STRINGDEF
        {
            setBraceStartsValue(true);
        }
        ((LPARENTHESIS i=id EQUAL fieldValue RPARENTHESIS
                {
                    pbf_->file()->defineString(i,
                                               currentValueParts_.begin(),
                                               currentValueParts_.end());
                }
            )
        |(LBRACE i=id EQUAL fieldValue RBRACE
                {
                    pbf_->file()->defineString(i,
                                               currentValueParts_.begin(),
                                               currentValueParts_.end());
                }
            )
        )
    ;
preamble
    : PREAMBLE
        {
            setBraceStartsValue(true);
        }
        ((LBRACE fieldValue RBRACE
                {
                    bool newline = true;
                    for (list<ValuePart>::const_iterator
                             iter = currentValueParts_.begin();
                             iter != currentValueParts_.end();++iter)
                        {
                            pbf_->file()->addToPreamble(*iter,newline);
                            newline = false;
                        }
                }
            )
        |(LPARENTHESIS fieldValue RPARENTHESIS
                {
                    bool newline = true;
                    for (list<ValuePart>::const_iterator
                             iter = currentValueParts_.begin();
                             iter != currentValueParts_.end();++iter)
                        {
                            pbf_->file()->addToPreamble(*iter,newline);
                            newline = false;
                        }
                }
            )
        )
    ;
entry
{
    string k;
}
    : t:TYPE
        ((LBRACE k=key COMA
                {
                    int l = t->getLine();
#ifdef PARSER_DEBUG_MODE
                    cout<<"PARSER_DEBUG:parsing entry "<<k<<"["<<l<<"]"<<endl;
#endif
                    setBraceStartsValue(true);
                    currentEntry_ = pbf_->file()->addEntry(Entry(t->getText(),
                                                                 k,l));
                }
                fields
                RBRACE)
        |(LPARENTHESIS k=key COMA
                {
                    int l = t->getLine();
                    setBraceStartsValue(true);
                    currentEntry_ = pbf_->file()->addEntry(Entry(t->getText(),
                                                                 k,l));
                }
                fields
                RPARENTHESIS)
        )
        {
            currentEntry_->setComment(pbf_->currentComment());
            pbf_->clearCurrentComment();
        }
    ;
fields
    : field (COMA field)* (COMA)? 
    ;
field
{
    string i;
}
    : i=id e:EQUAL fieldValue
        {
#ifdef PARSER_DEBUG_MODE
            cout<<"PARSER_DEBUG:  parsing field "<<i<<"["<<l<<"]"<<endl;
#endif
            int l = e->getLine();
            FieldHandle h = currentEntry_->field(i);
            if (h.isMissing())
            {
                currentEntry_->addField(i,l).append(currentValueParts_.begin(),currentValueParts_.end());
            }
            else
            {
                int l = e->getLine();
                ostringstream os;
                os<<"multiple field "<<i<<" in "<<currentEntry_->key()
                  <<" -> extra ignored";
                pbf_->issueWarning(os.str(),getFilename(),l);
            }            
        }
    ;
fieldValue 
    :
        {
            currentValueParts_.clear();
        }
        fieldValuePart (POUND fieldValuePart)*
    ;
fieldValuePart 
    : t0:NAME
        {
            addPart(String,t0->getText());
            // Issue a warning if the name is not defined although it
            // does not prevent us from creating a value part.
            try {
                (void) pbf_->file()->stringText(t0->getText());
            }
            catch (range_error& e)
                {
                    int l = t0->getLine();
                    ostringstream os;
                    os<<"string name \""<<t0->getText()<<"\" is undefined";
                    pbf_->issueWarning(os.str(),getFilename(),l);
                    addPart(String,t0->getText());
                }
        }
    | t1:STRING { addPart(Quoted,t1->getText()); }
    | t2:VALUE  { addPart(Braced,t2->getText()); }      
    | t3:NUMBER { addPart(Number,t3->getText()); }      
    ;
key returns [std::string v]
    : t:NAME
        {
            v = string(t->getText());
        }
    | s:NUMBER
        {
            v = string(s->getText());
        }
    ;
id returns [std::string v]
    : t:NAME
        {
            v = string(t->getText());
        }
    ;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
//%%%%%%        LEXER                               %%%%%%%%%%    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
{
    // Will be included before lexer methods implementations
    // in lexer.cpp
    #include <sstream>
    #include <iostream>
    
    using namespace std;
}
class bibcommandlexer extends Lexer;
options {
    k=1;
    defaultErrorHandler=false;
    caseSensitive=false;
    charVocabulary='\3'..'\377';
}
{
    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
    private:
      ParsedBibFile* pbf_;
      bool           braceStartsValue_;
    public:
      inline void setBraceStartsValue(bool b)
          {
              braceStartsValue_ = b;
          }
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
}
POUND : '#'
    ;
LPARENTHESIS : '('
    ;
RPARENTHESIS : ')'
        {
            braceStartsValue_ = false;
            pbf_->selector()->pop();
        }
    ;
RBRACE : '}'
        {
            braceStartsValue_ = false;
            pbf_->selector()->pop();
        }
    ;
COMA : ','
    ;
EQUAL : '='
    ;
NAME
    : ('a'..'z') ('a'..'z'|'0'..'9'|'_'|'-'|'\''|':'|'.'|'+')*
    ;
NUMBER
    : ('0'..'9')+ 
    ;
protected
ESC
    : '\\'
        (~('\n' | '"')
        |'"'
            {
                const char* msg0 =
                    "BibTeX compliance does not allow escaped double quotes (\\\") "
                    "in double quoted field values (field = \"a value\"). Use braces"
                    " instead (field = {a \" in value} or field=\" a {\"} in value\")";
                const char* msg1 =
                    "Strict BibTeX would not accept escaped double quotes in "
                    "double quoted field values";
                int l = getLine();
                int c = getColumn();
                switch (pbf_->quoteCompliance())
                    {
                    case Strict :
                        throw xdkbib::parsing_error(getFilename(),msg0,l,c);
                        break;
                    case Warning:
                        pbf_->issueWarning(msg1,getFilename(),l);
                        break;
                    }
            }
        ) 
    ;
protected
ESC_WITH_QUOTE
    : '\\'
        (~('\n' | '"') |'"' ) 
    ;
protected
STRING_INTERNAL_WITH_QUOTES
    : ( ('\\' ~('\n'))=> ESC_WITH_QUOTE
        | ( '\r' { newline(); }
            | '\n' { newline(); }
            | '\\' '\n'   { newline(); }
            )
        | ~( '\r' | '\n' | '\\' | '{' | '}' )
        | '{' STRING_INTERNAL_WITH_QUOTES '}'
        )*
    ;
protected
STRING_INTERNAL
    : t:( ('\\' ~('\n'))=> ESC
        | ( '\r' { newline(); }
            | '\n' { newline(); }
            | '\\' '\n'   { newline(); }
            )
        | ~( '"' | '\r' | '\n' | '\\' | '{' | '}' )
        | '{' STRING_INTERNAL_WITH_QUOTES '}'
        )*
    ;
STRING: '"'! t:STRING_INTERNAL '"'!
    ;
protected
LBRACE : '{'
    ;
protected
VALUE_INTERNAL
    : ((~('{'|'}'|'\n')) | '\n' { newline(); } | VALUE)*
    ;
protected
VALUE
    : ('{' VALUE_INTERNAL '}')
    ;
LBRACE_OR_VALUE
    : { braceStartsValue_ }? t:VALUE
        {
            $setType(VALUE);
            string s = t->getText().substr(1,t->getText().size()-2);
            $setText(s);
        }
    | LBRACE
        {
            $setType(LBRACE);
        }
;
// The \r\n below is to parse DOS file end of lines
WS
    : ( ' ' | '\t' | ('\n'| "\r\n") { newline(); })
        {
            $setType(ANTLR_USE_NAMESPACE(antlr)Token::SKIP);
        }
	;
protected
TYPE
    : '@'! ('a'..'z')+
    ;
protected
STRINGDEF
    : "@string"
    ;
protected
PREAMBLE
    : "@preamble"
    ;
STRINGDEF_OR_PREAMBLE_OR_TYPE
    : ("@string") => STRINGDEF
        {
            $setType(STRINGDEF);
        }
    | ("@preamble") => PREAMBLE
        {
            $setType(PREAMBLE);
        }
    | ('@') => t:TYPE
        {
            $setType(TYPE);
        }
    ;
        
