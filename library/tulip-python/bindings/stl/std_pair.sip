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
%MappedType std::pair<T1, T2> /DocType="(T1, T2)"/{
%TypeHeaderCode
#include <utility>
%End

%ConvertToTypeCode
	PyObject *itm1 = NULL;
	PyObject *itm2 = NULL;
	
    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return 0;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
		if (!(PyTuple_Check(sipPy) && PyTuple_Size(sipPy) == 2))
			return 0;
	
		itm1 = PyTuple_GetItem(sipPy, 0);
		itm2 = PyTuple_GetItem(sipPy, 1);
	
        bool ok = sipCanConvertToType(itm1, kpTypeDef1, SIP_NOT_NONE);
        ok = ok && sipCanConvertToType(itm2, kpTypeDef2, SIP_NOT_NONE);
				
		if (!ok) {
			return 0;
		}
		
		return 1;
	}

	itm1 = PyTuple_GetItem(sipPy, 0);
	itm2 = PyTuple_GetItem(sipPy, 1);
	
	int t1State = 0;
	int t2State = 0;
	
    T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(itm1, kpTypeDef1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(itm2, kpTypeDef2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));

	if (*sipIsErr) {
        sipReleaseType(t1, kpTypeDef1, t1State);
        sipReleaseType(t2, kpTypeDef2, t2State);
		return 0;
	}

	std::pair<T1, T2> *p = new std::pair<T1, T2>(*t1, *t2);
	
    sipReleaseType(t1, kpTypeDef1, t1State);
    sipReleaseType(t2, kpTypeDef2, t2State);

	*sipCppPtr = p;
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

	PyObject *pyTuple = PyTuple_New(2);
	
	if (pyTuple == NULL) {
        return NULL;
	}
	
	PyObject *pyFirst = NULL;
	PyObject *pySecond = NULL;
	
	T1 *first = new T1(sipCpp->first);
	T2 *second = new T2(sipCpp->second);
	
    pyFirst = sipConvertFromNewType(first, kpTypeDef1, sipTransferObj);
    pySecond = sipConvertFromNewType(second, kpTypeDef2, sipTransferObj);
	
	if (pyFirst == NULL || pySecond == NULL || PyTuple_SetItem(pyTuple, 0, pyFirst)) {
		if (pyFirst) 
			Py_XDECREF(pyFirst);
		else
			delete first;
		if (pySecond)
			Py_XDECREF(pySecond);
		else
			delete second;
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	if (PyTuple_SetItem(pyTuple, 1, pySecond)) {
		Py_XDECREF(pySecond);
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	return pyTuple;
%End
};

template<T1*, T2*>
%MappedType std::pair<T1*, T2*> {
%TypeHeaderCode
#include <utility>
%End

%ConvertToTypeCode
	PyObject *itm1 = NULL;
	PyObject *itm2 = NULL;
	
    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return 0;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
		if (!(PyTuple_Check(sipPy) && PyTuple_Size(sipPy) == 2))
			return 0;
	
		itm1 = PyTuple_GetItem(sipPy, 0);
		itm2 = PyTuple_GetItem(sipPy, 1);
	
        bool ok = sipCanConvertToType(itm1, kpTypeDef1, SIP_NOT_NONE);
        ok = ok && sipCanConvertToType(itm2, kpTypeDef2, SIP_NOT_NONE);
			
		if (!ok) {
			return 0;
		}
		
		return 1;
	}

	itm1 = PyTuple_GetItem(sipPy, 0);
	itm2 = PyTuple_GetItem(sipPy, 1);
	
	int t1State = 0;
	int t2State = 0;
	
    T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(itm1, kpTypeDef1, NULL, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(itm2, kpTypeDef2, NULL, SIP_NOT_NONE, &t2State, sipIsErr));

	if (*sipIsErr) {
        sipReleaseType(t1, kpTypeDef1, t1State);
        sipReleaseType(t2, kpTypeDef2, t2State);
		return 0;
	}

    sipTransferTo(itm1, itm1);
    sipTransferTo(itm2, itm2);

	std::pair<T1*, T2*> *p = new std::pair<T1*, T2*>(t1, t2);
	
    sipReleaseType(t1, kpTypeDef1, t1State);
    sipReleaseType(t2, kpTypeDef2, t2State);

	*sipCppPtr = p;
	return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
	PyObject *pyTuple = PyTuple_New(2);

    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return NULL;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return NULL;
    }
	
	if (pyTuple == NULL) {
        return NULL;
	}
	
	PyObject *pyFirst = NULL;
	PyObject *pySecond = NULL;
	
	T1 *first = sipCpp->first;
	T2 *second = sipCpp->second;
	
    pyFirst = sipConvertFromType(first, kpTypeDef1, sipTransferObj);
    pySecond = sipConvertFromType(second, kpTypeDef2, sipTransferObj);
	
	if (pyFirst == NULL || pySecond == NULL || PyTuple_SetItem(pyTuple, 0, pyFirst)) {
		if (pyFirst) 
			Py_XDECREF(pyFirst);
		if (pySecond)
			Py_XDECREF(pySecond);
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	if (PyTuple_SetItem(pyTuple, 1, pySecond)) {
		Py_XDECREF(pySecond);
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	return pyTuple;
%End
};

template<T1*, T2>
%MappedType std::pair<T1*, T2> {
%TypeHeaderCode
#include <utility>
%End

%ConvertToTypeCode
	PyObject *itm1 = NULL;	
	PyObject *itm2 = NULL;

    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return 0;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
		if (!(PyTuple_Check(sipPy) && PyTuple_Size(sipPy) == 2))
			return 0;
	
		itm1 = PyTuple_GetItem(sipPy, 0);
		itm2 = PyTuple_GetItem(sipPy, 1);
	
        bool ok = sipCanConvertToType(itm1, kpTypeDef1, SIP_NOT_NONE);
        ok = ok && sipCanConvertToType(itm2, kpTypeDef2, SIP_NOT_NONE);
		
		if (!ok) {
			return 0;
		}
		
		return 1;
	}

	itm1 = PyTuple_GetItem(sipPy, 0);
	itm2 = PyTuple_GetItem(sipPy, 1);
	
	int t1State = 0;
	int t2State = 0;
	
    T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(itm1, kpTypeDef1, NULL, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(itm2, kpTypeDef2, sipTransferObj, SIP_NOT_NONE, &t2State, sipIsErr));

	if (*sipIsErr) {
        sipReleaseType(t1, kpTypeDef1, t1State);
        sipReleaseType(t2, kpTypeDef2, t2State);
		return 0;
	}

    sipTransferTo(itm1, itm1);

	std::pair<T1*, T2> *p = new std::pair<T1*, T2>(t1, *t2);
	
    sipReleaseType(t1, kpTypeDef1, t1State);
    sipReleaseType(t2, kpTypeDef2, t2State);

	*sipCppPtr = p;
	return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
	PyObject *pyTuple = PyTuple_New(2);
	
    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return NULL;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return NULL;
    }

	if (pyTuple == NULL) {
        return NULL;
	}
	
	PyObject *pyFirst = NULL;
	PyObject *pySecond = NULL;
	
	T1 *first = sipCpp->first;
	T2 *second = new T2(sipCpp->second);
	
    pyFirst = sipConvertFromType(first, kpTypeDef1, sipTransferObj);
    pySecond = sipConvertFromNewType(second, kpTypeDef2, sipTransferObj);
	
	if (pyFirst == NULL || pySecond == NULL || PyTuple_SetItem(pyTuple, 0, pyFirst)) {
		if (pyFirst) 
			Py_XDECREF(pyFirst);
		if (pySecond)
			Py_XDECREF(pySecond);
		else
			delete second;
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	if (PyTuple_SetItem(pyTuple, 1, pySecond)) {
		Py_XDECREF(pySecond);
		Py_XDECREF(pyTuple);
        return NULL;
	}
	
	return pyTuple;
%End
};

