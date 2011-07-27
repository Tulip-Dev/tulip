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
class PropertyAlgorithm : tlp::WithParameter, tlp::WithDependency /Abstract/{
%TypeHeaderCode
#include <tulip/PropertyAlgorithm.h>
%End
public:

%Docstring
Bases: :class:`tlp.WithParameter`

This base class describes plugins who only modify one property, e.g. selection, layout, ...

.. rubric:: class attributes

.. py:attribute:: graph

	The :class:`tlp.Graph` this property algorithm will be run on.

.. py:attribute:: dataSet

	A :class:`tlp.DataSet` containing parameters for this property algorithm, if any.
	
.. py:attribute:: pluginProgress

	A :class:`tlp.PluginProgress` to give feedback to the user. It can be :const:`None`, so use with caution.	 

.. rubric:: class methods
%End

	PropertyAlgorithm(const tlp::PropertyContext& context);
	
	virtual ~PropertyAlgorithm();
	
	virtual bool run();
%Docstring
tlp.PropertyAlgorithm.run()

This method is called to run the property algorithm. Derived classes must implement it.
It is a good practice to report progress through the PluginProgress.
The PluginProgress should also be used to report errors, if any.
Must return a boolean to indicate if the execution was successful.

:rtype: boolean
%End

	virtual bool check(std::string & /Out/);
%Docstring
tlp.PropertyAlgorithm.check()

Checks whether the algorithm can be applied on this graph or not.
Derived classes can reimplement it.
Must return a tuple whose first member indicates if the algorithm can be applied
and the second one can be used to provide an error message.

:rtype: (boolean, string) 
%End	
	
	tlp::Graph *graph;
  	tlp::PluginProgress *pluginProgress;
 	tlp::DataSet *dataSet;

};
};
