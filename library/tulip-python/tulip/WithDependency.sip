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
	std::string factoryName;
	std::string pluginName;
	std::string pluginRelease;

	Dependency(std::string fName, std::string pName,
			std::string pRelease);
};

class WithDependency {
public:
	///
	std::list<tlp::Dependency> getDependencies();
%Docstring
tlp.WithDependency.getDependencies() -> list of tlp.Dependency
%End
	/** Indicates that the current algorithm depends on the named algorithm of type 'Ty'
	 *  which is supposing to have the parameters specified as second argument.
	 *  If non null the second argument needs to be a null terminated array of character strings.
	 */
	// template<typename Ty> void addDependency(const char* name, const char* release);
};
};
