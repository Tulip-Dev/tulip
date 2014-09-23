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

#ifndef PYTHONCPPTYPESCONVERTERS_H
#define PYTHONCPPTYPESCONVERTERS_H

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/ColorScale.h>
#include <tulip/StringCollection.h>
#include <tulip/Size.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/TlpTools.h>

TLP_PYTHON_SCOPE void *convertSipWrapperToCppType(PyObject *sipWrapper, const std::string &cppTypename, const bool transferTo=false);
TLP_PYTHON_SCOPE PyObject *convertCppTypeToSipWrapper(void *cppObj, const std::string &cppTypename, bool fromNew=false);

TLP_PYTHON_SCOPE bool convertPyObjectToBool(PyObject *pyObject, bool &cppObject);
TLP_PYTHON_SCOPE PyObject *convertBoolToPyObject(bool cppObject);

TLP_PYTHON_SCOPE bool convertPyObjectToDouble(PyObject *pyObject, double &cppObject);
TLP_PYTHON_SCOPE PyObject *convertDoubleToPyObject(double cppObject);

TLP_PYTHON_SCOPE bool convertPyObjectToLong(PyObject *pyObject, long &cppObject);
TLP_PYTHON_SCOPE PyObject *convertLongToPyObject(long cppObject);

TLP_PYTHON_SCOPE bool convertPyObjectToUnsignedLong(PyObject *pyObject, unsigned long &cppObject);
TLP_PYTHON_SCOPE PyObject *convertUnsignedLongToPyObject(unsigned long cppObject);

class TLP_PYTHON_SCOPE ValueSetter {

public:

  ValueSetter(tlp::DataSet *dataSet, const std::string& key) :
    dataSet(dataSet), graph(NULL), key(key) {}

  ValueSetter(tlp::Graph *graph, const std::string& key) :
    dataSet(NULL), graph(graph), key(key) {}

  template <typename T>
  void setValue(const T &value) {
    if (dataSet) {
      dataSet->set(key, value);
    }
    else if (graph) {
      graph->setAttribute(key, value);
    }
  }

private :

  tlp::DataSet *dataSet;
  tlp::Graph *graph;
  std::string key;
};

TLP_PYTHON_SCOPE PyObject *getPyObjectFromDataType(const tlp::DataType *dataType, bool noCopy=false);

TLP_PYTHON_SCOPE bool setCppValueFromPyObject(PyObject *pyObj, ValueSetter &valSetter, tlp::DataType *dataType=NULL);

template <typename T>
class PyObjectToCppObjectConvertor {

public:

  bool convert(PyObject *pyObject, T &cppObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    void *cppObjPointer = convertSipWrapperToCppType(pyObject, className);

    if (cppObjPointer) {
      cppObject = *reinterpret_cast<T*>(cppObjPointer);
      return true;
    }

    return false;
  }

};

template <typename T>
class PyObjectToCppObjectConvertor<T*> {

public:

  bool convert(PyObject *pyObject, T *&cppObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    void *cppObjPointer = convertSipWrapperToCppType(pyObject, className, true);

    if (cppObjPointer) {
      cppObject = reinterpret_cast<T*>(cppObjPointer);
      return true;
    }

    return false;
  }
};

template <>
class PyObjectToCppObjectConvertor<PyObject *> {
public:
  bool convert(PyObject *pyObject, PyObject *&cppObject) {
    cppObject = pyObject;
    return true;
  }
};

template <>
class PyObjectToCppObjectConvertor<bool> {
public:
  bool convert(PyObject *pyObject, bool &cppObject) {
    return convertPyObjectToBool(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<double> {
public:
  bool convert(PyObject *pyObject, double &cppObject) {
    return convertPyObjectToDouble(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<float> {
public:
  bool convert(PyObject *pyObject, float &cppObject) {
    double val=0;
    PyObjectToCppObjectConvertor<double> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <>
class PyObjectToCppObjectConvertor<long> {

public:
  bool convert(PyObject *pyObject, long &cppObject) {
    return convertPyObjectToLong(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<int> {
public:
  bool convert(PyObject *pyObject, int &cppObject) {
    long val=0;
    PyObjectToCppObjectConvertor<long> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <>
class PyObjectToCppObjectConvertor<unsigned long> {
public:
  bool convert(PyObject *pyObject, unsigned long &cppObject) {
    return convertPyObjectToUnsignedLong(pyObject, cppObject);
  }
};

template <>
class PyObjectToCppObjectConvertor<unsigned int> {
public:
  bool convert(PyObject *pyObject, unsigned int &cppObject) {
    unsigned long val=0;
    PyObjectToCppObjectConvertor<unsigned long> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <typename T>
class CppObjectToPyObjectConvertor {

public:

  bool convert(const T &cppObject, PyObject *&pyObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    T *objCopy = new T(cppObject);
    PyObject *pyObj = convertCppTypeToSipWrapper(objCopy, className, true);

    if (pyObj) {
      pyObject = pyObj;
      return true;
    }
    else {
      delete objCopy;
    }

    return false;
  }
};

template <typename T>
class CppObjectToPyObjectConvertor<T*> {

public:

  bool convert(T *cppObject, PyObject *&pyObject) {
    std::string className = tlp::demangleClassName(typeid(T).name());

    PyObject *pyObj = convertCppTypeToSipWrapper(cppObject, className);

    if (pyObj) {
      pyObject = pyObj;
      return true;
    }

    return false;
  }

};

template <>
class CppObjectToPyObjectConvertor<PyObject*> {
public:
  bool convert(const PyObject* &cppObject, PyObject *&pyObject) {
    pyObject = const_cast<PyObject*>(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<bool> {
public:
  bool convert(const bool &cppObject, PyObject *&pyObject) {
    pyObject = convertBoolToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<long> {
public:
  bool convert(const long &cppObject, PyObject *&pyObject) {
    pyObject = convertLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<int> {
public:
  bool convert(const int &cppObject, PyObject *&pyObject) {
    pyObject = convertLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<unsigned int> {
public:
  bool convert(const unsigned int &cppObject, PyObject *&pyObject) {
    pyObject = convertUnsignedLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<unsigned long> {
public:
  bool convert(const unsigned long &cppObject, PyObject *&pyObject) {
    pyObject = convertUnsignedLongToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<double> {
public:
  bool convert(const double &cppObject, PyObject *&pyObject) {
    pyObject = convertDoubleToPyObject(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<float> {
public:
  bool convert(const float &cppObject, PyObject *&pyObject) {
    pyObject = convertDoubleToPyObject(cppObject);
    return true;
  }
};



#endif // PYTHONCPPTYPESCONVERTERS_H
