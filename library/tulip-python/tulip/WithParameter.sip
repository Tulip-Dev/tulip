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

namespace tlp {
%TypeHeaderCode
#include <tulip/WithParameter.h>
#include <tulip/ColorScale.h>
#include <tulip/StringCollection.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
%End
struct WithParameter {
	
	void addStringParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<std::string>(a0, a1, a2, a3);
%End

	void addStringCollectionParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::StringCollection>(a0, a1, a2, a3);
%End

	void addIntParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<int>(a0, a1, a2, a3);
%End

	void addDoubleParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<double>(a0, a1, a2, a3);
%End

	void addBoolParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<bool>(a0, a1, a2, a3);
%End		

	void addDoublePropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::DoubleProperty>(a0, a1, a2, a3);
%End

	void addIntegerPropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::IntegerProperty>(a0, a1, a2, a3);
%End

	void addLayoutPropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::LayoutProperty>(a0, a1, a2, a3);
%End

	void addSizePropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::SizeProperty>(a0, a1, a2, a3);
%End

	void addColorPropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::ColorProperty>(a0, a1, a2, a3);
%End

	void addBooleanPropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::BooleanProperty>(a0, a1, a2, a3);
%End

	void addColorParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::Color>(a0, a1, a2, a3);
%End

   void addColorScaleParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::ColorScale>(a0, a1, a2, a3);
%End

	void addPropertyParameter(const char* str,
			const char* inHelp=0,
			const char* inDefValue = 0,
			bool isMandatory = true);
%MethodCode
	sipCpp->addParameter<tlp::PropertyInterface*>(a0, a1, a2, a3);
%End
};
};