template<T1, T2*>
%MappedType std::pair<T1, T2*> {
%TypeHeaderCode
#include <utility>
%End

%ConvertToTypeCode
	PyObject *itm1 = NULL;
	PyObject *itm2 = NULL;
	
    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return 0;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return 0;
    }

	if (sipIsErr == NULL) {
		if (!(PyTuple_Check(sipPy) && PyTuple_Size(sipPy) == 2))
			return 0;
	
		itm1 = PyTuple_GetItem(sipPy, 0);
		itm2 = PyTuple_GetItem(sipPy, 1);
	
        bool ok = sipCanConvertToType(itm1, kpTypeDef1, SIP_NOT_NONE);
        ok = ok && sipCanConvertToType(itm2, kpTypeDef2, SIP_NOT_NONE);
			
		if (!ok) {
			return 0;
		}
		
		return 1;
	}

	itm1 = PyTuple_GetItem(sipPy, 0);
	itm2 = PyTuple_GetItem(sipPy, 1);
	
	int t1State = 0;
	int t2State = 0;
	
    T1 *t1 = reinterpret_cast<T1 *>(sipConvertToType(itm1, kpTypeDef1, sipTransferObj, SIP_NOT_NONE, &t1State, sipIsErr));
    T2 *t2 = reinterpret_cast<T2 *>(sipConvertToType(itm2, kpTypeDef2, NULL, SIP_NOT_NONE, &t2State, sipIsErr));

	if (*sipIsErr) {
        sipReleaseType(t1, kpTypeDef1, t1State);
        sipReleaseType(t2, kpTypeDef2, t2State);
		return 0;
	}

    sipTransferTo(itm2, itm2);

	std::pair<T1, T2*> *p = new std::pair<T1, T2*>(*t1, t2);
	
    sipReleaseType(t1, kpTypeDef1, t1State);
    sipReleaseType(t2, kpTypeDef2, t2State);

	*sipCppPtr = p;
	return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
	PyObject *pyTuple = PyTuple_New(2);
	
    const sipTypeDef* kpTypeDef1 = getSipType("T1");

    if (!kpTypeDef1) {
        return NULL;
    }

    const sipTypeDef* kpTypeDef2 = getSipType("T2");

    if (!kpTypeDef2) {
        return NULL;
    }

	if (pyTuple == NULL) {
        return NULL;
	}
	
	PyObject *pyFirst = NULL;
	PyObject *pySecond = NULL;
	
	T1 *first = new T1(sipCpp->first);
	T2 *second = sipCpp->second;
	
    pyFirst = sipConvertFromNewType(first, kpTypeDef1, sipTransferObj);
    pySecond = sipConvertFromType(second, kpTypeDef2, sipTransferObj);
	
	if (pyFirst == NULL || pySecond == NULL || PyTuple_SetItem(pyTuple, 0, pyFirst)) {
		if (pyFirst) 
			Py_XDECREF(pyFirst);
		else
			delete first;
		if (pySecond)
			Py_XDECREF(pySecond);
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	if (PyTuple_SetItem(pyTuple, 1, pySecond)) {
		Py_XDECREF(pySecond);
		Py_XDECREF(pyTuple);
		
        return NULL;
	}
	
	return pyTuple;
%End
};

