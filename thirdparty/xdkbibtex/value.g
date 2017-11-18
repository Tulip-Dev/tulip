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
    #include <xdkbibtex/value.h>
    #include <stack>
    #include <stdexcept>
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
    #include "value.h"
    #include <iostream>
    
    using namespace std;
    using namespace xdkbib;

    void valueparser::setSplitWord(const string s) throw(invalid_argument)
        {
            try {
                splitText_.clear();
                splitText_.readFrom(s);
                if (splitText_.nbWords() > 1)
                    {
                        throw invalid_argument(s+" is not a single word");
                    }
                splitWord_ = splitText_.words_[0];
            }
            catch (parsing_error e)
                {
                    throw invalid_argument(s+" is not a valid Text");
                }
        }

}
class valueparser extends Parser;
options {
    k=2;
    buildAST = false;
    defaultErrorHandler=false;
}
{
    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
    private:
      std::stack<xdkbib::Text*> textStack_;
      xdkbib::Text              splitText_;
      xdkbib::Word*             splitWord_;
    public:
      void setText(xdkbib::Text* t)
          {
              textStack_.push(t);
              splitWord_ = NULL;
          }
      void setSplitWord(const std::string s) throw(std::invalid_argument);
}
parse
    : (text)? EOF
    ;
text
    : (WS)? word (WS (word)?)* 
    ;
word 
{
    Word* w;
    Letter* l;
}
    :
        {
            w = new Word();
        }
        (l=letter { w->letters_.push_back(l); })+
        {
            // Here we check if we cannot subsplit the word
            if (splitWord_ && splitWord_->nbLetters()) {
                unsigned int start = 0;
                for (int i=0;i+splitWord_->nbLetters()<=w->nbLetters();++i) {
                    bool b = true;
                    for (unsigned int l=0;l<splitWord_->nbLetters() & b;++l) {
                        b &= ((*w)[i+l]->content() ==
                              (*splitWord_)[l]->content());
                    }
                    // TODO: remove empty words (yes,no,as an option?)
                    if (b) {
                        if (start != i) {
                            Word* wk = new Word();
                            for (unsigned int l=start;l<i;++l) {
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
                    for (unsigned int l=start;l<w->nbLetters();++l) {
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
        }        
    ;
letter returns [xdkbib::Letter* l]
    : c:LETTER
        {
            l = new SingleLetter(c->getText()[0]);
        }
    | LBRACE
        {
            PseudoLetter* pl = new PseudoLetter();
            textStack_.push(pl->text_);
            l = pl;
        }
        text
        {
            textStack_.pop();
        }
        RBRACE
    | LBRACE
        {
            l = new PseudoLetter();
        }
        RBRACE
    | t:TOKEN
        {
            l = new TokenLetter(t->getText());
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
    #include <iostream>
    
    using namespace std;
}
class valuelexer extends Lexer;
options {
    k=2;
    defaultErrorHandler=false;
    caseSensitive = false;
    charVocabulary='\3'..'\377';
}
{
    // Will be included before lexer methods declarations
    // *inside* class definition in lexer.hpp
}
LBRACE : '{';
RBRACE : '}' ;
TOKEN : '\\'! (('a'..'z' | '0'..'9')+
        | ~('a'..'z' | '0'..'9' | '{' | '}' | ' ' | '\t' | '\n' | '\r')) ;
LETTER
    :
        ~('{' | '}' | ' ' | '\t' | '\n' | '\r')
    ;
// The \r\n below is to parse DOS file end of lines
WS
    :
        ( ' ' | '\t' | ('\n'| "\r\n") { newline(); })+
	;
