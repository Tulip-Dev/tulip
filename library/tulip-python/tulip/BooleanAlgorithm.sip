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
#include <tulip/BooleanAlgorithm.h>
#include <tulip/BooleanProperty.h>
namespace tlp {
typedef tlp::PropertyFactory<tlp::BooleanAlgorithm> BooleanAlgorithmFactory;
};
%End

namespace tlp {
class BooleanAlgorithm : tlp::PropertyAlgorithm {
%TypeHeaderCode
#include <tulip/BooleanAlgorithm.h>
%End
public:
	virtual ~BooleanAlgorithm();
	tlp::BooleanProperty *booleanResult;
protected:
	BooleanAlgorithm(const tlp::PropertyContext &context);
	
};
};

namespace tlp {
class BooleanAlgorithmFactory : tlp::Plugin {
%TypeHeaderCode
#include <tulip/TemplateFactory.h>
%End
public:

	virtual ~BooleanAlgorithmFactory();
	
	virtual tlp::BooleanAlgorithm * createPluginObject(const tlp::PropertyContext &) = 0 /Factory/; 

	virtual  std::string getMajor() const;
	
	virtual  std::string getMinor() const;
	
	virtual  std::string getTulipMajor() const;
	
	virtual  std::string getTulipMinor() const;
	
	void registerPlugin();
%MethodCode
	tlp::BooleanProperty::initFactory();			
    tlp::BooleanProperty::factory->registerPlugin(sipCpp);
%End

};
};
