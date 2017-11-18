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

#ifndef XDKBIB_FORMATTER_H
#define XDKBIB_FORMATTER_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <xdkbibtex/value.h>
#include <xdkbibtex/entry.h>
#include <iostream>

namespace xdkbib {
class Entry;
class File;
class Author;
class AuthorList;
class Text;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Formatter
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Formatter {
public:
  virtual ~Formatter();
  virtual std::ostream& operator()(std::ostream& s,const Entry* e) const = 0;
  virtual std::ostream& operator()(std::ostream& s,const File* e) const = 0;
  virtual std::ostream& operator()(std::ostream& s,
                                   const Author* a) const = 0;
  virtual std::ostream& operator()(std::ostream& s,
                                   const AuthorList* a) const = 0;
  virtual std::ostream& operator()(std::ostream& s, const Text* t) const = 0;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of BibTeX
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class BibTeX : public Formatter {
public:
  BibTeX();
  virtual std::ostream& operator()(std::ostream& s,const Entry* e) const;
  virtual std::ostream& operator()(std::ostream& s,const File* e) const;
  virtual std::ostream& operator()(std::ostream& s,const Author* a) const;
  virtual std::ostream& operator()(std::ostream& s,
                                   const AuthorList* a) const;
  virtual std::ostream& operator()(std::ostream& s, const Text* t) const;
  template <class O>
  std::ostream& operator()(std::ostream& s,O first,O last) const;
private:
  void movePos(std::ostream&,unsigned int) const;
  mutable unsigned int indent_;
  mutable unsigned int pos_;
  mutable bool isFirstWord_;
};

template <class F>
std::pair<const Entry*,F> format(const Entry& e) {
  return std::make_pair(&e,F());
}
template <class F>
std::pair<const File*,F> format(const File& f) {
  return std::make_pair(&f,F());
}
template <class F>
std::pair<const Author*,F> format(const Author& a) {
  return std::make_pair(&a,F());
}
template <class F>
std::pair<const AuthorList*,F> format(const AuthorList& l) {
  return std::make_pair(&l,F());
}
template <class F>
std::pair<const Text*,F> format(const Text& t) {
  return std::make_pair(&t,F());
}
template <class F>
std::ostream& operator<<(std::ostream& s,
                         const std::pair<const Entry*,F>& p) {
  return p.second(s,p.first);
}
template <class F>
std::ostream& operator<<(std::ostream& s,
                         const std::pair<const File*,F>& p) {
  return p.second(s,p.first);
}
template <class F>
std::ostream& operator<<(std::ostream& s,
                         const std::pair<const Author*,F>& p) {
  return p.second(s,p.first);
}
template <class F>
std::ostream& operator<<(std::ostream& s,
                         const std::pair<const AuthorList*,F>& p) {
  return p.second(s,p.first);
}
template <class F>
std::ostream& operator<<(std::ostream& s,
                         const std::pair<const Text*,F>& p) {
  return p.second(s,p.first);
}
//************************************************************
// Implementation of BibTeX
//************************************************************
template <class O>
std::ostream&
BibTeX::operator()(std::ostream& s,O first,O last) const {
  for (O jter = first; jter != last; ++jter) {
    if (jter != first) {
      s<<" # ";
      movePos(s,3);
    }

    std::string prefix="";
    std::string suffix="";

    switch (jter->type()) {
    case Braced:
      prefix="{";
      suffix="}";
      break;

    case Quoted:
      prefix=suffix="\"";
      break;

    case Number:
    case String:
      break;
    }

    Text text;
    text.readFrom(jter->content());
    s<<prefix;
    indent_ += prefix.size();
    operator()(s,&text);
    indent_ -= prefix.size();
    s<<suffix;
  }

  return s;
}
}

#endif // XDKBIB_FORMATTER_H
