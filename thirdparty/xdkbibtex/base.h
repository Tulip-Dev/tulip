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

#ifndef XDKBIB_BASE_H
#define XDKBIB_BASE_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <stdexcept>

namespace xdkbib {
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of parsing_error
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class parsing_error : public std::runtime_error {
public:
  parsing_error(const std::string& what) throw();
  parsing_error(const std::string& fileName,const std::string& what,unsigned int l,unsigned int c) throw();
  virtual ~parsing_error() throw();
  const std::string& fileName() const;
  unsigned int line() const;
  unsigned int column() const;
  bool isInline() const;
private:
  bool         inline_;
  std::string  fileName_;
  unsigned int l_;
  unsigned int c_;
};
}

#endif // XDKBIB_BASE_H
