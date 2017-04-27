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

#include "base.h"

using namespace std;
using namespace xdkbib;

//************************************************************
// Implementation of parsing_error
//************************************************************
parsing_error::parsing_error(const string& what) throw ()
  : runtime_error(what),
    inline_(true),
    l_(0),
    c_(0)
{
}
parsing_error::parsing_error(const string& fileName,const string& what,unsigned int l,unsigned int c) throw ()
  : runtime_error(what),
    inline_(false),
    fileName_(fileName),
    l_(l),
    c_(c)
{
}
parsing_error::~parsing_error() throw()
{
}
const string&
parsing_error::fileName() const
{
  return fileName_;
}
unsigned int
parsing_error::line() const
{
  return l_;
}
unsigned int
parsing_error::column() const
{
  return c_;
}
bool
parsing_error::isInline() const
{
  return inline_;
}
