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
#include <tulip/ExportModule.h>
%End

namespace tlp {

class ExportModule: tlp::Plugin {
%TypeHeaderCode
#include <tulip/ExportModule.h>
%End
public:
  
%Docstring
Bases: :class:`tlp.WithParameter`

This abstract class describes an export plugin.

.. rubric:: class attributes

.. py:attribute:: graph

	The :class:`tlp.Graph` to export

.. py:attribute:: dataSet

	A :class:`tlp.DataSet` containing parameters for this export plugin, if any
	
.. py:attribute:: pluginProgress

	A :class:`tlp.PluginProgress` to give feedback to the user. It can be :const:`None`, so use with caution	 

.. rubric:: class methods

%End  
  
  ExportModule(const tlp::PluginContext* context);
  
  virtual ~ExportModule();

  virtual std::string fileExtension() const = 0;
  
  virtual std::string category() const;

  virtual bool exportGraph(std::ostream &os) = 0;
%Docstring
tlp.ExportModule.exportGraph(os)

This method is called to export the graph to a file.
Derived classes must implement it. 

The os parameter is an output file stream (initialized by the Tulip GUI
or by the :func:`tlp.exportGraph` function.). To write data to the file,
you have to use the following syntax::

	# write the number of nodes and edges to the file
	os << self.graph.numberOfNodes() << "\n"
	os << self.graph.numberOfEdges() << "\n"

A boolean must also be returned
to indicate if the export was successful.

:param os: an output file stream
:type os: std.ostream 
:rtype: boolean
%End 
  
  tlp::Graph *graph;
  tlp::PluginProgress *pluginProgress;
  tlp::DataSet *dataSet;
  
};

};
