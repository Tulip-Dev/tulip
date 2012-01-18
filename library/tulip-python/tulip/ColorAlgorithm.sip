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
#include <tulip/ColorAlgorithm.h>
#include <tulip/ColorProperty.h>
%End

namespace tlp {
class ColorAlgorithm : tlp::Algorithm {
%TypeHeaderCode
#include <tulip/ColorAlgorithm.h>
%End
public:

%Docstring
Bases: :class:`tlp.Algorithm`

This class is the base interface for color plugins.

.. rubric:: class attributes

.. py:attribute:: colorResult

	The result of the algorithm must be stored in this :class:`tlp.ColorProperty`.
%End

	virtual ~ColorAlgorithm();
	tlp::ColorProperty *colorResult;
protected:
	ColorAlgorithm(const tlp::AlgorithmContext &context);
	
};

};