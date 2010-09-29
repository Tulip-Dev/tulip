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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/tulipconf.h>
#include <tulip/tuliphash.h>
#include <iostream>
#include <string>
#include <cassert>
#include <typeinfo>
#include <list>
#include "tulip/StlIterator.h"

namespace tlp {

struct Graph;

 struct TLP_SCOPE DataMem {
   DataMem(){}
   virtual ~DataMem() {};
 };

 // Basic class to embed a value of any type
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

 // template class to embed value of know type
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
   ~DataTypeSerializer() {}
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

/*!  A container which allows insertion of different types.
     The inserted data must have a copy-constructor well done */
class TLP_SCOPE DataSet {
  /** the list of name value pairs */
  std::list< std::pair<std::string, DataType*> > data;
  /** management of data serialization
      two hashmap to retrieve data serializer from their
      type names and output type names
      tnTodsts => typename to data type serializer
      otnTodts => output type name to data type serializer */
  static TLP_HASH_MAP<std::string, DataTypeSerializer*> tnTodts, otnTodts;
  static void registerDataTypeSerializer(const std::string& typeName,
					 DataTypeSerializer* dts);
 public:
  DataSet() {}
  DataSet(const DataSet &set);
  DataSet& operator=(const DataSet &set);
  ~DataSet();
  /** Returns a copy of the value of the variable with name str.
     Type are checked in Debug Mode.
     If the variable str doesn't exist return false else true. */
  template<typename T> bool get(const std::string &str, T& value) const;
  /** Returns a copy of the value of the variable with name str.
     Type are checked in Debug Mode.
     If the variable str doesn't exist return false else true.
     The data is removed after the call. */
  template<typename T> bool getAndFree(const std::string &str, T& value);
  /** Set the value of the variable str.*/
  template<typename T> void set(const std::string &str, const T& value);
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
  /** Return an iterator on all values */
  Iterator< std::pair<std::string, DataType*> > *getValues() const;
};



///This class enables to define a structure
struct TLP_SCOPE StructDef {
  ///Add the variable of type T and name str in the structure.
  template<typename T> void add(const char* str,
				const char* inHelp = 0,
				const char* inDefValue = 0,
				bool mandatory = true);

  ///Returns if a name field exists
  bool hasField(std::string);

  ///Get iterator on structure fields
  Iterator< std::pair<std::string,std::string> >* getField() const;

  ///Get field help & default string-value (see also XXXType in Types.h)
  std::string getHelp( std::string str ) const;
  std::string getDefValue( std::string str ) const;
  void setDefValue(std::string name, std::string value);
  ///Return if field is mandatory
  bool isMandatory(std::string str) const;

  ///Remove the variable which have str has name in the structure.
  void		erase(std::string str);

  ///Build a default dataSet according to fields
  ///The optional Graph is needed to create properties (PropertyInterface*)
  void		buildDefaultDataSet( DataSet & ioDataSet, Graph * inG = 0 );
private:
  std::list< std::pair<std::string, std::string> > data;
  std::map<std::string,std::string> help;
  std::map<std::string,std::string> defValue;
  std::map<std::string,bool> mandatory;
};




//
// Standard structDef's HTML Help

#define	HTML_HELP_OPEN()	"<table><tr><td>" \
							"<table CELLSPACING=5 bgcolor=\"#EEEEEE\">"
#define	HTML_HELP_DEF(A,B)	"<tr><td><b>" A "</b></td><td><FONT COLOR=\"red\">" B "</td></tr>"
#define	HTML_HELP_BODY()	"</table></td><td>"
#define	HTML_HELP_CLOSE()	"</td></tr></table>"


/*
 * Usage :

  	const char * paramHelp[] = {
		// property
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "DoubleProperty" ) \
		HTML_HELP_BODY() \
		"This metricProxy is used to affect scalar values to graph items." \
		"The meaning of theses values depends of the choosen color model." \
		HTML_HELP_CLOSE(),

		// colormodel
		HTML_HELP_OPEN() \
		HTML_HELP_DEF( "type", "int" ) \
		HTML_HELP_DEF( "values", "[0,1]" ) \
		HTML_HELP_DEF( "default", "0" ) \
		HTML_HELP_BODY() \
		"This value defines the type of color interpolation. Following values are valid :" \
		"<ul><li>0: linear mapping on hue ;</li><li>1: RGB interpolation</li></ul>" \
		HTML_HELP_CLOSE(),
	};
	
	...	
	addParameter<DoubleProperty>("property",paramHelp[0]);
    addParameter<int>("colormodel",paramHelp[1]);
	...

 *
 */
}

#include "cxx/Reflect.cxx"

#endif


