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

template<TYPE>
%MappedType std::set<TYPE> /DocType="set-of-TYPE"/{
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   const sipTypeDef* kpTypeDef = getSipType("TYPE");

   if (!kpTypeDef) {
       return NULL;
   }

   // Create an empty Python set
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
   for (std::set<TYPE>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       TYPE *cpp = new TYPE(*it);
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromNewType(cpp, kpTypeDef, sipTransferObj)) == NULL) {
           // There was an error so garbage collect the Python set.
           Py_XDECREF(l);
           return NULL;
       }

       // Add the wrapper to the set.
       PySet_Add(l, pobj);
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode

   const sipTypeDef* kpTypeDef = getSipType("TYPE");

   if (!kpTypeDef) {
       return 0;
   }

   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;

       while (item = PyIter_Next(iterator)) {
           if (!sipCanConvertToType(item, kpTypeDef, SIP_NOT_NONE)) {
               //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
               Py_DECREF(item);
               Py_DECREF(iterator);
               return 0;
           }
           Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }
 
   // Convert Python set of TYPE to  std::set<TYPE>
   std::set<TYPE> *s = new std::set<TYPE>();
   
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
       int state;
       TYPE* p = reinterpret_cast<TYPE*>(sipConvertToType(item, kpTypeDef, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

       if (*sipIsErr) {
            Py_DECREF(item);
            Py_DECREF(iterator);
            sipReleaseType(p, kpTypeDef, state);
            delete s;
            return 0;
       }		

       s->insert(*p);
       Py_DECREF(item);
       sipReleaseType(p, kpTypeDef, state);
   }
   Py_DECREF(iterator);
   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End

};

template<TYPE*>
%MappedType std::set<TYPE*> /DocType="set-of-TYPE"/{
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   const sipTypeDef* kpTypeDef = getSipType("TYPE");

   if (!kpTypeDef) {
       return NULL;
   }

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
   for (std::set<TYPE*>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       TYPE *cpp = *it;
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromType(cpp, kpTypeDef, sipTransferObj)) == NULL) {
           // There was an error so garbage collect the Python set.
           Py_XDECREF(l);
           return NULL;
       }

       // Add the wrapper to the set.
       PySet_Add(l, pobj);
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode

   const sipTypeDef* kpTypeDef = getSipType("TYPE");

   if (!kpTypeDef) {
       return 0;
   }

   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
            if (!sipCanConvertToType(item, kpTypeDef, SIP_NOT_NONE)) {
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
            Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }
 
   // Convert Python list of TYPE to  std::set<TYPE>
   std::set<TYPE*> *s = new std::set<TYPE*>();
   
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
       int state;
       TYPE* p = reinterpret_cast<TYPE*>(sipConvertToType(item, kpTypeDef, NULL, SIP_NOT_NONE, &state, sipIsErr));

       if (*sipIsErr) {
            Py_DECREF(item);
            Py_DECREF(iterator);
            sipReleaseType(p, kpTypeDef, state);
            delete s;
            return 0;
       }		

       sipTransferTo(item, item);

       s->insert(p);
       Py_DECREF(item);
       sipReleaseType(p, kpTypeDef, state);
   }
   Py_DECREF(iterator);
   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<double> /DocType="set-of-double"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }    

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
    for (std::set<double>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PySet_Add(l, PyFloat_FromDouble(*it));
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
            if (!PyNumber_Check(item)) {
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to double");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
            Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }
 
   // Convert Python set of numbers to a std::set<double>
   std::set<double> *s = new std::set<double>();
   
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
       PyObject *f = PyNumber_Float(item);
       s->insert(PyFloat_AsDouble(f));
       Py_XDECREF(f);
       Py_DECREF(item);
   }
   Py_DECREF(iterator);

   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End   

};

