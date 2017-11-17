/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
extern PyObject* convertTlpDataSetToPyDict(const tlp::DataSet &dataSet, PyObject *dict = NULL);
extern tlp::DataSet* convertPyDictToTlpDataSet(PyObject *dict, tlp::DataSet *refDataSet=NULL, const std::string &algoName = "");
extern bool setDataSetEntryFromPyObject(tlp::DataSet *dataSet, const std::string &entry, PyObject *value, tlp::DataType *refDataType = NULL, const std::string &algoName = "");
%End

%ModuleCode
PyObject* convertTlpDataSetToPyDict(const tlp::DataSet &dataSet, PyObject *dict) {
  if (!dict) dict = PyDict_New();
  std::pair<std::string, tlp::DataType*> entry;
  forEach(entry, dataSet.getValues()) {
    PyObject *key = sipConvertFromNewType(new std::string(entry.first), sipFindType("std::string"), NULL);
    PyObject *val = NULL;
    if (entry.second->getTypeName() == std::string(typeid(tlp::StringCollection).name())) {
      tlp::StringCollection *sc = static_cast<tlp::StringCollection*>(entry.second->value);
      val = sipConvertFromNewType(new std::string(sc->getCurrentString()), sipFindType("std::string"), NULL);
    } else {
      val = getPyObjectFromDataType(entry.second);
    }
    PyDict_SetItem(dict, key, val);
  }
  return dict;
}

extern tlp::DataSet* convertPyDictToTlpDataSet(PyObject *dict, tlp::DataSet *refDataSet, const std::string &algoName) {
  PyObject *key = NULL;
  PyObject *val = NULL;
  Py_ssize_t pos = 0;
  int state=0, err=0;
  tlp::DataSet *ret = new tlp::DataSet();
  if (refDataSet) {
    *ret = *refDataSet;
  }
  enableErrorMessages(false);
  while (PyDict_Next(dict, &pos, &key, &val)) {
    std::string *keyStr = static_cast<std::string*>(sipConvertToType(key, sipFindType("std::string"), Py_None, SIP_NOT_NONE, &state, &err));
    tlp::DataType *dataType = NULL;
    if (refDataSet) {
      dataType = refDataSet->getData(*keyStr);
    }
    if (!setDataSetEntryFromPyObject(ret, *keyStr, val, dataType, algoName)) {
      delete ret;
      ret = NULL;
      break;
    }
  }
  enableErrorMessages(true);
  return ret;
}

bool setDataSetEntryFromPyObject(tlp::DataSet *dataSet, const std::string &key, PyObject *value, tlp::DataType *refDataType, const std::string &algoName) {
  bool ret = true;
  tlp::DataType* dataType = NULL;
  if (refDataType) {
    dataType = refDataType;
  }
  if (dataType && dataType->getTypeName() == std::string(typeid(tlp::StringCollection).name()) &&
#if PY_MAJOR_VERSION >= 3
  PyUnicode_Check(value)) {
#else
  PyString_Check(value)) {
#endif
    tlp::StringCollection *sc = static_cast<tlp::StringCollection*>(dataType->value);
#if PY_MAJOR_VERSION >= 3
    std::string entry(convertPythonUnicodeObjectToStdString(value));
#else
    std::string entry(PyString_AS_STRING(value));
#endif

    std::vector<std::string> values = sc->getValues();
    if (std::find(values.begin(), values.end(), entry) != values.end()) {
      sc->setCurrent(entry);
      dataSet->set(key, *sc);
    } else {
      std::string msg = "Invalid value provided (\"" + entry + "\") to string parameter '" + key +
                        "' of algorithm '" + algoName + "'.\n";

      msg += "Possible values are : ";
      for (size_t i = 0 ; i < values.size() ; ++i) {
        replaceAll(values[i], "\n", "\\n");
        msg += values[i];
        if (i != values.size() -1) {
          msg += ", ";
        }
      }
      PyErr_SetString(PyExc_Exception, msg.c_str());
      ret = false;
    }
  } else {
    ValueSetter valSetter(dataSet, key);
    if (!setCppValueFromPyObject(value, valSetter, dataType)) {
      std::string msg;
      if (!dataType) {
        msg = "Object of type ";
        msg += std::string(value->ob_type->tp_name);
        msg += " can not be stored in a Tulip dataset.";
      } else {
        msg = "Invalid type for the value provided to parameter '" + key + "' ";
        msg += "for algorithm '" + algoName + "'\n";
        msg += "Provided Python type is '" + std::string(value->ob_type->tp_name) + "'\n";
        msg += "Expected C++ type is '" + tlp::demangleClassName(dataType->getTypeName().c_str()) + "'";
      }
      PyErr_SetString(PyExc_Exception, msg.c_str());
      ret = false;
    }
  }
  delete dataType;
  return ret;
}

%End

namespace tlp {

struct DataSet /TypeHint="Dict[str, Any]", TypeHintValue="{}"/ {

%TypeCode
#include <tulip/PythonCppTypesConverter.h>
%End

%ConvertToTypeCode

  PyObject *key = NULL;
  PyObject *val = NULL;
  Py_ssize_t pos = 0;

