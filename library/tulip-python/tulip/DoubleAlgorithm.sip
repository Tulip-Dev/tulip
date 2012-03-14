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

%ModuleHeaderCode
#include <tulip/TemplateFactory.h>
#include <tulip/DoubleAlgorithm.h>
#include <tulip/DoubleProperty.h>
%End


namespace tlp {

class DoubleAlgorithm: tlp::Algorithm {
%TypeHeaderCode
#include <tulip/DoubleAlgorithm.h>
%End

public:

// Setting docstrings in that class binding makes SIP failed to generate a compilable source code (bug ?)

	virtual ~DoubleAlgorithm();
	tlp::DoubleProperty *doubleResult;

protected:

	DoubleAlgorithm(const tlp::AlgorithmContext &context);
	
};

};
