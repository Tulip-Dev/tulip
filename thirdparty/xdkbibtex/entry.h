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

#ifndef XDKBIB_ENTRY_H
#define XDKBIB_ENTRY_H

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <vector>
#include <map>

namespace xdkbib {
class ValuePart;
class Field;
class FieldHandle;
class Entry;

typedef enum { Braced,Quoted,Number,String } ValuePartType;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of ValuePart
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class ValuePart {
public:
  ValuePart();
  ValuePart(ValuePartType t,const std::string& v);
  ValuePartType type() const;
  const std::string& content() const;
private:
  ValuePartType type_;
  std::string   content_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Field
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Field {
public:
  Field();
  const std::string& name() const;
  const std::vector<ValuePart>& valueParts() const;
  int line() const;
private:
  friend class FieldHandle;
  friend class Entry;
  Field(const std::string& name,int line_=-1);
  std::string            name_;
  std::vector<ValuePart> valueParts_;
  int                    line_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of FieldHandle
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class FieldHandle {
public:
  FieldHandle();
  const Field* operator->() const;
  const std::string& name() const;
  std::string value() const;
  bool isMissing() const;

  FieldHandle& add(const ValuePart& p,int line=-1);
  FieldHandle& operator<<(const ValuePart& p);
  FieldHandle& operator+=(const ValuePart& p);
  template <class O>
  FieldHandle& append(O first,const O& last,int line=-1);
  void clear();

  const Entry* entry() const;
  operator bool() const;

  FieldHandle& operator++();
  void next();
protected:
  friend class Entry;
  FieldHandle(Entry* e,const std::string& name);
  FieldHandle(Entry* e,const std::map<std::string,Field>::iterator& f);
private:
  std::string                           xname_;
  bool                                  missing_;
  std::map<std::string,Field>::iterator field_;
  Entry*                                entry_;
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of Entry
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Entry {
public:
  Entry();
  virtual ~Entry();
  Entry(const std::string& type,const std::string& key,int line=-1);
  const std::string& type() const;
  const std::string& key() const;
  int line() const;
  const std::string& comment() const;
  void setComment(const std::string&);

  FieldHandle addField(const std::string& name,int line=-1);

  bool hasField(const std::string& fieldName) const;
  FieldHandle field(const std::string& fieldName);
  FieldHandle firstField();

  const std::string& sortKey() const;
  void setSortKey(const std::string& s);

  const std::map<std::string,Field>& fields() const;

  const std::string& longestFieldName() const;
protected:
  friend class FieldHandle;
  virtual std::string valueOf(const std::string& s) const;
private:
  std::string                 type_;
  std::string                 key_;
  std::map<std::string,Field> fieldsMap_;
  std::string                 sortKey_;
  int                         line_;
  std::string                 comment_;
};
//************************************************************
// Implementation of FieldHandle
//************************************************************
template <class O>
FieldHandle&
FieldHandle::append(O first,const O& last,int line) {
  if (isMissing()) {
    field_ = entry_->addField(xname_).field_;
    field_->second.line_ = line;
  }

  while (first != last) {
    field_->second.valueParts_.push_back(*first++);
  }

  return *this;
}
}

#endif // XDKBIB_ENTRY_H
