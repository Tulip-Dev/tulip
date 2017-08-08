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

#include "value.h"
#include "valuelexer.hpp"
#include "valueparser.hpp"

#include <sstream>
#include <iostream>

using namespace std;
using namespace xdkbib;

const Dictionary& xdkbib::isoaccents()
{
  static Dictionary d;
  if (d.empty())
  {
    d[pair<string,string>("`","A")] = "À";
    d[pair<string,string>("'","A")] = "Á";
    d[pair<string,string>("~","A")] = "Ã";
    d[pair<string,string>("\"","A")] = "Ä";
    d[pair<string,string>("^","A")] = "Â";
    d[pair<string,string>("r","A")] = "Å";
    d[pair<string,string>("A","A")] = "Å";
    d[pair<string,string>("A","E")] = "Æ";
    d[pair<string,string>("`","E")] = "È";
    d[pair<string,string>("'","E")] = "É";
    d[pair<string,string>("^","E")] = "Ê";
    d[pair<string,string>("\"","E")] = "Ë";
    d[pair<string,string>("`","I")] = "Ì";
    d[pair<string,string>("'","I")] = "Í";
    d[pair<string,string>("^","I")] = "Î";
    d[pair<string,string>("\"","I")] = "Ï";
    d[pair<string,string>("`","O")] = "Ò";
    d[pair<string,string>("'","O")] = "Ó";
    d[pair<string,string>("^","O")] = "Ô";
    d[pair<string,string>("~","O")] = "Õ";
    d[pair<string,string>("\"","O")] = "Ö";
    d[pair<string,string>("`","U")] = "Ù";
    d[pair<string,string>("'","U")] = "Ú";
    d[pair<string,string>("^","U")] = "Û";
    d[pair<string,string>("\"","U")] = "Ü";
    d[pair<string,string>("'","Y")] = "Ý";
    d[pair<string,string>("c","C")] = "Ç";
    d[pair<string,string>("~","N")] = "Ñ";
    d[pair<string,string>("s","s")] = "ß";
    d[pair<string,string>("`","a")] = "à";
    d[pair<string,string>("'","a")] = "á";
    d[pair<string,string>("~","a")] = "ã";
    d[pair<string,string>("\"","a")] = "ä";
    d[pair<string,string>("^","a")] = "â";
    d[pair<string,string>("r","a")] = "å";
    d[pair<string,string>("a","a")] = "å";
    d[pair<string,string>("a","e")] = "æ";
    d[pair<string,string>("`","e")] = "è";
    d[pair<string,string>("'","e")] = "é";
    d[pair<string,string>("^","e")] = "ê";
    d[pair<string,string>("\"","e")] = "ë";
    d[pair<string,string>("`","i")] = "ì";
    d[pair<string,string>("'","i")] = "í";
    d[pair<string,string>("^","i")] = "î";
    d[pair<string,string>("\"","i")] = "ï";
    d[pair<string,string>("`","o")] = "ò";
    d[pair<string,string>("'","o")] = "ó";
    d[pair<string,string>("^","o")] = "ô";
    d[pair<string,string>("~","o")] = "õ";
    d[pair<string,string>("\"","o")] = "ö";
    d[pair<string,string>("`","u")] = "ù";
    d[pair<string,string>("'","u")] = "ú";
    d[pair<string,string>("^","u")] = "û";
    d[pair<string,string>("\"","u")] = "ü";
    d[pair<string,string>("'","y")] = "ý";
    d[pair<string,string>("c","c")] = "ç";
    d[pair<string,string>("~","n")] = "ñ";
    d[pair<string,string>("s","s")] = "ß";
  }
  return d;
}
//************************************************************
// Implementation of Text
//************************************************************
Text::Text()
{
}
/*!
 * \remark: internally, the text has pointers on words. The destructor
 * frees this pointed words. The correct approach would be to use smart
 * pointers. But in our case, several texts cannot share words through
 * pointers because the class is read-only and the copy operator and copy
 * constructor are disabled (private).
 */
