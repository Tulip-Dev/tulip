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

#if defined(__GNUC__)
#include <cxxabi.h>
static std::string demangleClassName(const char* className) {
  static char demangleBuffer[256];
  int status;
  size_t length = 256;
  abi::__cxa_demangle((char *) className, (char *) demangleBuffer,
                      &length, &status);
  return std::string(demangleBuffer);
}
#elif defined(_MSC_VER)
static std::string demangleClassName(const char* className) {
  return std::string(className + 6);
}
#endif

static std::map<std::string, std::string> getTypenamesMap() {
  std::map<std::string, std::string> ret;
  ret[demangleClassName(typeid(std::vector<int>).name())] = "std::vector<int>";
  ret[demangleClassName(typeid(std::vector<unsigned int>).name())] = "std::vector<uint>";
  ret[demangleClassName(typeid(std::vector<long>).name())] = "std::vector<long>";
  ret[demangleClassName(typeid(std::vector<unsigned long>).name())] = "std::vector<unsigned long>";
  ret[demangleClassName(typeid(std::vector<float>).name())] = "std::vector<float>";
  ret[demangleClassName(typeid(std::vector<double>).name())] = "std::vector<double>";
  ret[demangleClassName(typeid(std::vector<bool>).name())] = "std::vector<bool>";
  ret[demangleClassName(typeid(std::vector<std::string>).name())] = "std::vector<std::string>";
  ret[demangleClassName(typeid(std::vector<tlp::node>).name())] = "std::vector<tlp::node>";
  ret[demangleClassName(typeid(std::vector<tlp::edge>).name())] = "std::vector<tlp::edge>";
  ret[demangleClassName(typeid(std::vector<tlp::Coord>).name())] = "std::vector<tlp::Coord>";
  ret[demangleClassName(typeid(std::vector<tlp::Color>).name())] = "std::vector<tlp::Color>";
  ret[demangleClassName(typeid(std::vector<tlp::Size>).name())] = "std::vector<tlp::Size>";
  ret[demangleClassName(typeid(std::vector<tlp::Graph*>).name())] = "std::vector<tlp::Graph*>";
  ret[demangleClassName(typeid(std::vector<tlp::ColorScale>).name())] = "std::vector<tlp::ColorScale>";
  ret[demangleClassName(typeid(std::vector<tlp::StringCollection>).name())] = "std::vector<tlp::StringCollection>";
  ret[demangleClassName(typeid(std::vector<tlp::BooleanProperty *>).name())] = "std::vector<tlp::BooleanProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::ColorProperty *>).name())] = "std::vector<tlp::ColorProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::DoubleProperty *>).name())] = "std::vector<tlp::DoubleProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::IntegerProperty *>).name())] = "std::vector<tlp::IntegerProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::LayoutProperty *>).name())] = "std::vector<tlp::LayoutProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::SizeProperty *>).name())] = "std::vector<tlp::SizeProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::StringProperty *>).name())] = "std::vector<tlp::StringProperty *>";
  ret[demangleClassName(typeid(std::vector<tlp::PropertyInterface *>).name())] = "std::vector<tlp::PropertyInterface *>";
  ret[demangleClassName(typeid(std::list<int>).name())] = "std::list<int>";
  ret[demangleClassName(typeid(std::list<unsigned int>).name())] = "std::list<uint>";
  ret[demangleClassName(typeid(std::list<long>).name())] = "std::list<long>";
  ret[demangleClassName(typeid(std::list<unsigned long>).name())] = "std::list<unsigned long>";
  ret[demangleClassName(typeid(std::list<float>).name())] = "std::list<float>";
  ret[demangleClassName(typeid(std::list<double>).name())] = "std::list<double>";
  ret[demangleClassName(typeid(std::list<bool>).name())] = "std::list<bool>";
  ret[demangleClassName(typeid(std::list<std::string>).name())] = "std::list<std::string>";
  ret[demangleClassName(typeid(std::list<tlp::node>).name())] = "std::list<tlp::node>";
  ret[demangleClassName(typeid(std::list<tlp::edge>).name())] = "std::list<tlp::edge>";
  ret[demangleClassName(typeid(std::list<tlp::Coord>).name())] = "std::list<tlp::Coord>";
  ret[demangleClassName(typeid(std::list<tlp::Color>).name())] = "std::list<tlp::Color>";
  ret[demangleClassName(typeid(std::list<tlp::Size>).name())] = "std::list<tlp::Size>";
  ret[demangleClassName(typeid(std::list<tlp::Graph*>).name())] = "std::list<tlp::Graph*>";
  ret[demangleClassName(typeid(std::list<tlp::ColorScale>).name())] = "std::list<tlp::ColorScale>";
  ret[demangleClassName(typeid(std::list<tlp::StringCollection>).name())] = "std::list<tlp::StringCollection>";
  ret[demangleClassName(typeid(std::list<tlp::BooleanProperty *>).name())] = "std::list<tlp::BooleanProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::ColorProperty *>).name())] = "std::list<tlp::ColorProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::DoubleProperty *>).name())] = "std::list<tlp::DoubleProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::IntegerProperty *>).name())] = "std::list<tlp::IntegerProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::LayoutProperty *>).name())] = "std::list<tlp::LayoutProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::SizeProperty *>).name())] = "std::list<tlp::SizeProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::StringProperty *>).name())] = "std::list<tlp::StringProperty *>";
  ret[demangleClassName(typeid(std::list<tlp::PropertyInterface *>).name())] = "std::list<tlp::PropertyInterface *>";
  ret[demangleClassName(typeid(std::set<int>).name())] = "std::set<int>";
  ret[demangleClassName(typeid(std::set<unsigned int>).name())] = "std::set<uint>";
  ret[demangleClassName(typeid(std::set<long>).name())] = "std::set<long>";
  ret[demangleClassName(typeid(std::set<unsigned long>).name())] = "std::set<unsigned long>";
  ret[demangleClassName(typeid(std::set<float>).name())] = "std::set<float>";
  ret[demangleClassName(typeid(std::set<double>).name())] = "std::set<double>";
  ret[demangleClassName(typeid(std::set<std::string>).name())] = "std::set<std::string>";
  ret[demangleClassName(typeid(std::set<tlp::node>).name())] = "std::set<tlp::node>";
  ret[demangleClassName(typeid(std::set<tlp::edge>).name())] = "std::set<tlp::edge>";
  ret[demangleClassName(typeid(std::set<tlp::Coord>).name())] = "std::set<tlp::Coord>";
  ret[demangleClassName(typeid(std::set<tlp::Color>).name())] = "std::set<tlp::Color>";
  ret[demangleClassName(typeid(std::set<tlp::Size>).name())] = "std::set<tlp::Size>";
  ret[demangleClassName(typeid(std::set<tlp::Graph*>).name())] = "std::set<tlp::Graph*>";
  return ret;
}

