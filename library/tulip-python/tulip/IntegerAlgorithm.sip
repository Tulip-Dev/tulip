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
#include <tulip/IntegerAlgorithm.h>
#include <tulip/IntegerProperty.h>
%End

namespace tlp {
class IntegerAlgorithm : tlp::Algorithm {
%TypeHeaderCode
#include <tulip/IntegerAlgorithm.h>
%End
public:

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for integer plugins.

.. rubric:: class attributes

.. py:attribute:: integerResult

	The result of the algorithm must be stored in this :class:`tlp.IntegerProperty`.
%End

	virtual ~IntegerAlgorithm();
	tlp::IntegerProperty *integerResult;
protected:
	IntegerAlgorithm(const tlp::AlgorithmContext &context);
	
};

};