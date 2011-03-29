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
	
	bool exportGraph(std::ostream &,Graph *g) {
		return exportGraph(g);
	}
	
	virtual bool exportGraph(Graph *g) = 0;
	
};
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

	PythonExportModule(tlp::AlgorithmContext context);
		
	virtual bool exportGraph(tlp::Graph *g) = 0;
	
};

class ExportModuleFactory : tlp::Plugin {
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
	sipCpp->initFactory();
	sipCpp->factory->registerPlugin(sipCpp);
%End
  
};
};
