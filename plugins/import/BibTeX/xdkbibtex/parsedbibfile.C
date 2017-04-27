// Copyright (C) 2004 Xavier Décoret <Xavier.Decoret@imag.fr>

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

#include "parsedbibfile.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace antlr;
using namespace xdkbib;

//************************************************************
//Implementation of ParsedBibFile
//************************************************************
ParsedBibFile::ParsedBibFile(xdkbib::File* file,
			     TokenStreamSelector* s,
			     Compliance quote)
  : file_(file),
    selector_(s),
    quoteCompliance_(quote)
{
}
ParsedBibFile::~ParsedBibFile()
{
}
void
ParsedBibFile::issueWarning(const string& v,const string& f,unsigned int l)
{
  cout<<"Warning--"<<v<<"-- in "<<f<<", line "<<l<<endl;
  // TODO: store warning in a list and let users of the
  // parse print them or not.
}
void
ParsedBibFile::setQuoteCompliance(Compliance c)
{
  quoteCompliance_ = c;
}
const string&
ParsedBibFile::currentComment() const
{
  return comment_;
}
void
ParsedBibFile::addToCurrentComment(const string& c)
{
  comment_ += c;
}
void
ParsedBibFile::clearCurrentComment()
{
  comment_.clear();
}

