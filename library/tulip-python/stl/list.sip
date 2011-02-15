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


// std::list
template<TYPE *>
%MappedType std::list<TYPE *>
{
%TypeHeaderCode
#include <list>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   int i = 0;
   for(std::list<TYPE *>::iterator iter = sipCpp->begin(); iter != sipCpp->end(); iter++) {
       TYPE *cpp = *iter;
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromType(cpp, sipType_TYPE, sipTransferObj)) == NULL)
       {
           // There was an error so garbage collect the Python list.
           Py_DECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, pobj);
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

   std::list<TYPE *> *V = new std::list<TYPE *>();
   
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
           V->push_back(p);

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

template<TYPE>
%MappedType std::list<TYPE>
{
%TypeHeaderCode
#include <list>
%End

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   int i = 0;
   for(std::list<TYPE>::iterator iter = sipCpp->begin(); iter != sipCpp->end(); iter++) {
       TYPE cpp = *iter;
       PyObject *pobj;

       // Get the Python wrapper for the Type instance, creating a new
       // one if necessary, and handle any ownership transfer.
       if ((pobj = sipConvertFromType(&cpp, sipType_TYPE, sipTransferObj)) == NULL)
       {
           // There was an error so garbage collect the Python list.
           Py_DECREF(l);
           return NULL;
       }

       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, pobj);
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

   std::list<TYPE> *V = new std::list<TYPE>();
   
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