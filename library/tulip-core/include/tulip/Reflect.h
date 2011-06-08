/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef _TULIPREFLECT
#define _TULIPREFLECT

#include <tulip/tulipconf.h>
#include <tulip/tuliphash.h>
#include <iostream>
#include <string>
#include <cassert>
#include <typeinfo>
#include <list>
#include <tulip/StlIterator.h>

namespace tlp {

class Graph;

// basic interface to embed a data of any type
struct TLP_SCOPE DataMem {
   DataMem(){}
   virtual ~DataMem() {};
};

// basic template to embed a value of a known type
template<typename TYPE> struct TypedValueContainer: public DataMem {
   TYPE value;
   TypedValueContainer() {}
   TypedValueContainer(const TYPE& val) : value(val) {}
   ~TypedValueContainer() {
  }
};

// Basic class to embed a value of any type
// in a data set
struct DataType :public DataMem {
   DataType(){}
   DataType(void *value):value(value) {}
   // return a deep copy of this
   virtual DataType *clone() const = 0;
   // must return the typeid().name of the type of the embedded value
   virtual std::string getTypeName() const = 0;
   // pointer to the embedded value
   void *value;
 };

 // template class to embed value of known type
 template<typename T>
 struct TypedData :public DataType {
   TypedData(void *value) :DataType(value) {}
   ~TypedData() {
     delete (T*) value;
   }
   DataType* clone() const {
     return new TypedData<T>(new T(*(T*)value));
   }

   std::string getTypeName() const {
     return std::string(typeid(T).name());
   }
 };

 // Basic class for serialization of DataType embedded value
 struct DataTypeSerializer {
   // the readable type name the serializer is designed for
   std::string outputTypeName;
   DataTypeSerializer(const std::string& otn):outputTypeName(otn) {}
   virtual ~DataTypeSerializer() {}
   // return a copy of this
   virtual DataTypeSerializer* clone() const = 0;
   // write the DataType embedded value into the output stream
   virtual void writeData(std::ostream& os, const DataType *data)=0;
   // build a DataType embedding value read from input stream
   virtual DataType* readData(std::istream &is)=0;
 };

 // a template class designs to simplify the developer's work
 // when writing a serializer class
 template<typename T>
   struct TypedDataSerializer :public DataTypeSerializer {
   TypedDataSerializer(const std::string& otn):DataTypeSerializer(otn) {}
   // declare new serialization virtual functions
   virtual void write(std::ostream& os, const T& value)=0;
   // return true if the read of value succeed, false if not
   virtual bool read(std::istream& is, T& value)=0;
   // define virtually inherited functions using the previous ones
   void writeData(std::ostream& os, const DataType* data) {
     write(os, *((T*) data->value));
   }
   DataType* readData(std::istream& is) {
     T value;
     bool ok = read(is, value);
     if (ok)
       return new TypedData<T>(new T(value));
     return NULL;
   }
 };

/**
 * @brief A container which can store data of any type, as long as it has a well-defined copy constructor.
 **/
class TLP_SCOPE DataSet {
  /**
   * @brief Internal list of key-value pairs.
   **/
  std::list< std::pair<std::string, DataType*> > data;
  /** management of data serialization
      two hashmap to retrieve data serializer from their
      type names and output type names
      tnTodsts => typename to data type serializer
      otnTodts => output type name to data type serializer */
  static TLP_HASH_MAP<std::string, DataTypeSerializer*> tnTodts;
  static TLP_HASH_MAP<std::string, DataTypeSerializer*> otnTodts;
  static void registerDataTypeSerializer(const std::string& typeName,
					 DataTypeSerializer* dts);
 public:
  DataSet() {}
  DataSet(const DataSet &set);
  DataSet& operator=(const DataSet &set);
  ~DataSet();
  /**
   * @brief Returns the stored value associated with the given key. 
   * The stored value is a copy of the original value that was set.
   * If there is no value associated with the given key, the input value is left untouched.
   *
   * @param key The key with which the data we want to retrieve is associated.
   * @param value A variable wich will be overwritten with the value to retrieve.
   * @return bool Whether there is a value associated with given key or not.
   **/
  template<typename T> bool get(const std::string &key, T& value) const;
  /** Returns a copy of the value of the variable with name str.
     Type are checked in Debug Mode.
     If the variable str doesn't exist return false else true.
     The data is removed after the call. */
  /**
   * @brief Returns the stored value, and deletes the stored copy.
   * If no value is found, nothing is deleted.
   * 
   * @param key The key with which the data we want to retrieve is associated.
   * @param value A variable wich will be overwritten with the value to retrieve.
   * @return bool Whether there is a value associated with given key or not.
   **/
  template<typename T> bool getAndFree(const std::string &key, T& value);
  
  /**
   * @brief Stores a copy of the given param, associated with the key.
   * The value must have a well-formed copy constructor.
   *
   * @param key The key which can be used to retrieve the data.
   * @param value The data to store.
   * @return void
   **/
  template<typename T> void set(const std::string &key, const T& value);
  /** register a serializer for a known type */
  template<typename T> static void registerDataTypeSerializer(const DataTypeSerializer &serializer) {
    registerDataTypeSerializer(std::string(typeid(T).name()), serializer.clone());
  }
  /** write an embedded value */
  void writeData(std::ostream& os, const std::string& prop,
		 const DataType* dt) const;
  /** static version used for serialization */
  static void write(std::ostream& os, const DataSet& ds);
  /** read an embedded value */
  bool readData(std::istream& is, const std::string& prop,
		const std::string& outputTypeName);
  /** static version used for serialization */
  static bool read(std::istream& is, DataSet& ds);
  /** return true if str exists else false.*/
  bool exist(const std::string &str) const;
  /** remove a named pair */
  void remove(const std::string &str);
  /** return an untyped value if any */
  DataType* getData(const std::string &str) const;
  /** set from an untyped value */
  void setData(const std::string &str, const DataType* value);
  /** Returns an iterator on all values */
  tlp::Iterator< std::pair<std::string, DataType*> > *getValues() const;
  /** Returns the number of registered values */
  unsigned int size() const;
  /** Indicates whether the set is empty of not */
  bool empty() const;

  /** Return the data type serializer associated to the given typename.
      Returns NULL if no serializer is found */
  static DataTypeSerializer *typenameToSerializer(const std::string &name);
};

}

#include "cxx/Reflect.cxx"

#endif