static std::map<std::string, std::string> cppTypenameToSipTypename = getTypenamesMap();

template <typename T>
class PyObjectToCppObjectConvertor {

public:

  bool convert(PyObject *pyObject, T &cppObject) {
    std::string className = demangleClassName(typeid(T).name());

    const sipTypeDef* kTypeDef = sipFindType(className.c_str());

    if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, SIP_NOT_NONE)) {
      int state=0, err=0;
      cppObject = *reinterpret_cast<T*>(sipConvertToType(pyObject, kTypeDef, Py_None, SIP_NOT_NONE, &state, &err));
      return true;
    }
    else if (cppTypenameToSipTypename.find(className) != cppTypenameToSipTypename.end()) {
      kTypeDef = sipFindType(cppTypenameToSipTypename[className].c_str());

      if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, SIP_NOT_NONE)) {
        int state=0, err=0;
        cppObject = *reinterpret_cast<T*>(sipConvertToType(pyObject, kTypeDef, Py_None, SIP_NOT_NONE, &state, &err));
        return true;
      }
    }

    return false;
  }
};

template <typename T>
class PyObjectToCppObjectConvertor<T*> {

public:

  bool convert(PyObject *pyObject, T *&cppObject) {
    std::string className = demangleClassName(typeid(T).name());

    const sipTypeDef* kTypeDef = sipFindType(className.c_str());

    if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, SIP_NOT_NONE)) {
      int state=0, err=0;
      cppObject = reinterpret_cast<T*>(sipConvertToType(pyObject, kTypeDef, NULL, SIP_NOT_NONE, &state, &err));
      sipTransferTo(pyObject, pyObject);
      return true;
    }
    else if (cppTypenameToSipTypename.find(className) != cppTypenameToSipTypename.end()) {
      kTypeDef = sipFindType(cppTypenameToSipTypename[className].c_str());

      if (kTypeDef && sipCanConvertToType(pyObject, kTypeDef, SIP_NOT_NONE)) {
        int state=0, err=0;
        cppObject = reinterpret_cast<T*>(sipConvertToType(pyObject, kTypeDef, NULL, SIP_NOT_NONE, &state, &err));
        sipTransferTo(pyObject, pyObject);
        return true;
      }
    }

    return false;
  }
};

