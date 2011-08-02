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
namespace tlp {
class PythonExportModule : public ExportModule {
public :
	PythonExportModule(AlgorithmContext context) : 
		ExportModule(context){}
	
	bool exportGraph(std::ostream &,Graph *) {
		return exportGraph();
	}
	
	virtual bool exportGraph() = 0;
	
};
typedef tlp::FactoryInterface<tlp::ExportModule, tlp::AlgorithmContext> ExportModuleFactory;
}
%End

namespace tlp {

class ExportModule: tlp::WithParameter, tlp::WithDependency /Abstract/ {
%TypeHeaderCode
#include <tulip/ExportModule.h>
%End
public:
  
  ExportModule(tlp::AlgorithmContext context);
  
  virtual ~ExportModule();
  
  tlp::Graph *graph;
  tlp::PluginProgress *pluginProgress;
  tlp::DataSet *dataSet;
  
};

class PythonExportModule : tlp::ExportModule {
public :

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

	PythonExportModule(tlp::AlgorithmContext context);
		
	virtual bool exportGraph() = 0;
%Docstring
tlp.PythonExportModule.exportGraph()

This method is called to export the graph.
Derived classes must implement it. A boolean must also be returned
to indicate if the export was successful.

:rtype: boolean
%End 
	
};

class ExportModuleFactory : tlp::AbstractPluginInfo {
%TypeHeaderCode
#include <tulip/ExportModule.h>
%End
public:
      
  virtual ~ExportModuleFactory();
  
  virtual tlp::ExportModule * createPluginObject(tlp::AlgorithmContext)=0 /Factory/ ;
  
  virtual  std::string getMajor();
  
  virtual  std::string getMinor();
  
  virtual  std::string getTulipMajor();
  
  virtual  std::string getTulipMinor();
  
  void registerPlugin();
%MethodCode
  tlp::StaticPluginLister<tlp::ExportModule, tlp::AlgorithmContext>::registerPlugin(sipCpp);
%End
  
};
};
