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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

template<T1, T2>
%MappedType std::map<T1, T2>
{
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL) {
		return PyDict_Check(sipPy);
	}

	PyObject *key = NULL;
	PyObject *val = NULL;
	Py_ssize_t pos = 0;
	
	std::map <T1, T2> *map = new std::map<T1, T2>();
	
	while (PyDict_Next(sipPy, &pos, &key, &val)) {
		int t1State = 0;
		int t2State = 0;
		
		T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(key, sipType_T1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
		T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, sipType_T2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));
		
		if (*sipIsErr) {
			delete map;
			
			sipReleaseType(t1, sipType_T1, t1State);
			sipReleaseType(t2, sipType_T2, t2State);
			
			return 0;
		}
		
		(*map)[*t1] = *t2;
		
		sipReleaseType(t1, sipType_T1, t1State);
		sipReleaseType(t2, sipType_T2, t2State);
	}
	
	*sipCppPtr = map;
	return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
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
		
		key = sipConvertFromNewType(cppKey, sipType_T1, sipTransferObj);
		val = sipConvertFromNewType(cppVal, sipType_T2, sipTransferObj);
		
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
%MappedType std::map<T1, T2 *>
{
%TypeHeaderCode
#include <map>
%End

%ConvertToTypeCode
	if (sipIsErr == NULL) {
		return PyDict_Check(sipPy);
	}

	PyObject *key = NULL;
	PyObject *val = NULL;
	Py_ssize_t pos = 0;
	
	std::map <T1, T2 *> *map = new std::map<T1, T2 *>();
	
	while (PyDict_Next(sipPy, &pos, &key, &val)) {
		int t1State = 0;
		
		T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(key, sipType_T1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
		T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(val, sipType_T2, sipTransferObj, 0, 0, sipIsErr));
		
		if (*sipIsErr) {
			delete map;
			
			sipReleaseType(t1, sipType_T1, t1State);
			
			return 0;
		}
		
		(*map)[*t1] = t2;
		
		sipReleaseType(t1, sipType_T1, t1State);
	}
	
	*sipCppPtr = map;
	return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
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
		
		key = sipConvertFromNewType(cppKey, sipType_T1, sipTransferObj);
		val = sipConvertFromType(cppVal, sipType_T2, sipTransferObj);
		
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