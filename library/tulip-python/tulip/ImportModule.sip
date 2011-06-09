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
namespace tlp {
class PythonImportModule : public ImportModule {
public :
	PythonImportModule(AlgorithmContext context) : 
		ImportModule(context){}
	
	bool import() {
		return importGraph();
	}
	
	virtual bool importGraph() = 0;
};
typedef tlp::FactoryInterface<tlp::ImportModule, tlp::AlgorithmContext> ImportModuleFactory;
};
%End


namespace tlp {
class ImportModule :tlp::WithParameter, tlp::WithDependency /Abstract/ {
%TypeHeaderCode
#include <tulip/ImportModule.h>
%End
public:

  ImportModule (tlp::AlgorithmContext context);
  virtual ~ImportModule();
    
  tlp::Graph *graph;
  tlp::DataSet *dataSet;
  tlp::PluginProgress *pluginProgress;
};

class PythonImportModule : tlp::ImportModule {
public:

  PythonImportModule(tlp::AlgorithmContext context);
  
  virtual bool importGraph() = 0;
  
};

class ImportModuleFactory : tlp::AbstractPluginInfo {
%TypeHeaderCode
#include <tulip/ImportModule.h>
%End
public:
	virtual ~ImportModuleFactory();
	
	virtual tlp::ImportModule * createPluginObject(tlp::AlgorithmContext) = 0 /Factory/; 
	
	virtual  std::string getMajor() const;
	
	virtual  std::string getMinor() const;
	
	virtual  std::string getTulipMajor() const;
	
	virtual  std::string getTulipMinor() const;
	
	void registerPlugin();
%MethodCode
	tlp::StaticPluginLister<tlp::ImportModule, tlp::AlgorithmContext>::registerPlugin(sipCpp);
%End
};
};