%MappedType std::pair<unsigned int, unsigned int> /DocType="(uint, uint)"/{
%TypeHeaderCode
#include <utility>
%End

%ConvertToTypeCode
    PyObject *itm1 = NULL;
    PyObject *itm2 = NULL;

    if (sipIsErr == NULL) {
        if (!(PyTuple_Check(sipPy) && PyTuple_Size(sipPy) == 2))
            return 0;

        itm1 = PyTuple_GetItem(sipPy, 0);
        itm2 = PyTuple_GetItem(sipPy, 1);


#if PY_MAJOR_VERSION >= 3
        bool ok = PyLong_Check(itm1);
        ok = ok && PyLong_Check(itm2);
#else
        bool ok = (PyLong_Check(itm1) || PyInt_Check(itm1));
        ok = ok && (PyLong_Check(itm2) || PyInt_Check(itm2));
#endif

        if (!ok) {
            return 0;
        }

        return 1;
    }

    itm1 = PyTuple_GetItem(sipPy, 0);
    itm2 = PyTuple_GetItem(sipPy, 1);

    int t1State = 0;
    int t2State = 0;

    unsigned int ui1 = 0;
    unsigned int ui2 = 0;

#if PY_MAJOR_VERSION >= 3
    ui1 = PyLong_AsUnsignedLong(itm1);
    ui2 = PyLong_AsUnsignedLong(itm2);
#else
    if (PyLong_Check(itm1)) {
        ui1 = PyLong_AsUnsignedLong(itm1);
    } else {
        ui1 = PyInt_AsUnsignedLongMask(itm1);
    }
    if (PyLong_Check(itm2)) {
        ui2 = PyLong_AsUnsignedLong(itm2);
    } else {
        ui2 = PyInt_AsUnsignedLongMask(itm2);
    }
#endif

    std::pair<unsigned int, unsigned int> *p = new std::pair<unsigned int, unsigned int>(ui1, ui2);

    *sipCppPtr = p;
    return sipGetState(sipTransferObj);
%End

%ConvertFromTypeCode
    PyObject *pyTuple = PyTuple_New(2);

    if (pyTuple == NULL) {
        return 0;
    }

    PyObject *pyFirst = NULL;
    PyObject *pySecond = NULL;

    pyFirst = PyLong_FromUnsignedLong(sipCpp->first);
    pySecond = PyLong_FromUnsignedLong(sipCpp->second);

    if (pyFirst == NULL || pySecond == NULL || PyTuple_SetItem(pyTuple, 0, pyFirst)) {
        if (pyFirst)
            Py_XDECREF(pyFirst);

        if (pySecond)
            Py_XDECREF(pySecond);

        Py_XDECREF(pyTuple);

        return 0;
    }

    if (PyTuple_SetItem(pyTuple, 1, pySecond)) {
        Py_XDECREF(pySecond);
        Py_XDECREF(pyTuple);

        return 0;
    }

    return pyTuple;
%End
};

