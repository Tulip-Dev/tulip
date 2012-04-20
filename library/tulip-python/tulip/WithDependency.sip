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

    // ==================================================================================

        void addBooleanAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addBooleanAlgorithmDependency(algorithm, release )

    Add a dependency on a boolean algorithm.

    :param name: the name of the algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::BooleanAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End


    // ==================================================================================

        void addColorAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addColorAlgorithmDependency(algorithm, release )

    Add a dependency on a color algorithm.

    :param name: the name of the algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::ColorAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End

    // ==================================================================================

        void addDoubleAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addDoubleAlgorithmDependency(algorithm, release )

    Add a dependency on a double algorithm.

    :param name: the name of the algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::DoubleAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End

    // ==================================================================================

        void addIntegerAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addIntegerAlgorithmDependency(algorithm, release )

    Add a dependency on an integer algorithm.

    :param name: the name of the algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::IntegerAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End
    // ==================================================================================

        void addLayoutAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addLayoutAlgorithmDependency(algorithm, release )

    Add a dependency on a layout algorithm.

    :param name: the name of the layout algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::LayoutAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End
    // ==================================================================================

        void addSizeAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addSizeAlgorithmDependency(algorithm, release )

    Add a dependency on a size algorithm.

    :param name: the name of the layout algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::SizeAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End

    // ==================================================================================

        void addStringAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addStringAlgorithmDependency(algorithm, release )

    Add a dependency on a string algorithm.

    :param name: the name of the layout algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::StringAlgorithm>((*a0).c_str(), (*a1).c_str());
    %End

    // ==================================================================================

        void addAlgorithmDependency(const std::string & algorithm,
                const std::string & release );
    %Docstring
    tlp.WithParameter.addAlgorithmDependency(algorithm, release )

    Add a dependency on an algorithm.

    :param name: the name of the layout algorithm
    :type name: string
    :param release: the version of the algortihm
    :type release: string
    %End

    %MethodCode
        sipCpp->addDependency<tlp::Algorithm>((*a0).c_str(), (*a1).c_str());
    %End
};
};
