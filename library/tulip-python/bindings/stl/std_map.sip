/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

template<T1, T2>
%MappedType std::map<T1, T2> /TypeHint="Dict[T1, T2]"/ {
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode

  PyObject *key = SIP_NULLPTR;
  PyObject *val = SIP_NULLPTR;
  Py_ssize_t pos = 0;

  const sipTypeDef* kpTypeDef1 = getSipType("T1");

  if (!kpTypeDef1) {
    return 0;
  }

  const sipTypeDef* kpTypeDef2 = getSipType("T2");

  if (!kpTypeDef2) {
    return 0;
  }

  if (sipIsErr == SIP_NULLPTR) {
    if (!PyDict_Check(sipPy)) {
      return 0;
    }
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      bool ok = sipCanConvertToType(key, kpTypeDef1, SIP_NOT_NONE);
      ok = ok && sipCanConvertToType(val, kpTypeDef2, SIP_NOT_NONE);
      if (!ok) {
        return 0;
      }
    }
    return 1;
  }

  std::map <T1, T2> *map = new std::map<T1, T2>();

  while (PyDict_Next(sipPy, &pos, &key, &val)) {
    int t1State = 0;
    int t2State = 0;
    
    T1 *t1 = static_cast<T1 *>(sipConvertToType(key, kpTypeDef1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = static_cast<T2 *>(sipConvertToType(val, kpTypeDef2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
    
    if (*sipIsErr) {
      delete map;
      sipReleaseType(t1, kpTypeDef1, t1State);
      sipReleaseType(t2, kpTypeDef2, t2State);
      return 0;
    }
    
    (*map)[*t1] = *t2;
    
    sipReleaseType(t1, kpTypeDef1, t1State);
    sipReleaseType(t2, kpTypeDef2, t2State);
  }

  *sipCppPtr = map;
  return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode

  const sipTypeDef* kpTypeDef1 = getSipType("T1");

  if (!kpTypeDef1) {
    return SIP_NULLPTR;
  }

  const sipTypeDef* kpTypeDef2 = getSipType("T2");

  if (!kpTypeDef2) {
    return SIP_NULLPTR;
  }

  PyObject *pyDict = SIP_NULLPTR;
  pyDict = PyDict_New();
  if (pyDict == SIP_NULLPTR) {
    return SIP_NULLPTR;
  }

  for (std::map<T1, T2>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
    PyObject *key = SIP_NULLPTR;
    PyObject *val = SIP_NULLPTR;
    T1 *cppKey = new T1(it->first);
    T2 *cppVal = new T2(it->second);
    
    key = sipConvertFromNewType(cppKey, kpTypeDef1, sipTransferObj);
    val = sipConvertFromNewType(cppVal, kpTypeDef2, sipTransferObj);
    
    if (key == SIP_NULLPTR || val == SIP_NULLPTR || PyDict_SetItem(pyDict, key, val) < 0) {
      Py_DECREF(pyDict);

      if (key) {
        Py_DECREF(key);
      } else {
        delete cppKey;
      }

      if (val != SIP_NULLPTR) {
        Py_DECREF(val);
      } else {
        delete cppVal;
      }

      return SIP_NULLPTR;
    }
    
    Py_DECREF(key);
    Py_DECREF(val);
  }

  return pyDict;
%End
};

template<T1, T2>
%MappedType std::map<T1, T2 *> /TypeHint="Dict[T1, T2]"/ {
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
  PyObject *key = SIP_NULLPTR;
  PyObject *val = SIP_NULLPTR;
  Py_ssize_t pos = 0;

  const sipTypeDef* kpTypeDef1 = getSipType("T1");

  if (!kpTypeDef1) {
    return 0;
  }

  const sipTypeDef* kpTypeDef2 = getSipType("T2");

  if (!kpTypeDef2) {
    return 0;
  }

  if (sipIsErr == SIP_NULLPTR) {
    if (!PyDict_Check(sipPy)) {
      return 0;
    }
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      bool ok = sipCanConvertToType(key, kpTypeDef1, SIP_NOT_NONE);
      ok = ok && sipCanConvertToType(val, kpTypeDef2, SIP_NOT_NONE);
      if (!ok) {
        return 0;
      }
    }
    return 1;
  }

  std::map <T1, T2 *> *map = new std::map<T1, T2 *>();

  while (PyDict_Next(sipPy, &pos, &key, &val)) {
    int t1State = 0;
    int t2State = 0;
    
    T1 *t1 = static_cast<T1 *>(sipConvertToType(key, kpTypeDef1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = static_cast<T2 *>(sipConvertToType(val, kpTypeDef2, SIP_NULLPTR, SIP_NOT_NONE, &t2State, sipIsErr));
    
    if (*sipIsErr) {
      delete map;
      sipReleaseType(t1, kpTypeDef1, t1State);
      sipReleaseType(t2, kpTypeDef2, t2State);
      return 0;
    }
    
    sipTransferTo(val, val);

    (*map)[*t1] = t2;
    
    sipReleaseType(t1, kpTypeDef1, t1State);
    sipReleaseType(t2, kpTypeDef2, t2State);
  }

  *sipCppPtr = map;
  return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode

  const sipTypeDef* kpTypeDef1 = getSipType("T1");

  if (!kpTypeDef1) {
    return SIP_NULLPTR;
  }

  const sipTypeDef* kpTypeDef2 = getSipType("T2");

  if (!kpTypeDef2) {
    return SIP_NULLPTR;
  }

  PyObject *pyDict = SIP_NULLPTR;
  pyDict = PyDict_New();
  if (pyDict == SIP_NULLPTR) {
    return SIP_NULLPTR;
  }

  for (std::map<T1, T2 *>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
    PyObject *key = SIP_NULLPTR;
    PyObject *val = SIP_NULLPTR;
    T1 *cppKey = new T1(it->first);
    T2 *cppVal = it->second;

    key = sipConvertFromNewType(cppKey, kpTypeDef1, sipTransferObj);
    val = sipConvertFromType(cppVal, kpTypeDef2, sipTransferObj);

    if (key == SIP_NULLPTR || val == SIP_NULLPTR || PyDict_SetItem(pyDict, key, val) < 0) {
      Py_DECREF(pyDict);
      
      if (key) {
        Py_DECREF(key);
      } else {
        delete cppKey;
      }
      
      if (val != SIP_NULLPTR) {
        Py_DECREF(val);
      }
      
      return SIP_NULLPTR;
    }

    Py_DECREF(key);
    Py_DECREF(val);
  }

  return pyDict;
%End
};

template<T2>
%MappedType std::map<float, T2> /TypeHint="Dict[float, T2]"/{
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode

  PyObject *key = SIP_NULLPTR;
  PyObject *val = SIP_NULLPTR;
  Py_ssize_t pos = 0;

  const sipTypeDef* kpTypeDef2 = getSipType("T2");

  if (!kpTypeDef2) {
      return 0;
  }

  if (sipIsErr == SIP_NULLPTR) {
    if (!PyDict_Check(sipPy)) {
      return 0;
    }
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      bool ok = PyFloat_Check(key);
      ok = ok && sipCanConvertToType(val, kpTypeDef2, SIP_NOT_NONE);
      if (!ok) {
        return 0;
      }
    }
    return 1;
  }


  std::map <float, T2> *map = new std::map<float, T2>();

  while (PyDict_Next(sipPy, &pos, &key, &val)) {
    int t2State = 0;
    
    float t1 = PyFloat_AsDouble(key);
    T2 *t2 = static_cast<T2 *>(sipConvertToType(val, kpTypeDef2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
    
    if (*sipIsErr) {
      delete map;		
      sipReleaseType(t2, kpTypeDef2, t2State);
      return 0;
    }
    
    (*map)[t1] = *t2;
    
    sipReleaseType(t2, kpTypeDef2, t2State);
  }

  *sipCppPtr = map;
  return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode

  const sipTypeDef* kpTypeDef2 = getSipType("T2");

  if (!kpTypeDef2) {
      return SIP_NULLPTR;
  }

  PyObject *pyDict = SIP_NULLPTR;
  pyDict = PyDict_New();
  if (pyDict == SIP_NULLPTR) {
    return SIP_NULLPTR;
  }

  for (std::map<float, T2>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
    PyObject *key = SIP_NULLPTR;
    PyObject *val = SIP_NULLPTR;
    float cppKey = it->first;
    T2 *cppVal = new T2(it->second);
    
    key = PyFloat_FromDouble(cppKey);
    val = sipConvertFromNewType(cppVal, kpTypeDef2, sipTransferObj);
    
    if (key == SIP_NULLPTR || val == SIP_NULLPTR || PyDict_SetItem(pyDict, key, val) < 0) {
      Py_DECREF(pyDict);
      
      if (key) {
        Py_DECREF(key);
      } 
      
      if (val != SIP_NULLPTR) {
        Py_DECREF(val);
      } else {
        delete cppVal;
      }
      
      return SIP_NULLPTR;
    }
    
    Py_DECREF(key);
    Py_DECREF(val);
  }

  return pyDict;
%End
};

