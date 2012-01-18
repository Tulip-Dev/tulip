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
#include <tulip/SizeAlgorithm.h>
#include <tulip/SizeProperty.h>
%End

namespace tlp {
class SizeAlgorithm : tlp::Algorithm {
%TypeHeaderCode
#include <tulip/SizeAlgorithm.h>
%End
public:

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for size plugins.

.. rubric:: class attributes

.. py:attribute:: sizeResult

	The result of the algorithm must be stored in this :class:`tlp.SizeProperty`.
%End

	virtual ~SizeAlgorithm();
	tlp::SizeProperty *sizeResult;
protected:
	SizeAlgorithm(const tlp::AlgorithmContext &context);
	
};

};