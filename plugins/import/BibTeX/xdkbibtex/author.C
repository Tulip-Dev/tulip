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

#include "author.h"
#include "value.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <deque>

using namespace std;
using namespace xdkbib;

namespace
{
  typedef enum { Lower,Upper,Undetermined } Case;
  Case
  caseOf(const Word* w)
  {
    vector<Letter*>::const_iterator iter = w->letters().begin();
    while (iter != w->letters().end())
    {
      //  Walk to the first single letter
      while (iter != w->letters().end() &&
	     (*iter)->isPseudo())
      {
	++iter;
      }
      // If not found, return Undetermined
      if (iter == w->letters().end()) return Undetermined;
      // For token search the token letters
      if ((*iter)->isToken())
      {
	TokenLetter* l=dynamic_cast<TokenLetter*>(*iter);
	for (string::const_iterator
	       jter = l->token().begin(),
	       jstop= l->token().end();
	     jter != jstop;++jter)
	{
	  if (isalpha(*jter)) return islower(*jter)?Lower:Upper;
	}
      }
      // Otherwise check if case can be fixed, if yes return it.
      if ((*iter)->isSingle())
      {
	char c = dynamic_cast<SingleLetter*>(*iter)->character();
	if (isalpha(c)) return islower(c)?Lower:Upper;
      }
      ++iter;
    }
    return Undetermined;
  }
  
  template <class T>
  string join(T first,T last,const char* sep)
  {
    string s;
    if (first != last)
    {
      s=*first;
      while (++first != last) s += sep + *first;
    }
    return s;
  }
}
//************************************************************
// Implementation of Author
//************************************************************
Author::Author()
{
}
const vector<string>&
Author::first() const
{
  return first_;
}
const vector<string>&
Author::von() const
{
  return von_;
}
const vector<string>&
Author::jr() const
{
  return jr_;
}
const vector<string>&
Author::last() const
{
  return last_;
}
string
Author::firstJoin(const char* sep) const
{
  return join(first_.begin(),first_.end(),sep);
}
string
Author::vonJoin(const char* sep) const
{
  return join(von_.begin(),von_.end(),sep);
}
string
Author::jrJoin(const char* sep) const
{
  return join(jr_.begin(),jr_.end(),sep);
}
string
Author::lastJoin(const char* sep) const
{
  return join(last_.begin(),last_.end(),sep);
}
void
Author::pushFirst(const string& v)
{
  first_.push_back(v);
}
void
Author::pushVon(const string& v)
{
  von_.push_back(v);
}
void
Author::pushJr(const string& v)
{
  jr_.push_back(v);
}
void
Author::pushLast(const string& v)
{
  last_.push_back(v);
}
//************************************************************
// Implementation of AuthorList
//************************************************************
void
AuthorList::readFrom(const char* nameList)
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
throw(parsing_error)
#endif
{
  clear();
  Text t;
  // We split in name parts according to top level whitespaces and comas.
  t.readFrom(nameList,",");
  // Now we walk authors separated by "and"
  for (unsigned int first=0,i=first,last=t.words().size();i<last;++i)
  {
    const Word* w = t.words()[i];
    if (w->content() == "and" || i==last-1)
    {
      // Depending wether we met "and" or end of text, we skip the i-th word
      // or not.
      unsigned int j = (i!=last-1)?i:(i+1);
      if (j>first)
      {
	// We split in groups separated by comas. If they are more than 3
	// comas, we throw an exception.
	deque<deque<Word*> > groups;
	groups.push_back(deque<Word*>());
	while (first != j)
	{
	  Word* wf = t.words()[first];
	  if (wf->content() == ",")
	  {
	    groups.push_back(deque<Word*>());
	  }
	  else
	  {
	    groups.back().push_back(wf);
	  }
	  ++first;
	}
	++first;
	// Now we test for the three cases
	if (groups.size() == 1)
	{
	  const deque<Word*>& g = groups[0];
	  push_back(Author());
	  // First is the longest sequence of white-space separated words
	  // starting with an uppercase or caseless and not being the whole
	  // string.
	  unsigned int i=0;
	  while (i<g.size()-1 && caseOf(g[i]) != Lower)
	  {
	    back().pushFirst(g[i++]->content());
	  }
	  // von is the longest sequence of whitespace separated words whose
	  // last word starts with lower case and that is not the whole
	  // string.
	  int vonFirst = i,vonLast = i-1;
	  while (i<g.size()-1)
	  {
	    if (caseOf(g[i]) == Lower) vonLast = i;
	    ++i;
	  }
	  while (vonFirst<=vonLast)
	  {
	    back().pushVon(g[vonFirst++]->content());
	  }
	  // Last is what remains
	  i = vonLast+1;
	  while (i<g.size())
	  {
	    back().pushLast(g[i++]->content());
	  }	  
	}
	else if (groups.size() == 2 || groups.size() == 3)
	{
	  push_back(Author());
	  // von is the longest sequence of whitespace separated words whose
	  // last word starts with lower case and that is not the whole
	  // string.
	  const deque<Word*>& g = groups[0];
	  int vonFirst = 0,vonLast = -1;
	  unsigned int i=0;
	  while (i<g.size()-1)
	  {
	    if (caseOf(g[i]) == Lower) vonLast = i;
	    ++i;
	  }
	  while (vonFirst<=vonLast)
	  {
	    back().pushVon(g[vonFirst++]->content());
	  }
	  // Last is what remains
	  i = vonLast+1;
	  while (i<g.size())
	  {
	    back().pushLast(g[i++]->content());
	  }
	  // If there is 3 parts, jr is eveything in the 2nd group
	  if (groups.size() == 3)
	  {
	    for (deque<Word*>::const_iterator
		   iter = groups[1].begin(),
		   istop= groups[1].end();
		 iter != istop;++iter)
	    {
	      back().pushJr((*iter)->content());
	    }
	  }
	  // First is eveything in last group
	  for (deque<Word*>::const_iterator
		 iter = groups.back().begin(),
		 istop= groups.back().end();
	       iter != istop;++iter)
	  {
	    back().pushFirst((*iter)->content());
	  }	  
	}
	else throw parsing_error("more than 2 comas in name");
      }
      // TODO: else warning of empty authors?
      ++i;
    }
  }
}
