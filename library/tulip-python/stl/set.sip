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
// | inspired for bindings by the Booggie project development team           |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

template<TYPE>
%MappedType std::set<TYPE> {
%TypeHeaderCode
#include <set>
%End


%ConvertToTypeCode

   // Check if type is compatible
   if (sipIsErr == NULL) {
   	   if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!sipCanConvertToType(item, sipType_TYPE, SIP_NOT_NONE)) {
                printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to TYPE");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of TYPE to  std::set<TYPE>
   std::set<TYPE> *s = new std::set<TYPE>();
   
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       int state;
       TYPE* p = reinterpret_cast<TYPE*>(sipConvertToType(PyList_GET_ITEM(sipPy, i), sipType_TYPE, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

	   if (*sipIsErr) {
          sipReleaseType(p, sipType_TYPE, state);
          delete s;
          return 0;
       }		

       s->insert(*p);

       sipReleaseType(p, sipType_TYPE, state);       
   }

   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End



%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped P2d.
   int i = 0;
   for (std::set<TYPE>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       TYPE *cpp = new TYPE(*it);
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromNewType(cpp, sipType_TYPE, sipTransferObj)) == NULL) {
           // There was an error so garbage collect the Python list.
           Py_XDECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, pobj);
   }

   // Return the Python list.
   return l;
%End

};

%MappedType std::set<double> {
%TypeHeaderCode
#include <set>
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
   	   if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!PyNumber_Check(item)) {
                printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to double");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of numbers to a std::set<double>
   std::set<double> *s = new std::set<double>();
   
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       PyObject *f = PyNumber_Float(PyList_GET_ITEM(sipPy, i));
       s->insert(PyFloat_AsDouble(f));
       Py_XDECREF(f);       
   }

   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End   

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }    

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   int i = 0;
    for (std::set<double>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, PyFloat_FromDouble(*it));
   }

   // Return the Python list.
   return l;
%End


};


%MappedType std::set<int> {
%TypeHeaderCode
#include <set>
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL) {
   	   if (!PyList_Check(sipPy)) {
          return 0;
       }
       for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
            PyObject *item = PyList_GET_ITEM(sipPy, i);
            if (!PyInt_Check(item)) {
                printErrorMessage("TypeError : object in list of type " + std::string(item->ob_type->tp_name) + " can not be converted to int");
                return 0;
            }
       }
       return 1;
   }
 
   // Convert Python list of integers to a std::set<int>
   std::set<int> *s = new std::set<int>();
   
   for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
       s->insert(PyInt_AsLong(PyList_GET_ITEM(sipPy, i)));
   }

   *sipCppPtr = s;
   return sipGetState(sipTransferObj);
%End


%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL) {
       return NULL;
   }

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   int i = 0;
   for (std::set<int>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it) {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, PyInt_FromLong(*it));
   }

   // Return the Python list.
   return l;
%End


};

