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
%MappedType std::set<TYPE>
{
%TypeHeaderCode
#include <set>
%End


%ConvertToTypeCode
	PyObject *it = NULL;
	PyObject *itm = NULL;
	// Check the type if that is all that is required.
	if (sipIsErr == NULL)
	{
		it = PyObject_GetIter(sipPy);
		if (it == NULL) {
			return 0;
		}

		while ((itm = PyIter_Next(it))) {
			bool ok = (itm && sipCanConvertToType(itm, sipType_TYPE, SIP_NOT_NONE));

			Py_XDECREF(itm);

			if (!ok) {
				Py_XDECREF(it);
				return 0;
			}
		}

		Py_XDECREF(it);
		return 1;
	}

	it = PyObject_GetIter(sipPy);
	if (it == NULL) {
		return 0;
	}

	std::set<TYPE> *ql = new std::set<TYPE>;

	while ((itm = PyIter_Next(it)))
	{
		int state;
		TYPE *t = reinterpret_cast<TYPE *>(sipConvertToType(itm, sipType_TYPE, sipTransferObj, SIP_NOT_NONE, &state, sipIsErr));

		Py_DECREF(itm);

		if (*sipIsErr)
		{
			sipReleaseType(t, sipType_TYPE, state);
			Py_XDECREF(it);
			delete ql;
			return 0;
		}

		ql->insert(*t);

		sipReleaseType(t, sipType_TYPE, state);
	}
	Py_XDECREF(it);
	*sipCppPtr = ql;

	return sipGetState(sipTransferObj);
%End



%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped P2d.
   int i = 0;
   for (std::set<TYPE>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
   {
       TYPE *cpp = new TYPE(*it);
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
       PyList_SET_ITEM(l, i++, pobj);
   }

   // Return the Python list.
   return l;
%End

};



%MappedType std::set<double>
{
%TypeHeaderCode
#include <set>
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
   std::set<double> *V = new std::set<double>();
 
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
           V->insert(PyFloat_AsDouble(f));

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

%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   int i = 0;
    for (std::set<double>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
   {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, PyFloat_FromDouble(*it));
   }

   // Return the Python list.
   return l;
%End


};


%MappedType std::set<int>
{
%TypeHeaderCode
#include <set>
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
   std::set<int> *V = new std::set<int>();

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
           V->insert(val);

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


%ConvertFromTypeCode
   PyObject *l;

   // Create the Python list of the correct length.
   if ((l = PyList_New(sipCpp -> size())) == NULL)
       return NULL;

   // Go through each element in the C++ instance and convert it to a
   // wrapped object.
   int i = 0;
   for (std::set<int>::iterator it = sipCpp->begin(); it != sipCpp->end(); ++it)
   {
       // Add the wrapper to the list.
       PyList_SET_ITEM(l, i++, PyInt_FromLong(*it));
   }

   // Return the Python list.
   return l;
%End


};

