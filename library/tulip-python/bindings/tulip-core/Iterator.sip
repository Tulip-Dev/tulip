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

namespace tlp {

template<TYPE> 
class Iterator /Abstract/ {
%TypeHeaderCode
#include <tulip/Iterator.h>
%End
public:

	Iterator();

	TYPE next();
%MethodCode
    if (sipCpp->hasNext()) {
// if iterator is on base types (int, double, pointers, ...),
// ITERATOR_NO_COPY has to be defined when compiling the generated bindings file    
#ifdef ITERATOR_NO_COPY
    	sipRes = sipCpp->next();
#else
    	sipRes = new TYPE(sipCpp->next());
#endif
    } else {
        PyErr_SetString(PyExc_Exception, "Iterator : next() has been called but there is no more elements to iterate on.");
        sipIsErr = -1;
    }
%End

	bool hasNext();	

	 Iterator * __iter__();
%MethodCode
    sipRes = sipCpp;
%End

    TYPE __next__();
%MethodCode
    if (sipCpp->hasNext()) {
// if iterator is on base types (int, double, pointers, ...),
// ITERATOR_NO_COPY has to be defined when compiling the generated bindings file    
#ifdef ITERATOR_NO_COPY
    	sipRes = sipCpp->next();
#else
    	sipRes = new TYPE(sipCpp->next());
#endif
    } else {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
%End



};

};