  if (sipIsErr == NULL) {
    if (sipCanConvertToType(sipPy, sipFindType("tlp::DataSet"), SIP_NOT_NONE|SIP_NO_CONVERTORS)) {
      return 1;
    } else if (PyDict_Check(sipPy)) {
      while (PyDict_Next(sipPy, &pos, &key, &val)) {
        if (!sipCanConvertToType(key, sipFindType("std::string"), SIP_NOT_NONE)) {
          return 0;
        }
      }
      return 1;
    }
    return 0;
  }

  int state=0, err=0;

  if (PyDict_Check(sipPy)) {
    *sipCppPtr = convertPyDictToTlpDataSet(sipPy);
  } else {
    *sipCppPtr = new tlp::DataSet(*static_cast<tlp::DataSet*>(sipConvertToType(sipPy, sipFindType("tlp::DataSet"), sipTransferObj, SIP_NOT_NONE|SIP_NO_CONVERTORS, &state, &err)));
  }

  if (!*sipCppPtr) {
    *sipIsErr = 1;
  }

  return sipGetState(sipTransferObj);

%End

%Docstring

.. deprecated:: 4.8
   The direct use of that class is now deprecated as it is now mapped to a Python dictionnary
   for commodity of use (see :ref:`Deprecation of direct use of the tlp.DataSet class <deprecatedDataSet>`).

This class is used to store a set of parameters to transmit to a Tulip algorithm.
Below is an exhaustive list of data types that can be stored in a Tulip data set:

  * boolean
  * integer
  * float
  * string
  * :class:`tlp.node`
  * :class:`tlp.edge`
  * :class:`tlp.Graph`
  * :class:`tlp.Coord`
  * :class:`tlp.Color`
  * :class:`tlp.Size`
  * :class:`tlp.DataSet`
  * :class:`tlp.ColorScale`
  * :class:`tlp.StringCollection`
  * :class:`tlp.BooleanProperty`
  * :class:`tlp.ColorProperty`
  * :class:`tlp.DoubleProperty`
  * :class:`tlp.IntegerProperty`
  * :class:`tlp.LayoutProperty`
  * :class:`tlp.SizeProperty`
  * :class:`tlp.StringProperty`
  * :class:`tlp.PropertyInterface`
  * list of those types

If you try to store an unsupported data type, an exception will be thrown.
Each value stored in a data set is associated to a string key. Reading / Writing a value in 
a data set is done through the [] operator as illustrated below::

  # data set creation
  dataSet = tlp.DataSet()
  
  # writing integer values
  dataSet["height"] = 100
  dataSet["width"] = 300
  
  # reading integer values
  area = dataSet["height"] * dataSet["width"] 

When reading a data set value, a reference is returned not a copy.

%End

  DataSet();
  
  DataSet(const tlp::DataSet &set);

  bool exist(const std::string &str) const;
%Docstring
tlp.DataSet.exist(key)

Checks if the data set contains a value associated to a particular key.

:param key: the key to check
:type key: string
:rtype: boolean
%End

  void remove(const std::string &str);
%Docstring
tlp.DataSet.remove(key)

Remove the value associated to a particular key from the data set if it exists.

:param key: the key of the value to remove.
:type key: string
%End

  unsigned int size() const;
%Docstring
tlp.DataSet.size()

Returns the number of values stored in the data set.

:rtype: integer
%End

  std::vector<std::string> getKeys() const /TypeHint="List[str]"/;
%Docstring
tlp.DataSet.getKeys()

Returns the list of keys associated to the values stored in the data set.

:rtype: list of string
%End

%MethodCode
  std::vector<std::string> ret;
  tlp::Iterator< std::pair<std::string, tlp::DataType*> > *it = sipCpp->getValues();
  while (it->hasNext()) {
    ret.push_back(it->next().first);
  }
  delete it;
  sipRes = new std::vector<std::string>(ret);
%End

  void __setitem__(const std::string &attributeName, SIP_PYOBJECT po);
%MethodCode
  sipIsErr = setDataSetEntryFromPyObject(sipCpp, *a0, a1) ? 0 : 1;
%End

  SIP_PYOBJECT __getitem__(const std::string &attributeName);
%MethodCode
  sipRes = NULL;
  tlp::Iterator< std::pair<std::string, tlp::DataType*> > *it = sipCpp->getValues();
  tlp::DataType* dataType = NULL;
  while (it->hasNext()) {
    std::pair<std::string, tlp::DataType*> p = it->next();
    if (p.first == *a0) {
      dataType = p.second;
    }
  }
  delete it;

  sipRes = getPyObjectFromDataType(dataType, true);

  if (!sipRes) {
    std::ostringstream oss;
    oss << "Dataset entry \"" << *a0 << "\" does not exist.";
    sipIsErr = 1;
    PyErr_SetString(PyExc_AttributeError, oss.str().c_str());
  }
%End

  SIP_PYOBJECT toDict();
%Docstring
tlp.DataSet.toDict()

Converts that data set to a Python dictionnary.

:rtype: a dictionnary reflecting the data set content
%End
%MethodCode
  sipRes = convertTlpDataSetToPyDict(*sipCpp);
%End

  SIP_PYOBJECT __repr__() const;
%MethodCode
  PyObject *pyDict = convertTlpDataSetToPyDict(*sipCpp);
  sipRes = PyObject_Repr(pyDict);
  Py_XDECREF(pyDict);
%End

};
};
