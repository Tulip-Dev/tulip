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
public:
	unsigned int id;
	edge();
	edge(unsigned int);
	bool __eq__(const tlp::edge) const;
	bool __ne__(const tlp::edge) const;
	bool isValid() const;
	
	SIP_PYOBJECT __repr__() const;
%MethodCode
		std::ostringstream oss;
		oss << "<edge " << sipCpp->id << ">";
		std::string s = oss.str();
        sipRes = PyString_FromString(s.c_str());
%End

	long __hash__() const;
%MethodCode
        return static_cast<long>(sipCpp->id);
%End
};
};
