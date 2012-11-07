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
class Algorithm : tlp::Plugin {
%TypeHeaderCode
#include <tulip/Algorithm.h>
%End
public:

%Docstring
Bases: :class:`tlp.Plugin`

This abstract class describes a general algorithm plugin.
Basic functionality consists in checking the algorithm can run on the current Graph 
(e.g. is the graph simple ?) then running the algorithm.
The algorithm can and should report progress and which task it is performing 
if it is decomposed in multiple phases (e.g. layouting the graph, coloring it, ...).

.. rubric:: class attributes

.. py:attribute:: graph

	The :class:`tlp.Graph` this algorithm will be run on.

.. py:attribute:: dataSet

	A :class:`tlp.DataSet` containing parameters for this algorithm, if any.
	
.. py:attribute:: pluginProgress

	A :class:`tlp.PluginProgress` to give feedback to the user. It can be :const:`None`, so use with caution.	 

.. rubric:: class methods

%End

    Algorithm(const tlp::PluginContext* context);

	virtual ~Algorithm();

	virtual bool run() = 0;
%Docstring
tlp.Algorithm.run()

This method is the entry point of the algorithm when it is called.
Derived classes must implement it. It is a good practice to report progress through the PluginProgress.
The PluginProgress should also be used to report errors, if any. A boolean must also be returned
to indicate if the algorithm was successful.

:rtype: boolean	
%End

	virtual bool check(std::string & /Out/);
%Docstring
tlp.Algorithm.check()

This method is called before applying the algorithm on the input graph.
You can perform some precondition checks here. Derived classes can reimplement it.
Must return a tuple whose first member indicates if the algorithm can be applied
and the second one can be used to provide an error message

:rtype: (boolean, string)
%End

    virtual std::string category() const;

	tlp::Graph *graph;
 	tlp::PluginProgress *pluginProgress;
  	tlp::DataSet *dataSet;
	
};
};

