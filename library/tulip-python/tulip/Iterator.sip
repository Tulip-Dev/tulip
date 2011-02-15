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

%ModuleHeaderCode
#include <tulip/Graph.h>
static void *getPointer(tlp::Graph *p) {
	return p;
}

static void *getPointer(tlp::node n) {
	return &n;
}

static void *getPointer(std::string s) {
	return &s;
}

static void *getPointer(tlp::edge e) {
	return &e;
}

%End

namespace tlp {

template<TYPE> 
class Iterator /Abstract/ {
%TypeHeaderCode
#include <tulip/Iterator.h>
%End
public:

	Iterator();

	TYPE next();

	bool hasNext();	

	 Iterator * __iter__();
%MethodCode
    sipRes = sipCpp;
%End

    SIP_PYOBJECT __next__();
%MethodCode
	#define sipType_tlp_GraphPointer sipType_tlp_Graph
    if (sipCpp->hasNext()) {
    	if ("TYPE" == "tlp::GraphPointer") {
    		sipRes = sipConvertFromType(getPointer(sipCpp->next()), sipType_TYPE, NULL);
    	} else {
    		sipRes = sipConvertFromNewType(new TYPE(sipCpp->next()), sipType_TYPE, NULL);
    	}
    } else {
        PyErr_SetNone(PyExc_StopIteration);
    }
%End

};



};

