/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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
class node {
%TypeHeaderCode
#include <tulip/Node.h>
%End

%Docstring
An instance of this class represent a node in a Tulip graph. It encapsulates an integer identifier.
Use specific methods in the :class:`tlp.Graph` class to get references on such objects (
for instance :meth:`tlp.Graph.addNode`, :meth:`tlp.Graph.getNodes`, :meth:`tlp.Graph.getOutNodes`, ...).
Node objects can be used as keys in a Python dictionnary.

The node identifier can be accessed through the **id** class member.
%End

public:
  unsigned int id;
  node();
  node(unsigned int);
  bool __eq__(const tlp::node);
  bool __ne__(const tlp::node);
  bool isValid();
%Docstring
tlp.node.isValid()

Returns :const:`True` if the node is a valid one, meaning it is attached to a graph.

:rtype: boolean 
%End

  std::string __repr__() const;
%MethodCode
  std::ostringstream oss;
  oss << "<node " << sipCpp->id << ">";
  sipRes = new std::string(oss.str());
%End
 
  long __hash__() const;
%MethodCode
  return static_cast<long>(sipCpp->id);
%End

  SIP_PYOBJECT __copy__() const;
%MethodCode
  sipRes = sipConvertFromNewType(new tlp::node(sipCpp->id), sipFindType("tlp::node"), NULL);
%End

  SIP_PYOBJECT __deepcopy__(SIP_PYOBJECT) const;
%MethodCode
  sipRes = sipConvertFromNewType(new tlp::node(sipCpp->id), sipFindType("tlp::node"), NULL);
%End

};
};
