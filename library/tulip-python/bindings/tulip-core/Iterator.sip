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
    sipRes = copyValue(sipCpp->next());
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
    sipRes = copyValue(sipCpp->next());
  } else {
    PyErr_SetNone(PyExc_StopIteration);
    return NULL;
  }
%End

};

};
