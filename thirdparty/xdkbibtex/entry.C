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

#include "entry.h"
#include <iostream>

using namespace std;
using namespace xdkbib;

//************************************************************
// Implementation of ValuePart
//************************************************************
ValuePart::ValuePart()
{
}
ValuePart::ValuePart(ValuePartType t,const string& v)
  : type_(t),
    content_(v)
{
}
ValuePartType
ValuePart::type() const
{
  return type_;
}
const string&
ValuePart::content() const
{
  return content_;
}
//************************************************************
// Implementation of Field
//************************************************************
Field::Field()
{
}
Field::Field(const string& name,int line)
  : name_(name),
    line_(line)
{ 
}
const string&
Field::name() const
{
  return name_;
}
const vector<ValuePart>&
Field::valueParts() const
{
  return valueParts_;
}
int
Field::line() const
{
  return line_;
}
//************************************************************
// Implementation of FieldHandle
//************************************************************
FieldHandle::FieldHandle()
  : xname_("{null}"),   
    missing_(false),
    entry_(NULL)
{
}
FieldHandle::FieldHandle(Entry* e,const string& name)
  : xname_(name),
    missing_(true),
    entry_(e)
{ 
}
FieldHandle::FieldHandle(Entry* e,const map<string,Field>::iterator& f)
  : xname_(f->first),
    missing_(false),
    field_(f),
    entry_(e)
{ 
}
const Field*
FieldHandle::operator->() const
{
  return &field_->second;
}
bool
FieldHandle::isMissing() const
{
  return missing_;
}
const string&
FieldHandle::name() const
{
  return xname_;
}
string
FieldHandle::value() const
{
  string r="";
  if (!isMissing())
  {
    for (vector<ValuePart>::const_iterator
	   iter = field_->second.valueParts().begin();
	 iter != field_->second.valueParts().end();++iter)
    {
      switch (iter->type())
      {
      case Braced:
      case Quoted:
      case Number:
	r += iter->content();
	break;
      case String:
	r += entry()->valueOf(iter->content());
	break;
      }
    }
  }
  return r;
}
/*!
 * Add a value part to the field. If  isMissing() returns true, then it is
 * first added to entry() (thus it is no longer missing).
 *
 * Identical to operator<<(const ValuePart&) and operator+=(const ValuePart&)
 */
FieldHandle&
FieldHandle::add(const ValuePart& p,int line)
{
  if (isMissing())
  {
    field_ = entry_->addField(xname_).field_;
    field_->second.line_ = line;
  }
  field_->second.valueParts_.push_back(p);
  return *this;
}
/*!
 * Add a value part to the field. If isMissing() returns trues, then it is
 * first added to entry() (thus it is no longer missing).
 *
 * Identical to operator+=(const ValuePart& p) and add(const ValuePart&)
 */
FieldHandle&
FieldHandle::operator<<(const ValuePart& p)
{
  return add(p);
}
/*!
 * Add a value part to the field. If isMissing() returns true, then it is
 * first added to entry() (thus it is no longer missing).
 *
 * Identical to operator<<(const ValuePart& p) and add(const ValuePart&)
 */
FieldHandle&
FieldHandle::operator+=(const ValuePart& p)
{
  return add(p);
}
void
FieldHandle::clear()
{
  if (!isMissing())
  {
    field_->second.valueParts_.clear();
  }
}
const Entry*
FieldHandle::entry() const
{
  return entry_;
}
FieldHandle::operator bool() const
{
  return entry_ != NULL && field_ != entry_->fieldsMap_.end();
}
void
FieldHandle::next()
{  
  if (++field_ != entry_->fieldsMap_.end())
  {
    xname_ = field_->first;
  }
  else
  {
    xname_ = "{null}";
    missing_ = false;
    entry_ = NULL;
  }
}
FieldHandle&
FieldHandle::operator++()
{
  next();
  return *this;
}
//************************************************************
// Implementation of Entry
//************************************************************
Entry::Entry()
{
}
Entry::~Entry()
{
}
Entry::Entry(const string& type,const string& key,int line)
  : type_(type),
    key_(key),
    sortKey_(key),
    line_(line)
{
}
const string&
Entry::type() const
{
  return type_;
}
const string&
Entry::key() const
{
  return key_;
}
FieldHandle
Entry::addField(const string& name,int line)
{
  // convert f.name() to lower case
  string l(name.size(),' ');
  for (unsigned int i=0;i<l.size();++i) l[i] = tolower(name[i]);

  map<string,Field>::iterator iter = 
    fieldsMap_.insert(fieldsMap_.begin(),make_pair(l,Field(name,line)));
  
  return FieldHandle(this,iter);
}
bool
Entry::hasField(const std::string& fieldName) const
{
  return fieldsMap_.find(fieldName) != fieldsMap_.end();
}
FieldHandle
Entry::field(const string& fieldName)
{
  map<string,Field>::iterator fter = fieldsMap_.find(fieldName);
  if (fter != fieldsMap_.end())
  {
    return FieldHandle(this,fter);
  }
  return FieldHandle(this,fieldName);
}
FieldHandle
Entry::firstField()
{
  map<string,Field>::iterator iter = fieldsMap_.begin();
  return !fieldsMap_.empty()?FieldHandle(this,iter):FieldHandle();
}
const string&
Entry::sortKey() const
{
  return sortKey_;
}
void
Entry::setSortKey(const string& s)
{
  sortKey_ = s;
}
int
Entry::line() const
{
  return line_;
}
const map<string,Field>&
Entry::fields() const
{
  return fieldsMap_;
}
const string&
Entry::longestFieldName() const
{
  int maxLen = -1;
  const string* maxStr = NULL;
  for (map<string,Field>::const_iterator
	 iter = fieldsMap_.begin();
       iter != fieldsMap_.end();++iter)
  {
    int l = static_cast<int>(iter->first.size());
    if (l > maxLen)
    {
      maxLen = l;
      maxStr = &iter->first;
    }
  }
  static const string empty = "";
  return maxStr?*maxStr:empty;
}
string
Entry::valueOf(const string&) const
{
  return "";
}
const string&
Entry::comment() const
{
  return comment_;
}
void
Entry::setComment(const string& s)
{
  comment_ = s;
}

