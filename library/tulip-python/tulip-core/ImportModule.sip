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
#include <tulip/ImportModule.h>
#include <tulip/PluginContext.h>.h>
%End

namespace tlp {

class ImportModule : tlp::Plugin {
%TypeHeaderCode
#include <tulip/ImportModule.h>
%End

public:

%Docstring
Bases: :class:`tlp.Plugin`

This abstract class describes an import plugin.

.. rubric:: class attributes

.. py:attribute:: graph

	An empty :class:`tlp.Graph` to populate when running the import process

.. py:attribute:: dataSet

	A :class:`tlp.DataSet` containing parameters for this import plugin, if any
	
.. py:attribute:: pluginProgress

	A :class:`tlp.PluginProgress` to give feedback to the user. It can be :const:`None`, so use with caution	 

.. rubric:: class methods

%End

    ImportModule (const tlp::PluginContext* context);
    virtual ~ImportModule();

    virtual std::string getGroup() const;

    virtual std::string category() const;

    virtual bool importGraph() = 0;
%Docstring
tlp.ImportModule.importGraph()

This method is called to import a graph.
Derived classes must implement it. A boolean must also be returned
to indicate if the import was successful.

:rtype: boolean
%End  
    
  tlp::Graph *graph;
  tlp::DataSet *dataSet;
  tlp::PluginProgress *pluginProgress;
};

};

