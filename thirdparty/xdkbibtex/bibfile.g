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
    #include "bibcommandparser.hpp"
    #include <iostream>
    
    using namespace std;
}
class bibfileparser extends Parser;
options {
    k=1;
    buildAST = false;
    defaultErrorHandler=false;
}
{
    // Will be included before lexer methods declarations
    // *inside* class definition in parser.hpp
    private:
      ParsedBibFile* pbf_;
      int            mark_;
    public:
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
}
parse
    : (comment | bibcommand)* EOF
        {
            pbf_->file()->setComment(pbf_->currentComment());
            pbf_->clearCurrentComment();
        }
        
    ;
comment
    : c:COMMENT
        {
            pbf_->addToCurrentComment(c->getText()); 
        }
    ;
bibcommand
    : ARROBAS 
        {
            bibcommandparser p(getInputState());
            p.setParsedBibFile(pbf_);
            p.parse();
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
class bibfilelexer extends Lexer;
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
      ParsedBibFile*    pbf_;    
    public:
      inline void setParsedBibFile(ParsedBibFile* pbf)
          {
              pbf_ = pbf;
          }
}
COMMENT : ((~('@' | '\n') | '\n' { newline(); })+)
    ;
protected
ARROBAS : '@'
        {
            pbf_->selector()->push("cmnd");
        }
    ;
protected
ARROBASCOMMENT : "@comment";
        
ARROBAS_OR_COMMENT
{
    int m = mark();    
}
    : ("@comment") => ARROBASCOMMENT
        {
            commit();
            $setType(COMMENT);
        }
    | ('@') => ARROBAS
        {
            rewind(m);
            $setType(ARROBAS);
        }
    ;
        
