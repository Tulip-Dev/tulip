/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/StlIterator.h>

#include <string>
#include <typeinfo>
#include <list>

namespace tlp {

class Graph;

///@cond DOXYGEN_HIDDEN
// basic interface to embed a data of any type
struct TLP_SCOPE DataMem {
  DataMem() {}
  virtual ~DataMem() {}
};

// basic template to embed a value of a known type
template<typename TYPE> struct TypedValueContainer: public DataMem {
  TYPE value;
  TypedValueContainer() {}
  TypedValueContainer(const TYPE& val) : value(val) {}
  ~TypedValueContainer() {
  }
};
///@endcond

/**
 * @ingroup Structures
 * @brief Describes a value of any type
 */
struct DataType :public DataMem {
  DataType() {}
  DataType(void *value):value(value) {}

  /**
   * @return A deep copy of the stored value.
   */
  virtual DataType *clone() const = 0;

  /**
   * @return The C++ typename of the stored element
   */
  virtual std::string getTypeName() const = 0;

  /**
   * @brief The actual pointer to the element's data
   */
  void *value;
};

///@cond DOXYGEN_HIDDEN
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

// forward declaration of DataSet
class DataSet;

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
  // set a value into a DataSet
  virtual bool setData(DataSet& ds, const std::string& prop, const std::string& value)=0;
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
  // set a value into a DataSet
  virtual bool setData(DataSet& ds, const std::string& prop, const std::string& value)=0;
};

// This class is there to ensure the destruction of DataTypeSerializer objects
// when program ends
class DataTypeSerializerContainer {

public :

  ~DataTypeSerializerContainer() {
    TLP_HASH_MAP<std::string, DataTypeSerializer*>::iterator it = tnTodts.begin();

    for (; it != tnTodts.end() ; ++it) {
      delete it->second;
    }
  }

  TLP_HASH_MAP<std::string, DataTypeSerializer*> tnTodts;
  TLP_HASH_MAP<std::string, DataTypeSerializer*> otnTodts;

};
///@endcond

/**
 * @ingroup Structures
 * @brief A container that can store data from any type.
 *
 * The DataSet aggregate data of various types into a single structure and map each value to a key (std::string) describing its name.
 * DataSet is mainly used in plugins. When creating a plugin, one can add input parameters (using tlp::WithParameter methods) and retrieve them from the dataSet member variable once they have been set by the user.
 **/
class TLP_SCOPE DataSet {
  //Internal list of key-value pairs.
  std::list< std::pair<std::string, DataType*> > data;
  /* management of data serialization
      two hashmap to retrieve data serializer from their
      type names and output type names
      tnTodsts => typename to data type serializer
      otnTodts => output type name to data type serializer */
  static DataTypeSerializerContainer serializerContainer;
  static void registerDataTypeSerializer(const std::string& typeName,
                                         DataTypeSerializer* dts);
public:
  DataSet() {}
  DataSet(const DataSet &set);
  ~DataSet();

  /**
   * @brief Performs a deep copy of a DataSet into another
   */
  DataSet& operator=(const DataSet &set);

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

  /**
   * @brief Registers a serializer for a known type
   *
   * Serializers are used to write/read from std::ostream objects when saving DataSet instances.
  */
  template<typename T> static void registerDataTypeSerializer(const DataTypeSerializer &serializer) {
    registerDataTypeSerializer(std::string(typeid(T).name()), serializer.clone());
  }

  /**
   * @brief write an embedded value
  */
  void writeData(std::ostream& os, const std::string& prop,const DataType* dt) const;

  /**
   * @brief Serializes a DataSet into a stream
   */
  static void write(std::ostream& os, const DataSet& ds);

  /**
   * @brief Read a value and stores it into the specified type
   */
  bool readData(std::istream& is, const std::string& prop, const std::string& outputTypeName);

  /**
   * @brief Reads a stream and stores its contents into a DataSet
   */
  static bool read(std::istream& is, DataSet& ds);

  /**
   * @param str the name of the member to look for
   * @return true if str exists into the DataSet.
   */
  bool exist(const std::string &str) const;

  /**
   * @brief Removes an element from the DataSet
   * @param str the name of the element to remove
   */
  void remove(const std::string &str);

  /**
   * @param str The name of the element to retrieve
   * @return A untyped value for a given element name. NULL if the element does not exist
   */
  DataType* getData(const std::string &str) const;

  /**
   * @brief Set from an untyped value
   */
  void setData(const std::string &str, const DataType* value);

  /**
   * @return An iterator over stored values as a std::pair name => untyped value
   */
  tlp::Iterator< std::pair<std::string, DataType*> > *getValues() const;

  /**
   * @brief Returns the number of registered values
   */
  unsigned int size() const;

  /**
   * @brief Indicates whether the set is empty of not
   */
  bool empty() const;

  /**
   * @return the data type serializer associated to the given typename. NULL if no serializer is found
   */
  static DataTypeSerializer *typenameToSerializer(const std::string &name);

  /**
   * @return A std::string representation of a DataSet
   */
  std::string toString() const;
};

}

#include "cxx/DataSet.cxx"

#endif
