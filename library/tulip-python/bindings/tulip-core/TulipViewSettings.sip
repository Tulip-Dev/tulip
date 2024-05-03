/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/TulipViewSettings.h>
%End

namespace tlp {

class NodeShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:
  enum NodeShapes /BaseType=IntEnum/ {
    Billboard=NodeShape::Billboard,
    Circle=NodeShape::Circle,
    Cone=NodeShape::Cone,
    Cross=NodeShape::Cross,
    Cube=NodeShape::Cube,
    CubeOutlined=NodeShape::CubeOutlined,
    CubeOutlinedTransparent=NodeShape::CubeOutlinedTransparent,
    Cylinder=NodeShape::Cylinder,
    Diamond=NodeShape::Diamond,
    GlowSphere=NodeShape::GlowSphere,
    HalfCylinder=NodeShape::HalfCylinder,
    Hexagon=NodeShape::Hexagon,
    Pentagon=NodeShape::Pentagon,
    Ring=NodeShape::Ring,
    RoundedBox=NodeShape::RoundedBox,
    Sphere=NodeShape::Sphere,
    Square=NodeShape::Square,
    Triangle=NodeShape::Triangle,
    Window=NodeShape::Window,
    Star=NodeShape::Star,
    FontAwesomeIcon=NodeShape::FontAwesomeIcon,
    Icon=NodeShape::Icon
  };
};

class EdgeShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:
 enum EdgeShapes /BaseType=IntEnum/ {
    Polyline=EdgeShape::EdgeShapes::Polyline,
    BezierCurve=EdgeShape::EdgeShapes::BezierCurve,
    CatmullRomCurve=EdgeShape::EdgeShapes::CatmullRomCurve,
    CubicBSplineCurve=EdgeShape::EdgeShapes::CubicBSplineCurve
  };
};



class EdgeExtremityShape /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

  enum EdgeExtremityShapes /BaseType=IntEnum/ {
    None=EdgeExtremityShape::None,
    Arrow=EdgeExtremityShape::Arrow,
    Circle=EdgeExtremityShape::Circle,
    Cone=EdgeExtremityShape::Cone,
    Cross=EdgeExtremityShape::Cross,
    Cube=EdgeExtremityShape::Cube,
    CubeOutlinedTransparent=EdgeExtremityShape::CubeOutlinedTransparent,
    Cylinder=EdgeExtremityShape::Cylinder,
    Diamond=EdgeExtremityShape::Diamond,
    GlowSphere=EdgeExtremityShape::GlowSphere,
    Hexagon=EdgeExtremityShape::Hexagon,
    Pentagon=EdgeExtremityShape::Pentagon,
    Ring=EdgeExtremityShape::Ring,
    Sphere=EdgeExtremityShape::Sphere,
    Square=EdgeExtremityShape::Square,
    Star=EdgeExtremityShape::Star,
    FontAwesomeIcon=EdgeExtremityShape::FontAwesomeIcon,
    Icon=EdgeExtremityShape::Icon
  };
};

class LabelPosition /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

enum LabelPositions /BaseType=IntEnum/ {
    Center=LabelPosition::Center,
    Top=LabelPosition::Top,
    Bottom=LabelPosition::Bottom,
    Left=LabelPosition::Left,
    Right=LabelPosition::Right
  };
};

class TulipViewSettings : tlp::Observable {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

public:

  static tlp::TulipViewSettings &instance();

  tlp::Color defaultColor(tlp::ElementType elem) const;

  void setDefaultColor(tlp::ElementType elem, const tlp::Color& color);

  tlp::Color defaultBorderColor(tlp::ElementType elem) const;

  void setDefaultBorderColor(tlp::ElementType elem, const tlp::Color& color);

  float defaultBorderWidth(tlp::ElementType elem) const;

  void setdefaultBorderWidth(tlp::ElementType elem, float borderWidth);

  tlp::Color defaultLabelColor() const;

  void setDefaultLabelColor(const tlp::Color& color);

  tlp::Color defaultLabelBorderColor() const;

  void setDefaultLabelBorderColor(const tlp::Color& color);

  float defaultLabelBorderWidth() const;

  void setDefaultLabelBorderWidth(float borderWidth);

  int defaultLabelPosition() const;

  void setDefaultLabelPosition(int position);

  tlp::Size defaultSize(tlp::ElementType elem) const;

  void setDefaultSize(tlp::ElementType elem, const tlp::Size& size);

  int defaultShape(tlp::ElementType elem) const;

  void setDefaultShape(tlp::ElementType elem, int shape);

  int defaultEdgeExtremitySrcShape() const;

  void setDefaultEdgeExtremitySrcShape(int shape);

  int defaultEdgeExtremityTgtShape() const;

  void setDefaultEdgeExtremityTgtShape(int shape);

  tlp::Size defaultEdgeExtremitySrcSize() const;

  void setDefaultEdgeExtremitySrcSize(const tlp::Size &size);

  tlp::Size defaultEdgeExtremityTgtSize() const;

  void setDefaultEdgeExtremityTgtSize(const tlp::Size &size);

  std::string defaultFontFile() const;

  void setDefaultFontFile(const std::string &fontFile);

  int defaultFontSize() const;

  void setDefaultFontSize(int fontSize);

private:

  TulipViewSettings();
  
};

class ViewSettingsEvent : public tlp::Event /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/TulipViewSettings.h>
%End

%ConvertToSubClassCode
   if (dynamic_cast<tlp::ViewSettingsEvent*>(sipCpp)) {
        sipType = sipFindType("tlp::ViewSettingsEvent");
   } else {
        sipType = NULL;
   }
%End

public:

  enum ViewSettingsEventType /BaseType=IntEnum/ {
    TLP_DEFAULT_COLOR_MODIFIED,
    TLP_DEFAULT_SHAPE_MODIFIED,
    TLP_DEFAULT_SIZE_MODIFIED,
    TLP_DEFAULT_LABEL_COLOR_MODIFIED
  };

  ViewSettingsEventType getType() const;

  tlp::ElementType getElementType() const;

  tlp::Color getColor() const;

  tlp::Size getSize() const;

  int getShape() const;

};
};
