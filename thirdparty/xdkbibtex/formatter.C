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

#include "formatter.h"
#include "entry.h"
#include "file.h"
#include "author.h"

using namespace std;
using namespace xdkbib;

namespace
{
  static const string fieldPrefix = "  ";
  static const string fieldSuffix = " = ";
  static const unsigned int posMax = 70;
  string
  lowercase(string l)
  {
    for (unsigned int i=0;i<l.size();++i) l[i] = tolower(l[i]);
    return l;
  }
  string
  capitalize(string l)
  {
    if (l.empty()) return l;
    l[0] = toupper(l[0]);
    for (unsigned int i=1;i<l.size();++i) l[i] = tolower(l[i]);
    return l;
  }
}
//************************************************************
// Implementation of Formatter
//************************************************************
Formatter::~Formatter()
{
}
//************************************************************
// Implementation of BibTeX
//************************************************************
BibTeX::BibTeX()
  : indent_(0),
    pos_(0)
{
}
ostream&
BibTeX::operator()(ostream& s,const Entry* e) const
{
  s<<e->comment();
  const unsigned int maxLen = e->longestFieldName().size();
  indent_ = fieldSuffix.size()+maxLen+fieldPrefix.size();
  s<<'@'<<capitalize(e->type())<<'{'<<e->key()<<",\n";
  for (map<string,Field>::const_iterator
	   first = e->fields().begin(),
	 iter = first,
	   istop= e->fields().end();
	 iter != istop;++iter)
  {
    if (iter != first) s<<",\n";
    s<<fieldPrefix<<lowercase(iter->second.name())
     <<string(maxLen-iter->second.name().size(),' ')
     <<fieldSuffix;
    isFirstWord_ = true;
    pos_ = indent_;
    operator()(s,
	       iter->second.valueParts().begin(),
	       iter->second.valueParts().end());
  }
  if (!e->fields().empty()) s<<'\n'; 
  return s<<'}';
}
ostream&
BibTeX::operator()(ostream& s,const File* f) const
{
  bool hasPreamble = false;
  for (vector<vector<ValuePart> >::const_iterator
	 iter = f->preambleComponents().begin(),
	 istop= f->preambleComponents().end();
       iter != istop;++iter)
  {
    if (!iter->empty())
    {
      hasPreamble = true;
      s<<"@Preamble{";
      operator()(s,iter->begin(),iter->end());
      s<<"}\n";
    }
  }
  if (hasPreamble)
  {
    s<<'\n';
  }
  bool hasStrings = false;
  for (map<string,vector<ValuePart> >::const_iterator
	 iter = f->strings().begin(),
	 istop= f->strings().end();
       iter != istop;++iter)
  {
    if (!iter->second.empty())
    {
      hasStrings = true;
      s<<"@String{"<<iter->first<<" = ";
      operator()(s,iter->second.begin(),iter->second.end());
      s<<"}\n";
    }
  }
  if (hasStrings)
  {
    s<<'\n';
  }
  for (vector<FileEntry>::const_iterator
	 first= f->entries().begin(),
	 iter = first,
	 istop= f->entries().end();
       iter != istop;++iter)
  {
    if (iter != first) s<<'\n';
    operator()(s,&*iter);
  }
  s<<f->comment();
  return s;
}
ostream&
BibTeX::operator()(ostream& s,const Author* a) const
{
  if (!a->von().empty())
  {
    s<<a->vonJoin();
    if (!a->last().empty()) s<<' ';
  }
  if (!a->last().empty()) s<<a->lastJoin();
  if (!a->jr().empty() || !a->first().empty())
  {
    s<<',';
    if (!a->jr().empty()) s<<' '<<a->jrJoin()<<',';
    if (!a->first().empty()) s<<' '<<a->firstJoin();
  }
  return s;
}
ostream&
BibTeX::operator()(ostream& s,const AuthorList* a) const
{
  for (AuthorList::const_iterator
	 first = a->begin(),iter = first;
       iter != a->end();++iter)
  {
    if (iter != first) s<<" and ";
    operator()(s,&*iter);
  }
  return s;
}
ostream&
BibTeX::operator()(ostream& s,const Text* t) const
{
  for (vector<Word*>::const_iterator iter=t->words().begin();
       iter != t->words().end();++iter)
  {
    const Word* w = *iter;
    if (!isFirstWord_)
    {
      s<<' ';
      ++pos_;
    }
    string c = w->content();
    // We print the word if it cannot be split (no pseudo letter which could
    // contain whitespace on which we to split) or it fits.
    if (!w->hasPseudoLetters() || pos_+1+c.size() < 80)
    {
      s<<c;
      isFirstWord_ = false;
      movePos(s,c.size());
    }
    else
    {
      // Otherwise (it is too long but it has pseudo letters) we print all
      // the letters until the pseudo letter.
      for (vector<Letter*>::const_iterator iter = w->letters().begin();
	   iter != w->letters().end();++iter)
      {
	const Letter* l = *iter;
	if (!l->isPseudo())
	{
	  string c=l->content();
	  s<<c;
	  isFirstWord_ = false;
	  movePos(s,c.size());	  
	}
	else
	{
	  s<<'{';
	  isFirstWord_ = true;
	  movePos(s,1);
	  operator()(s,l->asPseudo()->text());
	  s<<'}';
	  movePos(s,1);
	  
	}
      }
      isFirstWord_ = false;
    }
  }
  return s;
}
void
BibTeX::movePos(ostream& s,unsigned int p) const
{
  pos_ += p;
  if (pos_>posMax)
  {
    s<<'\n'<<string(indent_,' ');
    pos_ = indent_;
    isFirstWord_ = true;
  }
}  
