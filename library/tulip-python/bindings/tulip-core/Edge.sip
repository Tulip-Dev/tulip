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
class edge {

%TypeHeaderCode
#include <tulip/tulipconf.h>
#include <tulip/Edge.h>
%End

%Docstring
An instance of this class represent an edge in a Tulip graph. It encapsulates an integer identifier.
Use specific methods in the :class:`tlp.Graph` class to get references on such objects (
for instance :meth:`tlp.Graph.addEdge`, :meth:`tlp.Graph.getEdges`, :meth:`tlp.Graph.getInEdges`, ...).
Edge objects can be used as keys in a Python dictionnary.

The edge identifier can be accessed through the **id** class member.
%End

public:
    unsigned int id;
    edge();
	edge(unsigned int);
    bool __eq__(const tlp::edge) const;
    bool __ne__(const tlp::edge) const;
    bool isValid() const;
%Docstring
tlp.edge.isValid()

Returns :const:`True` if the edge is a valid one, meaning it is attached to a graph.

:rtype: boolean 
%End
	
    SIP_PYOBJECT __repr__() const;
%MethodCode
		std::ostringstream oss;
		oss << "<edge " << sipCpp->id << ">";
        std::string s = oss.str();
#if PY_MAJOR_VERSION >= 3
        sipRes = PyUnicode_FromString(s.c_str());
#else
        sipRes = PyString_FromString(s.c_str());
#endif
%End

    long __hash__() const;
%MethodCode
        return static_cast<long>(sipCpp->id);
%End
};
};