template <>
class PyObjectToCppObjectConvertor<bool> {
public:
  bool convert(PyObject *pyObject, bool &cppObject) {
    if (PyFloat_Check(pyObject)) {
      cppObject = (pyObject == Py_True);
      return true;
    }

    return false;
  }
};

template <>
class PyObjectToCppObjectConvertor<double> {
public:
  bool convert(PyObject *pyObject, double &cppObject) {
    if (PyFloat_Check(pyObject)) {
      cppObject = PyFloat_AsDouble(pyObject);
      return true;
    }

    return false;
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
class PyObjectToCppObjectConvertor<unsigned int> {
public:
  bool convert(PyObject *pyObject, unsigned int &cppObject) {
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
    long val=0;
    PyObjectToCppObjectConvertor<long> convertor;
    bool ok = convertor.convert(pyObject, val);
    cppObject = val;
    return ok;
  }
};

template <typename T>
class CppObjectToPyObjectConvertor {

public:

  bool convert(const T &cppObject, PyObject *&pyObject) {
    std::string className = demangleClassName(typeid(T).name());

    const sipTypeDef* kTypeDef = sipFindType(className.c_str());

    if (kTypeDef) {
      pyObject = sipConvertFromNewType(new T(cppObject), kTypeDef, NULL);
      return true;
    }
    else if (cppTypenameToSipTypename.find(className) != cppTypenameToSipTypename.end()) {
      kTypeDef = sipFindType(cppTypenameToSipTypename[className].c_str());

      if (kTypeDef) {
        pyObject = sipConvertFromNewType(new T(cppObject), kTypeDef, NULL);
        return true;
      }
    }

    return false;
  }

};

template <typename T>
class CppObjectToPyObjectConvertor<T*> {

public:

  bool convert(T *cppObject, PyObject *&pyObject) {
    std::string className = demangleClassName(typeid(T).name());

    const sipTypeDef* kTypeDef = sipFindType(className.c_str());

    if (kTypeDef) {
      pyObject = sipConvertFromType(cppObject, kTypeDef, NULL);
      return true;
    }
    else if (cppTypenameToSipTypename.find(className) != cppTypenameToSipTypename.end()) {
      kTypeDef = sipFindType(cppTypenameToSipTypename[className].c_str());

      if (kTypeDef) {
        pyObject = sipConvertFromType(cppObject, kTypeDef, NULL);
        return true;
      }
    }

    return false;
  }

};

template <>
class CppObjectToPyObjectConvertor<bool> {
public:
  bool convert(const bool &cppObject, PyObject *&pyObject) {
    pyObject = PyBool_FromLong(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<long> {
public:
  bool convert(const long &cppObject, PyObject *&pyObject) {
    pyObject = PyLong_FromLong(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<int> {
public:
  bool convert(const int &cppObject, PyObject *&pyObject) {
    pyObject = PyLong_FromLong(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<unsigned int> {
public:
  bool convert(const unsigned int &cppObject, PyObject *&pyObject) {
    pyObject = PyLong_FromLong(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<unsigned long> {
public:
  bool convert(const unsigned long &cppObject, PyObject *&pyObject) {
    pyObject = PyLong_FromLong(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<double> {
public:
  bool convert(const double &cppObject, PyObject *&pyObject) {
    pyObject = PyFloat_FromDouble(cppObject);
    return true;
  }
};

template <>
class CppObjectToPyObjectConvertor<float> {
public:
  bool convert(const float &cppObject, PyObject *&pyObject) {
    pyObject = PyFloat_FromDouble(cppObject);
    return true;
  }
};

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

#define CHECK_CPP_TYPE_CONVERSION(T)\
    if (!pyObj && dataType && dataType->getTypeName() == std::string(typeid(T).name())) {\
        pyObj = getPyObjectFromCppObject(*(reinterpret_cast<T*>(dataType->value)));\
    }

static PyObject *getPyObjectFromDataType(const tlp::DataType *dataType) {

  PyObject *pyObj = NULL;
  CHECK_CPP_TYPE_CONVERSION(bool)
  CHECK_CPP_TYPE_CONVERSION(int)
  CHECK_CPP_TYPE_CONVERSION(long)
  CHECK_CPP_TYPE_CONVERSION(unsigned int)
  CHECK_CPP_TYPE_CONVERSION(unsigned long)
  CHECK_CPP_TYPE_CONVERSION(double)
  CHECK_CPP_TYPE_CONVERSION(float)
  CHECK_CPP_TYPE_CONVERSION(std::string)
  CHECK_CPP_TYPE_CONVERSION(tlp::node)
  CHECK_CPP_TYPE_CONVERSION(tlp::edge)
  CHECK_CPP_TYPE_CONVERSION(tlp::Graph*)
  CHECK_CPP_TYPE_CONVERSION(tlp::DataSet)
  CHECK_CPP_TYPE_CONVERSION(tlp::Coord)
  CHECK_CPP_TYPE_CONVERSION(tlp::Color)
  CHECK_CPP_TYPE_CONVERSION(tlp::Size)
  CHECK_CPP_TYPE_CONVERSION(tlp::ColorScale)
  CHECK_CPP_TYPE_CONVERSION(tlp::StringCollection)
  CHECK_CPP_TYPE_CONVERSION(tlp::BooleanProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::ColorProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::DoubleProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::IntegerProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::LayoutProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::SizeProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::StringProperty *)
  CHECK_CPP_TYPE_CONVERSION(tlp::PropertyInterface *)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::Graph*>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::node>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::edge>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<std::string>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::Coord>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::Color>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::Size>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::ColorScale>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::StringCollection>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::BooleanProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::ColorProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::DoubleProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::IntegerProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::LayoutProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::SizeProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::StringProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::PropertyInterface *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<bool>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<int>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<long>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<unsigned int>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<unsigned long>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<double>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<float>)
  CHECK_CPP_TYPE_CONVERSION(std::set<tlp::Graph*>)
  CHECK_CPP_TYPE_CONVERSION(std::set<tlp::node>)
  CHECK_CPP_TYPE_CONVERSION(std::set<tlp::edge>)
  CHECK_CPP_TYPE_CONVERSION(std::set<std::string>)
  CHECK_CPP_TYPE_CONVERSION(std::set<tlp::Coord>)
  CHECK_CPP_TYPE_CONVERSION(std::set<tlp::Color>)
  CHECK_CPP_TYPE_CONVERSION(std::set<tlp::Size>)
  CHECK_CPP_TYPE_CONVERSION(std::set<int>)
  CHECK_CPP_TYPE_CONVERSION(std::set<long>)
  CHECK_CPP_TYPE_CONVERSION(std::set<unsigned int>)
  CHECK_CPP_TYPE_CONVERSION(std::set<unsigned long>)
  CHECK_CPP_TYPE_CONVERSION(std::set<double>)
  CHECK_CPP_TYPE_CONVERSION(std::set<float>)
  CHECK_CPP_TYPE_CONVERSION(std::list<tlp::Graph*>)
  CHECK_CPP_TYPE_CONVERSION(std::list<tlp::node>)
  CHECK_CPP_TYPE_CONVERSION(std::list<tlp::edge>)
  CHECK_CPP_TYPE_CONVERSION(std::list<std::string>)
  CHECK_CPP_TYPE_CONVERSION(std::list<tlp::Coord>)
  CHECK_CPP_TYPE_CONVERSION(std::list<tlp::Color>)
  CHECK_CPP_TYPE_CONVERSION(std::list<tlp::Size>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::ColorScale>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::StringCollection>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::BooleanProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::ColorProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::DoubleProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::IntegerProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::LayoutProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::SizeProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::StringProperty *>)
  CHECK_CPP_TYPE_CONVERSION(std::vector<tlp::PropertyInterface *>)
  CHECK_CPP_TYPE_CONVERSION(std::list<int>)
  CHECK_CPP_TYPE_CONVERSION(std::list<long>)
  CHECK_CPP_TYPE_CONVERSION(std::list<bool>)
  CHECK_CPP_TYPE_CONVERSION(std::list<unsigned int>)
  CHECK_CPP_TYPE_CONVERSION(std::list<unsigned long>)
  CHECK_CPP_TYPE_CONVERSION(std::list<double>)
  CHECK_CPP_TYPE_CONVERSION(std::list<float>)
  return pyObj;
}

class ValueSetter {

public:

  ValueSetter(tlp::DataSet *dataSet, std::string key) :
    dataSet(dataSet), graph(NULL), key(key) {}

  ValueSetter(tlp::Graph *graph, std::string key) :
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

#define CHECK_SIP_TYPE_CONVERSION(CPP_TYPE, SIP_TYPE_STR)\
    if (sipCanConvertToType(pyObj, sipFindType(SIP_TYPE_STR), SIP_NOT_NONE)) {\
        valSetter.setValue(getCppObjectFromPyObject<CPP_TYPE>(pyObj));\
        return true;\
    }

#define CHECK_SIP_VECTOR_LIST_CONVERSION(CPP_TYPE, SIP_TYPE_STR)\
    if (sipCanConvertToType(pyObj, sipFindType("std::vector<SIP_TYPE_STR>"), SIP_NOT_NONE)) {\
        if (dataType && dataType->getTypeName() == std::string(typeid(std::list<CPP_TYPE>).name())) {\
            valSetter.setValue(getCppObjectFromPyObject<std::list<CPP_TYPE> >(pyObj));\
        } else {\
            valSetter.setValue(getCppObjectFromPyObject<std::vector<CPP_TYPE> >(pyObj));\
        }\
        return true;\
    }

static bool setCppValueFromPyObject(PyObject *pyObj, ValueSetter &valSetter, tlp::DataType *dataType=NULL) {

  if (PyBool_Check(pyObj)) {
    valSetter.setValue(getCppObjectFromPyObject<bool>(pyObj));
    return true;
  }

#if PY_MAJOR_VERSION < 3

  if (PyInt_CheckExact(pyObj) || PyLong_Check(pyObj)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<int>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned int>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<long>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned long>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(getCppObjectFromPyObject<float>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(double).name())) {
      valSetter.setValue(getCppObjectFromPyObject<double>(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<int>(pyObj));
    }

    return true;
  }

#else

  if (PyLong_Check(pyObj)) {
    if (dataType && dataType->getTypeName() == std::string(typeid(int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<int>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned int).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned int>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<long>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(unsigned long).name())) {
      valSetter.setValue(getCppObjectFromPyObject<unsigned long>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(float).name())) {
      valSetter.setValue(getCppObjectFromPyObject<float>(pyObj));
    }
    else if (dataType && dataType->getTypeName() == std::string(typeid(double).name())) {
      valSetter.setValue(getCppObjectFromPyObject<double>(pyObj));
    }
    else {
      valSetter.setValue(getCppObjectFromPyObject<int>(pyObj));
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
  CHECK_SIP_TYPE_CONVERSION(tlp::Coord, "tlp::Coord")
  CHECK_SIP_TYPE_CONVERSION(tlp::Color, "tlp::Color")
  CHECK_SIP_TYPE_CONVERSION(tlp::Size, "tlp::Size")
  CHECK_SIP_TYPE_CONVERSION(tlp::Graph*, "tlp::Graph")
  CHECK_SIP_TYPE_CONVERSION(tlp::DataSet, "tlp::DataSet")
  CHECK_SIP_TYPE_CONVERSION(tlp::ColorScale, "tlp::ColorScale")
  CHECK_SIP_TYPE_CONVERSION(tlp::StringCollection, "tlp::StringCollection")
  CHECK_SIP_TYPE_CONVERSION(tlp::BooleanProperty*, "tlp::BooleanProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::ColorProperty*, "tlp::ColorProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::DoubleProperty*, "tlp::DoubleProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::IntegerProperty*, "tlp::IntegerProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::LayoutProperty*, "tlp::LayoutProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::SizeProperty*, "tlp::SizeProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::StringProperty*, "tlp::StringProperty")
  CHECK_SIP_TYPE_CONVERSION(tlp::PropertyInterface*, "tlp::PropertyInterface")

  CHECK_SIP_VECTOR_LIST_CONVERSION(bool, "bool")
  CHECK_SIP_VECTOR_LIST_CONVERSION(std::string, "std::string")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::node, "tlp::node")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::edge, "tlp::edge")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Coord, "tlp::Coord")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Color, "tlp::Color")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Size, "tlp::Size")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::Graph*, "tlp::Graph")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::DataSet, "tlp::DataSet")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::ColorScale, "tlp::ColorScale")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::StringCollection, "tlp::StringCollection")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::BooleanProperty*, "tlp::BooleanProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::ColorProperty*, "tlp::ColorProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::DoubleProperty*, "tlp::DoubleProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::IntegerProperty*, "tlp::IntegerProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::LayoutProperty*, "tlp::LayoutProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::SizeProperty*, "tlp::SizeProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::StringProperty*, "tlp::StringProperty")
  CHECK_SIP_VECTOR_LIST_CONVERSION(tlp::PropertyInterface*, "tlp::PropertyInterface")

  CHECK_SIP_TYPE_CONVERSION(std::set<std::string>, "std::set<std::string>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::node>, "std::set<tlp::node>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::edge>, "std::set<tlp::edge>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Coord>, "std::set<tlp::Coord>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Color>, "std::set<tlp::Color>")
  CHECK_SIP_TYPE_CONVERSION(std::set<tlp::Size>, "std::set<tlp::Size>")


  return false;
}

#endif // PYTHONCPPTYPESCONVERTERS_H
