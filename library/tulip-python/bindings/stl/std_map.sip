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

template<T1, T2>
%MappedType std::map<T1, T2> /DocType="dict-of-T1-T2"/ {
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
	
	PyObject *key = NULL;
	PyObject *val = NULL;
	Py_ssize_t pos = 0;

    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return 0;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
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
		
        T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(key, kpTypeDef1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
        T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, kpTypeDef2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
		
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
        return NULL;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return NULL;
    }

	PyObject *pyDict = NULL;
	pyDict = PyDict_New();
	if (pyDict == NULL) {
		return NULL;
	}
	
	for (std::map<T1, T2>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
		PyObject *key = NULL;
		PyObject *val = NULL;
		T1 *cppKey = new T1(it->first);
		T2 *cppVal = new T2(it->second);
		
        key = sipConvertFromNewType(cppKey, kpTypeDef1, sipTransferObj);
        val = sipConvertFromNewType(cppVal, kpTypeDef2, sipTransferObj);
		
		if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
			Py_DECREF(pyDict);
			
			if (key) {
				Py_DECREF(key);
			} else {
				delete cppKey;
			}
			
			if (val != NULL) {
				Py_DECREF(val);
			} else {
				delete cppVal;
			}
			
			return NULL;
		}
		
		Py_DECREF(key);
		Py_DECREF(val);
	}
	
	return pyDict;
%End
};

template<T1, T2>
%MappedType std::map<T1, T2 *> /DocType="dict-of-T1-T2"/ {
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
	PyObject *key = NULL;
	PyObject *val = NULL;
	Py_ssize_t pos = 0;

    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return 0;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
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
		
        T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(key, kpTypeDef1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
        T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, kpTypeDef2, NULL, SIP_NOT_NONE, &t2State, sipIsErr));
		
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
        return NULL;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return NULL;
    }

	PyObject *pyDict = NULL;
	pyDict = PyDict_New();
	if (pyDict == NULL) {
		return NULL;
	}
	
	for (std::map<T1, T2 *>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
		PyObject *key = NULL;
		PyObject *val = NULL;
		T1 *cppKey = new T1(it->first);
		T2 *cppVal = it->second;
		
        key = sipConvertFromNewType(cppKey, kpTypeDef1, sipTransferObj);
        val = sipConvertFromType(cppVal, kpTypeDef2, sipTransferObj);
		
		if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
			Py_DECREF(pyDict);
			
			if (key) {
				Py_DECREF(key);
			} else {
				delete cppKey;
			}
			
			if (val != NULL) {
				Py_DECREF(val);
			}
			
			return NULL;
		}
		
		Py_DECREF(key);
		Py_DECREF(val);
	}
	
	return pyDict;
%End
};

template<T2>
%MappedType std::map<float, T2> /DocType="dict-of-float-T2"/{
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
	
	PyObject *key = NULL;
	PyObject *val = NULL;
	Py_ssize_t pos = 0;

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
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
        T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, kpTypeDef2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
		
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
        return NULL;
    }

	PyObject *pyDict = NULL;
	pyDict = PyDict_New();
	if (pyDict == NULL) {
		return NULL;
	}
	
	for (std::map<float, T2>::const_iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
		PyObject *key = NULL;
		PyObject *val = NULL;
		float cppKey = it->first;
		T2 *cppVal = new T2(it->second);
		
		key = PyFloat_FromDouble(cppKey);
        val = sipConvertFromNewType(cppVal, kpTypeDef2, sipTransferObj);
		
		if (key == NULL || val == NULL || PyDict_SetItem(pyDict, key, val) < 0) {
			Py_DECREF(pyDict);
			
			if (key) {
				Py_DECREF(key);
			} 
			
			if (val != NULL) {
				Py_DECREF(val);
			} else {
				delete cppVal;
			}
			
			return NULL;
		}
		
		Py_DECREF(key);
		Py_DECREF(val);
	}
	
	return pyDict;
%End
};

