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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert                              |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

// SIP support for std::vector
// by Giovanni Bajo <rasky@develer.com>
// Public domain


// ****************************************************
// SIP generic implementation for std::vector<>
// ****************************************************
// ALas, this template-based generic implementation is valid only
// if the element type is a SIP-wrapped type. For basic types (int, double, etc.)
// we are forced to cut & paste to provide a specialization.

template<TYPE>
%MappedType std::vector<TYPE>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped P2d.
   for (int i = 0; i < (int)sipCpp -> size(); ++i)
   {
       TYPE *cpp = new TYPE(sipCpp -> at(i));
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromNewType(cpp, sipType_TYPE, sipTransferObj)) == NULL)
       {
           // There was an error so garbage collect the Python list.
           Py_DECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, pobj);
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL)
   {
       // Must be any iterable
       PyObject *i = PyObject_GetIter(sipPy);
       bool iterable = (i != NULL);
       Py_XDECREF(i);
       return iterable;
   }

   // Iterate over the object
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item;

   std::vector<TYPE> *V = new std::vector<TYPE>();

   while ((item = PyIter_Next(iterator)))
   {
       if (!sipCanConvertToType(item, sipType_TYPE, SIP_NOT_NONE))
       {
           PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
           *sipIsErr = 1;
           break;
       }

       int state;
       TYPE* p = reinterpret_cast<TYPE*>(
            sipConvertToType(item, sipType_TYPE, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

       if (!*sipIsErr)
           V->push_back(*p);

       sipReleaseType(p, sipType_TYPE, state);
       Py_DECREF(item);
   }

   Py_DECREF(iterator);

   if (*sipIsErr)
   {
       delete V;
       return 0;
   }

   *sipCppPtr = V;
   return sipGetState(sipTransferObj);
%End
};

// ****************************************************
// Specialization for std::vector<double>
// ****************************************************

%MappedType std::vector<double>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i)
   {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyFloat_FromDouble(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL)
   {
       // Must be any iterable
       PyObject *i = PyObject_GetIter(sipPy);
       bool iterable = (i != NULL);
       Py_XDECREF(i);
       return iterable;
   }

   // Iterate over the object
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item;

   // Maximum number of elements
   int len = PyObject_Size(sipPy);
   std::vector<double> *V = new std::vector<double>();
   V->reserve(len);

   if (len)
   {
       while ((item = PyIter_Next(iterator)))
       {
           if (!PyNumber_Check(item))
           {
               PyErr_Format(PyExc_TypeError, "object in iterable is not a number");
               *sipIsErr = 1;
               break;
           }

           PyObject *f = PyNumber_Float(item);
           V->push_back(PyFloat_AsDouble(f));

           Py_DECREF(f);
           Py_DECREF(item);
       }

       Py_DECREF(iterator);

       if (*sipIsErr)
       {
           delete V;
           return 0;
       }
   }

   *sipCppPtr = V;
   return sipGetState(sipTransferObj);
%End
};


// ****************************************************
// Specialization for std::vector<int>
// ****************************************************

%MappedType std::vector<int>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i)
   {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyInt_FromLong(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL)
   {
       // Must be any iterable
       PyObject *i = PyObject_GetIter(sipPy);
       bool iterable = (i != NULL);
       Py_XDECREF(i);
       return iterable;
   }

   // Iterate over the object
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item;

   // Maximum number of elements
   int len = PyObject_Size(sipPy);
   std::vector<int> *V = new std::vector<int>();
   V->reserve(len);

   if (len)
   {
       while ((item = PyIter_Next(iterator)))
       {
           if (!PyInt_Check(item))
           {
               PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to float");
               *sipIsErr = 1;
               break;
           }

           int val = PyInt_AsLong(item);
           V->push_back(val);

           Py_DECREF(item);
       }

       Py_DECREF(iterator);

       if (*sipIsErr)
       {
           delete V;
           return 0;
       }
   }

   *sipCppPtr = V;
   return sipGetState(sipTransferObj);
%End
};

// ****************************************************
// Specialization for std::vector<bool>
// ****************************************************

%MappedType std::vector<bool>
{
%TypeHeaderCode
#include <vector>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   for (int i = 0; i < (int)sipCpp -> size(); ++i)
   {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i, PyBool_FromLong(sipCpp -> at(i)));
   }

   // Return the Python list.
   return l;
%End

%ConvertToTypeCode
   // Check if type is compatible
   if (sipIsErr == NULL)
   {
       // Must be any iterable
       PyObject *i = PyObject_GetIter(sipPy);
       bool iterable = (i != NULL);
       Py_XDECREF(i);
       return iterable;
   }

   // Iterate over the object
   PyObject *iterator = PyObject_GetIter(sipPy);
   PyObject *item;

   // Maximum number of elements
   int len = PyObject_Size(sipPy);
   std::vector<bool> *V = new std::vector<bool>();
   V->reserve(len);

   if (len)
   {
       while ((item = PyIter_Next(iterator)))
       {
           if (!PyBool_Check(item))
           {
               PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to bool");
               *sipIsErr = 1;
               break;
           }

           V->push_back(item == Py_True);

           Py_DECREF(item);
       }

       Py_DECREF(iterator);

       if (*sipIsErr)
       {
           delete V;
           return 0;
       }
   }

   *sipCppPtr = V;
   return sipGetState(sipTransferObj);
%End
};