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

#include "file.h"

#include "bibfilelexer.hpp"
#include "bibfileparser.hpp"
#include "bibcommandlexer.hpp"
#include "parsedbibfile.h"

#include <antlr/TokenStreamSelector.hpp>
#include <antlr/TokenStreamException.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace antlr;
using namespace xdkbib;

//************************************************************
// Implementation of FileEntry
//************************************************************
FileEntry::FileEntry()
{
}
FileEntry::FileEntry(const Entry& e,const File* f)
  : Entry(e),
    file_(f)
{
}
string
FileEntry::valueOf(const std::string& s) const
{
  return file_->stringText(s);
}
//************************************************************
// Implementation of File
//************************************************************
File::File()
{
}
File::~File()
{
}
void
File::readFromFile(const string& fileName,int o)
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
throw (parsing_error)
#endif
{
  TokenStreamSelector selector;
  ParsedBibFile pbf(this,
		    &selector,
		    (Compliance)(o & 3));

  ifstream s(fileName.c_str());
  bibfilelexer filelexer(s);    
  filelexer.setFilename(fileName);
  bibcommandlexer cmndlexer(filelexer.getInputState());    
    
  selector.addInputStream(&filelexer,"file");
  selector.addInputStream(&cmndlexer,"cmnd");
  selector.select("file");
  
  bibfileparser parser(selector);
  parser.setFilename(fileName);
  
  filelexer.setParsedBibFile(&pbf);
  cmndlexer.setParsedBibFile(&pbf);
  parser.setParsedBibFile(&pbf);

  try
  {
    parser.parse();
  }
  catch (const ANTLR_USE_NAMESPACE(antlr)RecognitionException& e)
  {
    throw parsing_error(fileName,e.getMessage(),e.getLine(),e.getColumn());
  }
  catch (const ANTLR_USE_NAMESPACE(antlr)TokenStreamException& e)
  {
    throw parsing_error(e.toString());
  }
  catch (const parsing_error& e)
  {
    throw e;
  }
  catch (...)
  {
    throw parsing_error("Unknown exception in readFromFile");
  }
}
const vector<vector<ValuePart> >&
File::preambleComponents() const
{
  return preambleComponents_;
}
const vector<FileEntry>&
File::entries() const
{
  return entries_;
}
const map<string,vector<ValuePart> >&
File::strings() const
{
  return strings_;
}
string
File::preamble() const
{
  string r;
  for (vector<vector<ValuePart> >::const_iterator
	 iter = preambleComponents_.begin();
       iter != preambleComponents_.end();++iter)
  {
    for (vector<ValuePart>::const_iterator
	   jter = iter->begin(),
	   jstop= iter->end();
	 jter != jstop;++jter)
    {
      r += jter->content();
    }
  }
  return r;
}
string
File::stringText(const string& s,bool nothrow) const
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
throw(range_error)
#endif
{
  map<string,vector<ValuePart> >::const_iterator fter = strings_.find(s);
  if (fter == strings_.end())
  {
    if (nothrow) return "";
    throw range_error(s);
  }
  const vector<ValuePart>& v = fter->second;

  string r;
  for (vector<ValuePart>::const_iterator
	 iter = v.begin();
       iter != v.end();++iter)
  {
    r += iter->content();
  }
  return r;
} 
void
File::clearPreamble()
{
  preambleComponents_.clear();
}
File&
File::addToPreamble(const ValuePart& p,bool newline)
{
  if (newline || preambleComponents_.empty())
  {
    preambleComponents_.push_back(vector<ValuePart>());
  }
  preambleComponents_.back().push_back(p);
  return *this;
}
FileEntry*
File::addEntry(const Entry& e)
{
  entries_.push_back(FileEntry(e,this));
  return &entries_.back();
}
void
File::clearEntries()
{
  entries_.clear();
}
void
File::clearStrings()
{
  strings_.clear();
}
File&
File::addToString(const string& s,const ValuePart& p)
{
  // convert f.name() to lower case
  string l(s.size(),' ');
  for (unsigned int i=0;i<l.size();++i) l[i] = tolower(s[i]);  
  
  strings_[l].push_back(p);
  return *this;
}
const string&
File::comment() const
{
  return comment_;
}
void
File::setComment(const string& s)
{
  comment_ =  s;
}
