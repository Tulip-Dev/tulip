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

#ifndef XDKBIB_PARSEDBIBFILE_H
#define XDKBIB_PARSEDBIBFILE_H

#include "file.h"

#include <antlr/TokenStreamSelector.hpp>
#include <string>
#include <deque>
#include <map>
#include <stdexcept>
#include <algorithm>

#if HAVE_CONFIG_H
#include <config.h>
#endif

typedef enum { Strict=0, Warning=1, Accept=4 } Compliance;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of ParsedBibFile
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ParsedBibFile {
public:
  ParsedBibFile(xdkbib::File* file,
                antlr::TokenStreamSelector* selector_,
                Compliance quote);
  virtual ~ParsedBibFile();

  virtual void issueWarning(const std::string&,
                            const std::string&,unsigned int);

  xdkbib::File* file() const;

  inline antlr::TokenStreamSelector* selector() const;

  inline Compliance quoteCompliance() const;
  void setQuoteCompliance(Compliance c);

  const std::string&  currentComment() const;
  void addToCurrentComment(const std::string&);
  void clearCurrentComment();
private:
  xdkbib::File*               file_;
  antlr::TokenStreamSelector* selector_;
  Compliance                  quoteCompliance_;
  std::string                 comment_;
};
//************************************************************
//Implementation of ParsedBibFile
//************************************************************
inline xdkbib::File*
ParsedBibFile::file() const {
  return file_;
}
inline antlr::TokenStreamSelector*
ParsedBibFile::selector() const {
  return selector_;
}
inline Compliance
ParsedBibFile::quoteCompliance() const {
  return quoteCompliance_;
}

#endif // XDKBIB_PARSEDBIBFILE_H
