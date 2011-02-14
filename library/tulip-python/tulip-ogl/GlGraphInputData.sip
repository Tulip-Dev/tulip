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
// | Improved and maintained by Antoine Lambert 			                 |
// | Contact:  antoine.lambert@labri.fr 								     |
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

class GlGraphInputData /Abstract/ {
%TypeHeaderCode
#include <tulip/GlGraphInputData.h>
%End
public:

	tlp::Graph* getGraph();

	tlp::LayoutProperty* getLayoutProperty();
	
	void setElementColorPropName(const std::string &name);
	
	std::string getElementColorPropName();
	
	void setElementLabelColorPropName(const std::string &name) ;
	
	std::string getElementLabelColorPropName();
	
	void setElementSizePropName(const std::string &name);
	
	std::string getElementSizePropName();
	
	void setElementLabelPositionPropName(const std::string &name);
	
	std::string getElementLabelPositionPropName();
	
	void setElementShapePropName(const std::string &name);
	
	std::string getElementShapePropName();
	
	void setElementRotationPropName(const std::string &name);
	
	std::string getElementRotationPropName();
	
	void setElementSelectedPropName(const std::string &name);
	
	std::string getElementSelectedPropName();
	
    void setElementFontPropName(const std::string &name);
    
    std::string getElementFontPropName();
    
    void setElementFontSizePropName(const std::string &name);
    
    std::string getElementFontSizePropName();
    
	void setElementLabelPropName(const std::string &name);
	
	std::string getElementLabelPropName();
	
	void setElementTexturePropName(const std::string &name);
	
	std::string getElementTexturePropName();
	
	void setElementBorderColorPropName(const std::string &name);
	
	std::string getElementBorderColorPropName();
	
	void setElementBorderWidthPropName(const std::string &name);
	
	std::string getElementBorderWidthPropName();
	
	void setElementLayoutPropName(const std::string &name);
	
	std::string getElementLayoutPropName();

	std::string getElementSourceAnchorShapePropName();

	void setElementSourceAnchorShapePropName(const std::string &name);

	std::string getElementSourceAnchorSizePropName();

	void setElementSourceAnchorSizePropName(const std::string& name);

	std::string getElementTargetAnchorShapePropName();

	void setElementTargetAnchorShapePropName(const std::string &name);

	std::string getElementTargetAnchorSizePropName();
	
	void setElementTargetAnchorSizePropName(const std::string& name);

  tlp::ColorProperty *elementColor;
  tlp::ColorProperty *elementLabelColor;
  tlp::SizeProperty *elementSize;
  tlp::IntegerProperty *elementLabelPosition;
  tlp::IntegerProperty *elementShape;
  tlp::DoubleProperty *elementRotation;
  tlp::BooleanProperty *elementSelected;
  tlp::StringProperty   *elementFont;
  tlp::IntegerProperty  *elementFontSize;
  tlp::StringProperty *elementLabel;
  tlp::LayoutProperty *elementLayout;
  tlp::StringProperty *elementTexture;
  tlp::ColorProperty *elementBorderColor;
  tlp::DoubleProperty *elementBorderWidth;
  tlp::IntegerProperty *elementSrcAnchorShape;
  tlp::SizeProperty *elementSrcAnchorSize;
  tlp::IntegerProperty *elementTgtAnchorShape;
  tlp::SizeProperty *elementTgtAnchorSize;
  tlp::IntegerProperty *elementAnimationFrame;

  tlp::Graph* graph;


};
};

