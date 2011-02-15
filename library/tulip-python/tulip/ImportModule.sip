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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert                              |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/ImportModule.h>
namespace tlp {
class PythonImportModule : public ImportModule {
public :
	PythonImportModule(AlgorithmContext context) : 
		ImportModule(context){}
	
	bool import(const std::string &s) {
		return importGraph(s);
	}
	
	virtual bool importGraph(const std::string &s) = 0;
};
}
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
  
  virtual bool importGraph(const std::string &s) = 0;
  
};

class ImportModuleFactory : tlp::Plugin {
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
	sipCpp->initFactory();
	sipCpp->factory->registerPlugin(sipCpp);
%End
};
};

