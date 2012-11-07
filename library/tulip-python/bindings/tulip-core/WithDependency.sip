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
%TypeHeaderCode
#include <tulip/WithDependency.h>
%End
struct Dependency {
	std::string pluginName;
	std::string pluginRelease;

  Dependency(std::string pName,
			std::string pRelease);
};

class WithDependency {
public:
	///
    const std::list<tlp::Dependency>& dependencies();
%Docstring
tlp.WithDependency.getDependencies() -> list of tlp.Dependency
%End
    void addDependency(const std::string & algorithm,const std::string & release );
%Docstring
tlp.WithParameter.addBooleanAlgorithmDependency(algorithm, release )

Add a dependency on a boolean algorithm.

:param name: the name of the algorithm
:type name: string
:param release: the version of the algortihm
:type release: string
%End

%MethodCode
    sipCpp->addDependency((*a0).c_str(), (*a1).c_str());
%End
};
};
