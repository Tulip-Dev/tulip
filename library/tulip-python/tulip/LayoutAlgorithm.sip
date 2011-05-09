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
#include <tulip/LayoutAlgorithm.h>
#include <tulip/LayoutProperty.h>
namespace tlp {
typedef tlp::PropertyFactory<tlp::LayoutAlgorithm> LayoutAlgorithmFactory;
};
%End


namespace tlp {
class LayoutAlgorithm: tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/LayoutAlgorithm.h>
%End
public:
	virtual ~LayoutAlgorithm();
	tlp::LayoutProperty *layoutResult;
protected:
	LayoutAlgorithm(const tlp::PropertyContext &context);
	
};

class LayoutAlgorithmFactory : tlp::PluginInfoInterface {
%TypeHeaderCode
#include <tulip/TemplateFactory.h>
%End
public:

	virtual ~LayoutAlgorithmFactory();
	
	virtual tlp::LayoutAlgorithm * createPluginObject(const tlp::PropertyContext &) = 0 /Factory/; 

	virtual  std::string getMajor() const;
	
	virtual  std::string getMinor() const;
	
	virtual  std::string getTulipMajor() const;
	
	virtual  std::string getTulipMinor() const;
	
	void registerPlugin();
%MethodCode
	tlp::LayoutProperty::initFactory();			
    tlp::LayoutProperty::factory->registerPlugin(sipCpp);
%End

};

};



