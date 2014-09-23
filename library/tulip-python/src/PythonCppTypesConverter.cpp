/**
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

#include "tulip/PythonIncludes.h"
#include "tulip/PythonCppTypesConverter.h"

static std::map<std::string, std::string> getTypenamesMap() {
  std::map<std::string, std::string> ret;
  ret[tlp::demangleClassName(typeid(std::string).name())] = "std::string";
  ret[tlp::demangleClassName(typeid(std::vector<int>).name())] = "std::vector<int>";
  ret[tlp::demangleClassName(typeid(std::vector<unsigned int>).name())] = "std::vector<uint>";
  ret[tlp::demangleClassName(typeid(std::vector<long>).name())] = "std::vector<long>";
  ret[tlp::demangleClassName(typeid(std::vector<unsigned long>).name())] = "std::vector<unsigned long>";
  ret[tlp::demangleClassName(typeid(std::vector<float>).name())] = "std::vector<float>";
  ret[tlp::demangleClassName(typeid(std::vector<double>).name())] = "std::vector<double>";
  ret[tlp::demangleClassName(typeid(std::vector<bool>).name())] = "std::vector<bool>";
  ret[tlp::demangleClassName(typeid(std::vector<std::string>).name())] = "std::vector<std::string>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::node>).name())] = "std::vector<tlp::node>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::edge>).name())] = "std::vector<tlp::edge>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::Coord>).name())] = "std::vector<tlp::Coord>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::Color>).name())] = "std::vector<tlp::Color>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::Size>).name())] = "std::vector<tlp::Size>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::Graph*>).name())] = "std::vector<tlp::Graph*>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::ColorScale>).name())] = "std::vector<tlp::ColorScale>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::StringCollection>).name())] = "std::vector<tlp::StringCollection>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::BooleanProperty *>).name())] = "std::vector<tlp::BooleanProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::ColorProperty *>).name())] = "std::vector<tlp::ColorProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::DoubleProperty *>).name())] = "std::vector<tlp::DoubleProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::IntegerProperty *>).name())] = "std::vector<tlp::IntegerProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::LayoutProperty *>).name())] = "std::vector<tlp::LayoutProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::SizeProperty *>).name())] = "std::vector<tlp::SizeProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::StringProperty *>).name())] = "std::vector<tlp::StringProperty *>";
  ret[tlp::demangleClassName(typeid(std::vector<tlp::PropertyInterface *>).name())] = "std::vector<tlp::PropertyInterface *>";
  ret[tlp::demangleClassName(typeid(std::list<int>).name())] = "std::list<int>";
  ret[tlp::demangleClassName(typeid(std::list<unsigned int>).name())] = "std::list<uint>";
  ret[tlp::demangleClassName(typeid(std::list<long>).name())] = "std::list<long>";
  ret[tlp::demangleClassName(typeid(std::list<unsigned long>).name())] = "std::list<unsigned long>";
  ret[tlp::demangleClassName(typeid(std::list<float>).name())] = "std::list<float>";
  ret[tlp::demangleClassName(typeid(std::list<double>).name())] = "std::list<double>";
  ret[tlp::demangleClassName(typeid(std::list<bool>).name())] = "std::list<bool>";
  ret[tlp::demangleClassName(typeid(std::list<std::string>).name())] = "std::list<std::string>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::node>).name())] = "std::list<tlp::node>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::edge>).name())] = "std::list<tlp::edge>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::Coord>).name())] = "std::list<tlp::Coord>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::Color>).name())] = "std::list<tlp::Color>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::Size>).name())] = "std::list<tlp::Size>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::Graph*>).name())] = "std::list<tlp::Graph*>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::ColorScale>).name())] = "std::list<tlp::ColorScale>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::StringCollection>).name())] = "std::list<tlp::StringCollection>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::BooleanProperty *>).name())] = "std::list<tlp::BooleanProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::ColorProperty *>).name())] = "std::list<tlp::ColorProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::DoubleProperty *>).name())] = "std::list<tlp::DoubleProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::IntegerProperty *>).name())] = "std::list<tlp::IntegerProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::LayoutProperty *>).name())] = "std::list<tlp::LayoutProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::SizeProperty *>).name())] = "std::list<tlp::SizeProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::StringProperty *>).name())] = "std::list<tlp::StringProperty *>";
  ret[tlp::demangleClassName(typeid(std::list<tlp::PropertyInterface *>).name())] = "std::list<tlp::PropertyInterface *>";
  ret[tlp::demangleClassName(typeid(std::set<int>).name())] = "std::set<int>";
  ret[tlp::demangleClassName(typeid(std::set<unsigned int>).name())] = "std::set<uint>";
  ret[tlp::demangleClassName(typeid(std::set<long>).name())] = "std::set<long>";
  ret[tlp::demangleClassName(typeid(std::set<unsigned long>).name())] = "std::set<unsigned long>";
  ret[tlp::demangleClassName(typeid(std::set<float>).name())] = "std::set<float>";
  ret[tlp::demangleClassName(typeid(std::set<double>).name())] = "std::set<double>";
  ret[tlp::demangleClassName(typeid(std::set<std::string>).name())] = "std::set<std::string>";
  ret[tlp::demangleClassName(typeid(std::set<tlp::node>).name())] = "std::set<tlp::node>";
  ret[tlp::demangleClassName(typeid(std::set<tlp::edge>).name())] = "std::set<tlp::edge>";
  ret[tlp::demangleClassName(typeid(std::set<tlp::Coord>).name())] = "std::set<tlp::Coord>";
  ret[tlp::demangleClassName(typeid(std::set<tlp::Color>).name())] = "std::set<tlp::Color>";
  ret[tlp::demangleClassName(typeid(std::set<tlp::Size>).name())] = "std::set<tlp::Size>";
  ret[tlp::demangleClassName(typeid(std::set<tlp::Graph*>).name())] = "std::set<tlp::Graph*>";
  return ret;
}

static std::map<std::string, std::string> cppTypenameToSipTypename = getTypenamesMap();

void *convertSipWrapperToCppType(PyObject *sipWrapper, const std::string &cppTypename, const bool transferTo) {
  PyObject *pyObject = sipWrapper;
  const sipTypeDef* kTypeDef = sipFindType(cppTypename.c_str());

  if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, SIP_NOT_NONE)) {
    int state=0, err=0;

    if (!transferTo) {
      return sipConvertToType(pyObject, kTypeDef, Py_None, SIP_NOT_NONE, &state, &err);
    }
    else {
      void *p = sipConvertToType(pyObject, kTypeDef, NULL, SIP_NOT_NONE, &state, &err);
      sipTransferTo(pyObject, pyObject);
      return p;
    }
  }
  else if (cppTypenameToSipTypename.find(cppTypename) != cppTypenameToSipTypename.end()) {
    kTypeDef = sipFindType(cppTypenameToSipTypename[cppTypename].c_str());

    if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, SIP_NOT_NONE)) {
      int state=0, err=0;

      if (!transferTo) {
        return sipConvertToType(pyObject, kTypeDef, Py_None, SIP_NOT_NONE, &state, &err);
      }
      else {
        void *p = sipConvertToType(pyObject, kTypeDef, NULL, SIP_NOT_NONE, &state, &err);
        sipTransferTo(pyObject, pyObject);
        return p;
      }
    }
  }

  return NULL;
}

PyObject *convertCppTypeToSipWrapper(void *cppObj, const std::string &cppTypename, bool fromNew) {
  const sipTypeDef* kTypeDef = sipFindType(cppTypename.c_str());

  if (kTypeDef) {
    if (fromNew) {
      return sipConvertFromNewType(cppObj, kTypeDef, NULL);
    }
    else {
      return sipConvertFromType(cppObj, kTypeDef, NULL);
    }
  }
  else if (cppTypenameToSipTypename.find(cppTypename) != cppTypenameToSipTypename.end()) {
    kTypeDef = sipFindType(cppTypenameToSipTypename[cppTypename].c_str());

    if (kTypeDef) {
      if (fromNew) {
        return sipConvertFromNewType(cppObj, kTypeDef, NULL);
      }
      else {
        return sipConvertFromType(cppObj, kTypeDef, NULL);
      }
    }
  }

  return NULL;
}

bool convertPyObjectToBool(PyObject *pyObject, bool &cppObject) {
  if (PyBool_Check(pyObject)) {
    cppObject = (pyObject == Py_True);
    return true;
  }

  return false;
}

PyObject *convertBoolToPyObject(bool cppObject) {
  return PyBool_FromLong(cppObject);
}

bool convertPyObjectToLong(PyObject *pyObject, long &cppObject) {

#if PY_MAJOR_VERSION >= 3

  if (PyLong_Check(pyObject)) {
    cppObject = PyLong_AsLong(pyObject);
    return true;
  }

#else

  if (PyLong_Check(pyObject)) {
    cppObject = PyLong_AsLong(pyObject);
    return true;
  }
  else if (PyInt_Check(pyObject)) {
    cppObject = PyInt_AsLong(pyObject);
    return true;
  }

#endif
  return false;
}



bool convertPyObjectToUnsignedLong(PyObject *pyObject, unsigned long &cppObject) {

#if PY_MAJOR_VERSION >= 3

  if (PyLong_Check(pyObject)) {
    cppObject = PyLong_AsUnsignedLong(pyObject);
    return true;
  }

#else

  if (PyLong_Check(pyObject)) {
    cppObject = PyLong_AsUnsignedLong(pyObject);
    return true;
  }
  else if (PyInt_Check(pyObject)) {
    cppObject = PyInt_AsLong(pyObject);
    return true;
  }

#endif
  return false;
}

PyObject *convertLongToPyObject(long cppObject) {
  return PyLong_FromLong(cppObject);
}

PyObject *convertUnsignedLongToPyObject(unsigned long cppObject) {
  return PyLong_FromUnsignedLong(cppObject);
}

bool convertPyObjectToDouble(PyObject *pyObject, double &cppObject) {
  if (PyFloat_Check(pyObject)) {
    cppObject = PyFloat_AsDouble(pyObject);
    return true;
  }

  return false;
}

PyObject *convertDoubleToPyObject(double cppObject) {
  return PyFloat_FromDouble(cppObject);
}

template <typename T>
PyObject *getPyObjectFromCppObject(const T&cppObject) {
  PyObject *pyObj = NULL;
  CppObjectToPyObjectConvertor<T> convertor;
  convertor.convert(cppObject, pyObj);
  return pyObj;
}

template <typename T>
T getCppObjectFromPyObject(PyObject *pyObj) {
  T v;
  PyObjectToCppObjectConvertor<T> convertor;
  convertor.convert(pyObj, v);
  return v;
}

template <typename T>
T *getCppPointerFromPyObject(PyObject *pyObj) {
  T *v=NULL;
  PyObjectToCppObjectConvertor<T*> convertor;
  convertor.convert(pyObj, v);
  return v;
}

#define CHECK_BASE_CPP_TYPE_CONVERSION(T)\
    if (!pyObj && dataType && dataType->getTypeName() == std::string(typeid(T).name())) {\
        pyObj = getPyObjectFromCppObject(*(reinterpret_cast<T*>(dataType->value)));\
    }\
 
#define CHECK_SIP_CPP_TYPE_CONVERSION(T)\
    if (!pyObj && dataType && dataType->getTypeName() == std::string(typeid(T).name())) {\
        if (noCopy) {\
            pyObj = getPyObjectFromCppObject(reinterpret_cast<T*>(dataType->value));\
        } else {\
            pyObj = getPyObjectFromCppObject(*(reinterpret_cast<T*>(dataType->value)));\
        }\
    }\
 

PyObject *getPyObjectFromDataType(const tlp::DataType *dataType, bool noCopy) {
  PyObject *pyObj = NULL;
  CHECK_BASE_CPP_TYPE_CONVERSION(bool)
  CHECK_BASE_CPP_TYPE_CONVERSION(int)
  CHECK_BASE_CPP_TYPE_CONVERSION(long)
  CHECK_BASE_CPP_TYPE_CONVERSION(unsigned int)
  CHECK_BASE_CPP_TYPE_CONVERSION(unsigned long)
  CHECK_BASE_CPP_TYPE_CONVERSION(double)
  CHECK_BASE_CPP_TYPE_CONVERSION(float)
  CHECK_BASE_CPP_TYPE_CONVERSION(std::string)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::node)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::edge)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::Graph*)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::PluginProgress*)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::DataSet)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::Coord)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::Color)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::Size)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::ColorScale)
  CHECK_SIP_CPP_TYPE_CONVERSION(tlp::StringCollection)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::BooleanProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::ColorProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::DoubleProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::IntegerProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::LayoutProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::SizeProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::StringProperty *)
  CHECK_BASE_CPP_TYPE_CONVERSION(tlp::PropertyInterface *)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Graph*>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::node>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::edge>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<std::string>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Coord>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Color>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::Size>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorScale>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringCollection>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::BooleanProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::DoubleProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::IntegerProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::LayoutProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::SizeProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::PropertyInterface *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<bool>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<unsigned int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<unsigned long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<double>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<float>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Graph*>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::node>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::edge>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<std::string>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Coord>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Color>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<tlp::Size>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<unsigned int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<unsigned long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<double>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::set<float>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Graph*>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::node>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::edge>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<std::string>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Coord>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Color>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<tlp::Size>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorScale>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringCollection>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::BooleanProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::ColorProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::DoubleProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::IntegerProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::LayoutProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::SizeProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::StringProperty *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::vector<tlp::PropertyInterface *>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<bool>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<unsigned int>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<unsigned long>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<double>)
  CHECK_SIP_CPP_TYPE_CONVERSION(std::list<float>)
  return pyObj;
}



#define CHECK_SIP_TYPE_CONVERSION(CPP_TYPE, SIP_TYPE_STR)\
    if (sipCanConvertToType(pyObj, sipFindType(SIP_TYPE_STR), SIP_NOT_NONE)) {\
        valSetter.setValue(getCppObjectFromPyObject<CPP_TYPE>(pyObj));\
    return true;\
    }

#define CHECK_SIP_POINTER_TYPE_CONVERSION(CPP_TYPE, SIP_TYPE_STR)\
    if (sipCanConvertToType(pyObj, sipFindType(SIP_TYPE_STR), SIP_NOT_NONE)) {\
        valSetter.setValue(getCppPointerFromPyObject<CPP_TYPE>(pyObj));\
    return true;\
    }

#define CHECK_SIP_VECTOR_LIST_CONVERSION(CPP_TYPE, SIP_TYPE_STR)\
    if (sipCanConvertToType(pyObj, sipFindType((std::string("std::vector<") + SIP_TYPE_STR + ">").c_str()), SIP_NOT_NONE)) {\
        if (dataType && dataType->getTypeName() == std::string(typeid(std::list<CPP_TYPE>).name())) {\
            valSetter.setValue(getCppObjectFromPyObject<std::list<CPP_TYPE> >(pyObj));\
        } else {\
            valSetter.setValue(getCppObjectFromPyObject<std::vector<CPP_TYPE> >(pyObj));\
        }\
        return true;\
    }

bool setCppValueFromPyObject(PyObject *pyObj, ValueSetter &valSetter, tlp::DataType *dataType) {

  if (PyBool_Check(pyObj)) {
    valSetter.setValue(getCppObjectFromPyObject<bool>(pyObj));
    return true;
  }

#if PY_MAJOR_VERSION < 3

  if (PyInt_CheckExact(pyObj) || PyLong_Check(pyObj)) {
    long val = getCppObjectFromPyObject<long>(pyObj);

    if (dataType && dataType->getTypeName() == std::string(typeid(int).name())) {
      valSetter.setValue(static_cast<int>(val));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned int>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(long).name())) {
      valSetter.setValue(val);
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned long>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(static_cast<float>(val));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(double).name())) {
      valSetter.setValue(static_cast<double>(val));
    }
    else {
      valSetter.setValue(static_cast<int>(val));
    }

    return true;
  }

#else

  if (PyLong_Check(pyObj)) {
    long val = getCppObjectFromPyObject<long>(pyObj);

    if (dataType && dataType->getTypeName() == std::string(typeid(int).name())) {
      valSetter.setValue(static_cast<int>(val));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned int>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(long).name())) {
      valSetter.setValue(val);
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned long>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(static_cast<float>(val));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(double).name())) {
      valSetter.setValue(static_cast<double>(val));
    }
    else {
      valSetter.setValue(static_cast<int>(val));
    }

    return true;
  }


#endif

  if (PyFloat_Check(pyObj)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(getCppObjectFromPyObject<float>(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<double>(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::vector<long>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<int> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<unsigned int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<unsigned int> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<long> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<unsigned long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<unsigned long> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<float> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<double> >(pyObj));

    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<int> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<unsigned int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<unsigned int> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<long> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<unsigned long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<unsigned long> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<float> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<double> >(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<int> >(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::vector<double>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::vector<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<float> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<float> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::list<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::list<double> >(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<std::vector<double> >(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::set<long>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::set<int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<int> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<unsigned int>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<unsigned int> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<long> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<unsigned long>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<unsigned long> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<float> >(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(std::set<double>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<double> >(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<std::set<int> >(pyObj));
    }

    return true;
  }

  if (sipCanConvertToType(pyObj, sipFindType("std::set<double>"), SIP_NOT_NONE)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(std::set<float>).name())) {
      valSetter.setValue(getCppObjectFromPyObject<std::set<float> >(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<std::set<double> >(pyObj));
    }

    return true;
  }

  CHECK_SIP_TYPE_CONVERSION(std::string, "std::string")
  CHECK_SIP_TYPE_CONVERSION(tlp::node, "tlp::node")
  CHECK_SIP_TYPE_CONVERSION(tlp::edge, "tlp::edge")
  CHECK_SIP_TYPE_CONVERSION(tlp::Vec3f, "tlp::Vec3f")
  CHECK_SIP_TYPE_CONVERSION(tlp::Color, "tlp::Color")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::Graph, "tlp::Graph")
  CHECK_SIP_TYPE_CONVERSION(tlp::DataSet, "tlp::DataSet")
  CHECK_SIP_TYPE_CONVERSION(tlp::ColorScale, "tlp::ColorScale")
  CHECK_SIP_TYPE_CONVERSION(tlp::StringCollection, "tlp::StringCollection")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::BooleanProperty, "tlp::BooleanProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::ColorProperty, "tlp::ColorProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::DoubleProperty, "tlp::DoubleProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::IntegerProperty, "tlp::IntegerProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::LayoutProperty, "tlp::LayoutProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::SizeProperty, "tlp::SizeProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::StringProperty, "tlp::StringProperty")
  CHECK_SIP_POINTER_TYPE_CONVERSION(tlp::PropertyInterface, "tlp::PropertyInterface")

  CHECK_SIP_VECTOR_LIST_CONVERSION(bool, "bool")
  CHECK_SIP_VECTOR_LIST_CONVERSION(std::string, "std::string")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::node, "tlp::node")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::edge, "tlp::edge")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Coord, "tlp::Coord")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Color, "tlp::Color")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Size, "tlp::Size")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Graph*, "tlp::Graph*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::DataSet, "tlp::DataSet")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::ColorScale, "tlp::ColorScale")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::StringCollection, "tlp::StringCollection")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::BooleanProperty*, "tlp::BooleanProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::ColorProperty*, "tlp::ColorProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::DoubleProperty*, "tlp::DoubleProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::IntegerProperty*, "tlp::IntegerProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::LayoutProperty*, "tlp::LayoutProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::SizeProperty*, "tlp::SizeProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::StringProperty*, "tlp::StringProperty*")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::PropertyInterface*, "tlp::PropertyInterface*")

  CHECK_SIP_TYPE_CONVERSION(std::set<std::string>, "std::set<std::string>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::node>, "std::set<tlp::node>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::edge>, "std::set<tlp::edge>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Coord>, "std::set<tlp::Coord>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Color>, "std::set<tlp::Color>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Size>, "std::set<tlp::Size>")

  return false;
}
