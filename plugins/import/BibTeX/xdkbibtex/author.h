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

#ifndef XDKBIB_AUTHOR_H
#define XDKBIB_AUTHOR_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <xdkbibtex/base.h>

#include <string>
#include <vector>

namespace xdkbib {
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Author
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Author {
public:
  Author();
  const std::vector<std::string>& first() const;
  const std::vector<std::string>& von() const;
  const std::vector<std::string>& jr() const;
  const std::vector<std::string>& last() const;

  std::string firstJoin(const char* sep=" ") const;
  std::string vonJoin(const char* sep=" ") const;
  std::string jrJoin(const char* sep=" ") const;
  std::string lastJoin(const char* sep=" ") const;

  void pushFirst(const std::string&);
  void pushVon(const std::string&);
  void pushJr(const std::string&);
  void pushLast(const std::string&);
private:
  std::vector<std::string> first_;
  std::vector<std::string> von_;
  std::vector<std::string> jr_;
  std::vector<std::string> last_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of AuthorList
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class AuthorList : public std::vector<Author> {
public:
  void readFrom(const char* nameList)
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
  throw(xdkbib::parsing_error)
#endif
  ;
};
}

#endif // XDKBIB_AUTHOR_H