Text::~Text()
{
  clear();
}
void
Text::clear()
{
  for (vector<Word*>::const_iterator iter = words_.begin();
       iter != words_.end();++iter)
  {
    delete *iter;
  }
  words_.clear();
}
Word*
Text::add(Word* w)
{
  if (w) words_.push_back(w);
  else words_.push_back(new Word());
  
  return words_.back();
}
Text&
Text::operator=(const Text& t)
{
  if (&t != this)
  {
    clear();
    for (vector<Word*>::const_iterator iter=t.words_.begin();
	 iter != t.words_.end();++iter)
    {
      words_.push_back((*iter)->clone());
    }
  }
  return *this;
}
Text::Text(const Text& t)
{
  for (vector<Word*>::const_iterator iter=t.words_.begin();
       iter != t.words_.end();++iter)
  {
    words_.push_back((*iter)->clone());
  }
}
void
Text::readFrom(const string& t,const string& split)
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
throw (parsing_error)
#endif
{
  clear();
  if (t.empty()) return;
  try
  {
    istringstream s(t);
    valuelexer lexer(s);    
    lexer.setFilename("xdkbib::Text::readFrom");
    valueparser parser(lexer);
    parser.setFilename("xdkbib::Text::readFrom");
    parser.setText(this);
    if (!split.empty()) parser.setSplitWord(split);
    parser.parse();
  }
  catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException e)
  {
    throw parsing_error(e.toString());
  }
} 
string
Text::content(ContentOptions options) const 
{
  string r;
  if (!words_.empty())
  {
    vector<Word*>::const_iterator iter = words_.begin();
    r = (*iter)->content(options);
    while (++iter != words_.end())
    {
      r = r+" "+(*iter)->content(options);
    }
  }
  return r;
}
bool
Text::hasContent(const string& s,ContentOptions options) const
{
  return content(options) == s;
}
void
Text::translate(const Dictionary& dict)
{
  vector<Word*> newWords;
  bool skip = false;
  for (vector<Word*>::const_iterator iter=words_.begin();
       iter!=words_.end();++iter)
  {
    const Word* w = *iter;
    
    Word* newWord = new Word();
    newWords.push_back(newWord);
    
    for (unsigned int i=0;i<w->letters().size()-1;++i)
    {
      if (skip)
      {
	skip = false; 
      }
      else
      {
	const Letter* l = w->letters()[i];
	const Letter* k = w->letters()[i+1];
	if (l->isSingle())
	{
	  newWord->letters_.push_back(l->clone());
	}
	else if (l->isToken())
	{
	  //bool found=false;
	  for (Dictionary::const_iterator kter=dict.begin();
	       kter!=dict.end();++kter)
	  {
	    const string& cm=kter->first.first;
	    const string& lt=kter->first.second;
	    const string& v=kter->second;
	    if (l->isTheToken(cm) && k->content(BracesStripped) == lt)
	    {
	      for (string::const_iterator iter=v.begin();iter!=v.end();++iter)
	      {
		newWord->letters_.push_back(new SingleLetter(*iter));
	      }
	      skip=true;
	      //found=true;
	      break;
	    }
	  }
	}
	else if (l->isPseudo())
	{
	  PseudoLetter* pl = new PseudoLetter();
	  newWord->letters_.push_back(pl);
	  pl->text_->translate(dict);
	}
      }
    }
    if (!skip)
    {
      newWord->letters_.push_back(w->letters_.back()->clone());      
    }
    skip = false;
  }
  clear();
  words_ = newWords;
}
unsigned int
Text::nbWords() const
{
  return words_.size();
}
const Word*
Text::operator[](unsigned int i) const
{
  return words_[i];
}
const vector<Word*>&
Text::words() const
{
  return words_;
}
Text*
Text::clone() const
{
  Text* t = new Text();
  for (vector<Word*>::const_iterator iter = words_.begin();
       iter != words_.end();++iter)
  {
    t->words_.push_back((*iter)->clone());
  }
  return t;
}
//************************************************************
// Implementation of Word
//************************************************************
/*!
 * \remark: internally, the word has pointers on letters. The destructor
 * frees this pointed letters. The correct approach would be to use smart
 * pointers. But in our case, several words cannot share letters through
 * pointers because the class is read-only and the copy operator and copy
 * constructor are disabled (private).
 */
