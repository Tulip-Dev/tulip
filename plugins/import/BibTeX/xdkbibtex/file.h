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

#ifndef XDKBIB_FILE_H
#define XDKBIB_FILE_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <xdkbibtex/base.h>
#include <xdkbibtex/entry.h>

#include <vector>
#include <map>
#include <string>
#include <stdexcept>

namespace xdkbib {
class File;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of FileEntry
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FileEntry : public Entry {
public:
  FileEntry();
protected:
  friend class File;
  FileEntry(const Entry& e,const File* f);
  virtual std::string valueOf(const std::string& s) const;
private:
  const File* file_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of File
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class File {
public:
  enum {
    StrictQuote       = 0,
    WarnQuote         = 1,
    AcceptQuote       = 2
  };
  File();
  ~File();
  void readFromFile(const std::string& fileName,
                    int options=StrictQuote)
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
  throw(xdkbib::parsing_error)
#endif
  ;

  const std::vector<std::vector<ValuePart> >& preambleComponents() const;
  const std::vector<FileEntry>& entries() const;
  const std::map<std::string,std::vector<ValuePart> >& strings() const;
  std::string preamble() const;
  std::string  stringText(const std::string& s,bool nothrow=true) const
#if (!defined(_MSC_VER) && __cplusplus < 201103L) || (defined(_MSC_VER) && _MSC_VER < 1800)
  throw(std::range_error)
#endif
  ;
  const std::string& comment() const;
  void setComment(const std::string&);


  void clearPreamble();
  File& addToPreamble(const ValuePart& p,bool newline=true);
  FileEntry* addEntry(const Entry& e);
  void clearEntries();
  template <class O>
  void defineString(const std::string& s,O first,const O& last);
  File& addToString(const std::string& s,const ValuePart& p);
  void clearStrings();
private:
  std::vector<std::vector<ValuePart> >          preambleComponents_;
  std::vector<FileEntry>                        entries_;
  std::map<std::string,std::vector<ValuePart> > strings_;
  std::string                                   comment_;
};
//************************************************************
// Implementation of File
//************************************************************
template <class O>
void
File::defineString(const std::string& s,O first,const O& last) {
  // TODO: check if it exists already?
  strings_[s] = std::vector<ValuePart>(first,last);
}
}

#endif // XDKBIB_FILE_H