%MappedType std::set<float> /DocType="set-of-float"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }    

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
    for (std::set<float>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PySet_Add(l, PyFloat_FromDouble(*it));
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
            if (!PyNumber_Check(item)) {
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to float");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
            Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }
 
   // Convert Python set of numbers to a std::set<float>
   std::set<float> *s = new std::set<float>();
   
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
       PyObject *f = PyNumber_Float(item);
       s->insert(static_cast<float>(PyFloat_AsDouble(f)));
       Py_XDECREF(f);
       Py_DECREF(item);
   }
   Py_DECREF(iterator);

   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End   

};

%MappedType std::set<int> /DocType="set-of-int"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
   for (std::set<int>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PySet_Add(l, PyLong_FromLong(*it));
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
            if (!PyLong_Check(item)) {
#else
            if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to int");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
            Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }
 
   // Convert Python set of integers to a std::set<int>
   std::set<int> *s = new std::set<int>();
   
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
       s->insert(static_cast<int>(PyLong_AsLong(item)));
#else
       if (PyLong_Check(item)) {
           s->insert(static_cast<int>(PyLong_AsLong(item)));
       } else {
           s->insert(static_cast<int>(PyInt_AsLong(item)));
       }
#endif
       Py_DECREF(item);
   }

   Py_DECREF(iterator);
   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<long> /DocType="set-of-long"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
   for (std::set<long>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PySet_Add(l, PyLong_FromLong(*it));
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
            if (!PyLong_Check(item)) {
#else
            if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to long");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
            Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }
 
   // Convert Python set of integers to a std::set<long>
   std::set<long> *s = new std::set<long>();
   
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
       s->insert(PyLong_AsLong(item));
#else
       if (PyLong_Check(item)) {
           s->insert(PyLong_AsLong(item));
       } else {
           s->insert(PyInt_AsLong(item));
       }
#endif
       Py_DECREF(item);
   }

   Py_DECREF(iterator);
   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<unsigned int> /DocType="set-of-uint"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
   for (std::set<unsigned int>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PySet_Add(l, PyLong_FromUnsignedLong(*it));
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
            if (!PyLong_Check(item)) {
#else
            if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to unsigned int");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
              Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }

   // Convert Python set of integers to a std::set<unsigned int>
   std::set<unsigned int> *s = new std::set<unsigned int>();

   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
       s->insert(static_cast<unsigned int>(PyLong_AsUnsignedLong(item)));
#else
       if (PyLong_Check(item)) {
           s->insert(static_cast<unsigned int>(PyLong_AsUnsignedLong(item)));
       } else {
           s->insert(static_cast<unsigned int>(PyInt_AsUnsignedLongMask(item)));
       }
#endif
       Py_DECREF(item);
   }
   Py_DECREF(iterator);
   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End

};

%MappedType std::set<unsigned long> /DocType="set-of-ulong"/ {
%TypeHeaderCode
#include <set>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create an empty Python set.
   if ((l = PySet_New(NULL)) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapper object.
   for (std::set<unsigned long>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PySet_Add(l, PyLong_FromUnsignedLong(*it));
   }

   // Return the Python set.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
       if (!PySet_Check(sipPy)) {
          return 0;
       }
       PyObject *iterator = PyObject_GetIter(sipPy);
       PyObject *item = NULL;
       while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
            if (!PyLong_Check(item)) {
#else
            if (!PyLong_Check(item) && !PyInt_Check(item)) {
#endif
                //printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to unsigned long");
                Py_DECREF(item);
                Py_DECREF(iterator);
                return 0;
            }
              Py_DECREF(item);
       }
       Py_DECREF(iterator);
       return 1;
   }

   // Convert Python set of integers to a std::set<unsigned long>
   std::set<unsigned long> *s = new std::set<unsigned long>();

   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item = NULL;
   while (item = PyIter_Next(iterator)) {
#if PY_MAJOR_VERSION >= 3
       s->insert(PyLong_AsUnsignedLong(item));
#else
       if (PyLong_Check(item)) {
           s->insert(PyLong_AsUnsignedLong(item));
       } else {
           s->insert(PyInt_AsUnsignedLongMask(item));
       }
#endif
       Py_DECREF(item);
   }
   Py_DECREF(iterator);
   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End

};