Word::~Word()
{
  clear();
}
string
Word::content(ContentOptions options) const
{
  string r;
  for (vector<Letter*>::const_iterator
	 iter = letters_.begin();
       iter != letters_.end();++iter)
  {
    r = r+(*iter)->content(options);
  }
  return r;
}
bool
Word::hasContent(const string& s,ContentOptions options) const
{
  return content(options) == s;
}
unsigned int
Word::nbLetters() const
{
  return letters_.size();
}  
const Letter*
Word::operator[](unsigned int i) const
{
  return letters_[i];
}
const vector<Letter*>&
Word::letters() const
{
  return letters_;
}
Word*
Word::clone() const
{
  Word* w = new Word();
  for (vector<Letter*>::const_iterator iter = letters_.begin();
       iter != letters_.end();++iter)
  {
    w->letters_.push_back((*iter)->clone());
  }
  return w;
}
Word::Word()
{
}
Word&
Word::operator=(const Word& w)
{
  clear();
  if (&w != this)
  {
    for (vector<Letter*>::const_iterator iter = w.letters_.begin();
	 iter != letters_.end();++iter)
    {
      letters_.push_back((*iter)->clone());
    }    
  }
  return *this;
}
Word::Word(const Word& w)
{
  for (vector<Letter*>::const_iterator iter = w.letters_.begin();
       iter != letters_.end();++iter)
  {
    letters_.push_back((*iter)->clone());
  }    
}
Letter*
Word::add(Letter* w)
{
  letters_.push_back(w);
  return letters_.back();
}
void
Word::clear()
{
  for (vector<Letter*>::const_iterator
	 iter = letters_.begin();
       iter != letters_.end();++iter)
  {
    delete *iter;
  }
  letters_.clear();
}
bool
Word::hasPseudoLetters() const
{
  for (vector<Letter*>::const_iterator
	 iter = letters_.begin();
       iter != letters_.end();++iter)
  {
    if ((*iter)->isPseudo()) return true;
  }
  return false;
}
//************************************************************
// Implementation of Letter
//************************************************************
bool
Letter::isSingle() const
{
  return false;
}
bool
Letter::isPseudo() const
{
  return false;
}
bool
Letter::isToken() const
{
  return false;
}
const SingleLetter*
Letter::asSingle() const
{
  return NULL;
}
const PseudoLetter*
Letter::asPseudo() const
{
  return NULL;
}
const TokenLetter*
Letter::asToken() const
{
  return NULL;
}
bool
Letter::isTheSingle(char) const
{
  return false;
}
bool
Letter::isTheToken(const string&) const
{
  return false;
}
bool
Letter::hasContent(const string& s,ContentOptions options) const
{
  return content(options) == s;
}
//************************************************************
// Implementation of SingleLetter
//************************************************************
char
SingleLetter::character() const
{
  return c_;
}
string
SingleLetter::content(ContentOptions) const
{
  return string(1,c_);
}
bool
SingleLetter::isSingle() const
{
  return isalpha(c_);
}
SingleLetter::SingleLetter(char c)
  : c_(c)
{
}
Letter*
SingleLetter::clone() const
{
  return new SingleLetter(c_);
}
const SingleLetter*
SingleLetter::asSingle() const
{
  return this;
}
bool
SingleLetter::isTheSingle(char c) const
{
  return c == c_;
}
//************************************************************
// Implementation of PseudoLetter
//************************************************************
string
PseudoLetter::content(ContentOptions options) const
{
  if (options & BracesStripped)
  {
    return text_->content(options);
  }
  return "{"+text_->content(options)+"}";
}
bool
PseudoLetter::isPseudo() const
{
  return true;
}
PseudoLetter::PseudoLetter()
  : text_(new Text())
{
}
PseudoLetter::~PseudoLetter()
{
  delete text_;
}
const Text*
PseudoLetter::text() const
{
  return text_;  
}
Letter*
PseudoLetter::clone() const
{
  PseudoLetter* l = new PseudoLetter;
  l->text_ = text_;
  return l;
}
const PseudoLetter*
PseudoLetter::asPseudo() const
{
  return this;
}
//************************************************************
// Implementation of TokenLetter
//************************************************************
const string&
TokenLetter::token() const
{
  return token_;
}
string
TokenLetter::content(ContentOptions options) const
{
  if (options & CommandStripped)
  {
    return "";
  }
  return "\\"+token_;
}
bool
TokenLetter::isToken() const
{
  return true;
}
TokenLetter::TokenLetter(const string& t)
  : token_(t)
{  
}
Letter*
TokenLetter::clone() const
{
  return new TokenLetter(token_);
}
const TokenLetter*
TokenLetter::asToken() const
{
  return this;
}
bool
TokenLetter::isTheToken(const string& s) const
{
  return token_ == s;
}
