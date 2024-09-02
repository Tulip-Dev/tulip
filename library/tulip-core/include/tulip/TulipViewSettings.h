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

#ifndef TULIPVIEWSETTINGS_H
#define TULIPVIEWSETTINGS_H

#include <tulip/tulipconf.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Graph.h>
#include <tulip/Observable.h>

#include <string>
#include <map>

namespace tlp {

class TLP_SCOPE TulipShape {

public:
  enum Values : int {
    None = -1,
    Arrow = 50,
    Billboard = 7,
    BottomShadowedSphere = 21,
    Circle = 14,
    Cone = 3,
    Cross = 8,
    Cube = 0,
    CubeOutlined = 1,
    CubeOutlinedTransparent = 9,
    Cylinder = 6,
    Diamond = 5,
    GlowSphere = 16,
    HalfCylinder = 10,
    Hexagon = 13,
    LeftBottomShadowedSphere = 22,
    Pentagon = 12,
    RightBottomShadowedSphere = 23,
    Ring = 15,
    RoundedBox = 18,
    Sphere = 2,
    Square = 4,
    Triangle = 11,
    Window = 17,
    Star = 19,
    Icon = 20
  };
};

class TLP_SCOPE NodeShape {

public:
  enum NodeShapes {
    Billboard = TulipShape::Billboard,
    BottomShadowedSphere = TulipShape::BottomShadowedSphere,
    Circle = TulipShape::Circle,
    Cone = TulipShape::Cone,
    Cross = TulipShape::Cross,
    Cube = TulipShape::Cube,
    CubeOutlined = TulipShape::CubeOutlined,
    CubeOutlinedTransparent = TulipShape::CubeOutlinedTransparent,
    Cylinder = TulipShape::Cylinder,
    Diamond = TulipShape::Diamond,
    GlowSphere = TulipShape::GlowSphere,
    HalfCylinder = TulipShape::HalfCylinder,
    Hexagon = TulipShape::Hexagon,
    LeftBottomShadowedSphere = TulipShape::LeftBottomShadowedSphere,
    Pentagon = TulipShape::Pentagon,
    RightBottomShadowedSphere = TulipShape::RightBottomShadowedSphere,
    Ring = TulipShape::Ring,
    RoundedBox = TulipShape::RoundedBox,
    Sphere = TulipShape::Sphere,
    Square = TulipShape::Square,
    Triangle = TulipShape::Triangle,
    Window = TulipShape::Window,
    Star = TulipShape::Star,
    Icon = TulipShape::Icon
  };

  static bool checkValue(int v) {
    return v > -1 && v < 24;
  }
};

class TLP_SCOPE EdgeShape {

public:
  enum EdgeShapes { Polyline = 0, BezierCurve = 4, CatmullRomCurve = 8, CubicBSplineCurve = 16 };

  inline bool static checkValue(int v) {
    switch (v) {
    case Polyline:
    case BezierCurve:
    case CatmullRomCurve:
    case CubicBSplineCurve:
      return true;
    default:
      return false;
    }
  }
};

class TLP_SCOPE EdgeExtremityShape {

public:
  enum EdgeExtremityShapes {
    None = TulipShape::None,
    Arrow = TulipShape::Arrow,
    Circle = TulipShape::Circle,
    Cone = TulipShape::Cone,
    Cross = TulipShape::Cross,
    Cube = TulipShape::Cube,
    CubeOutlinedTransparent = TulipShape::CubeOutlinedTransparent,
    Cylinder = TulipShape::Cylinder,
    Diamond = TulipShape::Diamond,
    GlowSphere = TulipShape::GlowSphere,
    Hexagon = TulipShape::Hexagon,
    Pentagon = TulipShape::Pentagon,
    Ring = TulipShape::Ring,
    Sphere = TulipShape::Sphere,
    Square = TulipShape::Square,
    Star = TulipShape::Star,
    Icon = TulipShape::Icon
  };

  inline bool static checkValue(int v) {
    switch (v) {
    case TulipShape::None:
    case TulipShape::Arrow:
    case TulipShape::Circle:
    case TulipShape::Cone:
    case TulipShape::Cross:
    case TulipShape::Cube:
    case TulipShape::CubeOutlinedTransparent:
    case TulipShape::Cylinder:
    case TulipShape::Diamond:
    case TulipShape::GlowSphere:
    case TulipShape::Hexagon:
    case TulipShape::Pentagon:
    case TulipShape::Ring:
    case TulipShape::Sphere:
    case TulipShape::Square:
    case TulipShape::Star:
    case TulipShape::Icon:
      return true;
    default:
      return false;
    }
  }
};

class TLP_SCOPE LabelPosition {

public:
  enum LabelPositions { Center = 0, Top = 1, Bottom = 2, Left = 3, Right = 4 };

  inline static bool checkValue(int v) {
    return v > -1 && v < 4;
  }
};

///@cond DOXYGEN_HIDDEN
class TLP_SCOPE TulipViewSettings : public Observable {

public:
  typedef std::map<LabelPosition::LabelPositions, std::string> labelmap;

  static TulipViewSettings &instance();

  static labelmap POSITION_LABEL_MAP;

  static Color defaultColor(ElementType elem);
  static void setDefaultColor(ElementType elem, const Color &color);

  static Color defaultBorderColor(ElementType elem);
  static void setDefaultBorderColor(ElementType elem, const Color &color);

  static float defaultBorderWidth(ElementType elem);
  static void setdefaultBorderWidth(ElementType elem, float borderWidth);

  static Color defaultLabelColor();
  static void setDefaultLabelColor(const Color &color);

  static Color defaultLabelBorderColor();
  static void setDefaultLabelBorderColor(const Color &color);

  static float defaultLabelBorderWidth();
  static void setDefaultLabelBorderWidth(float borderWidth);

  static int defaultLabelPosition();
  static void setDefaultLabelPosition(int position);

  static Size defaultSize(ElementType elem);
  static void setDefaultSize(ElementType elem, const Size &size);

  static int defaultShape(ElementType elem);
  static void setDefaultShape(ElementType elem, int shape);

  static int defaultEdgeExtremitySrcShape();
  static void setDefaultEdgeExtremitySrcShape(int shape);

  static int defaultEdgeExtremityTgtShape();
  static void setDefaultEdgeExtremityTgtShape(int shape);

  static Size defaultEdgeExtremitySrcSize();
  static void setDefaultEdgeExtremitySrcSize(const Size &size);

  static Size defaultEdgeExtremityTgtSize();
  static void setDefaultEdgeExtremityTgtSize(const Size &size);

  static std::string defaultFontFile();
  static void setDefaultFontFile(const std::string &fontFile);

  static int defaultFontSize();
  static void setDefaultFontSize(int fontSize);
};

class TLP_SCOPE ViewSettingsEvent : public tlp::Event {

public:
  enum ViewSettingsEventType {
    TLP_DEFAULT_COLOR_MODIFIED,
    TLP_DEFAULT_SHAPE_MODIFIED,
    TLP_DEFAULT_SIZE_MODIFIED,
    TLP_DEFAULT_LABEL_COLOR_MODIFIED
  };

  ViewSettingsEvent(ElementType elem, const Color &color)
      : Event(TulipViewSettings::instance(), Event::TLP_MODIFICATION),
        _type(TLP_DEFAULT_COLOR_MODIFIED), _elem(elem), _color(color) {}

  ViewSettingsEvent(ElementType elem, const Size &size)
      : Event(TulipViewSettings::instance(), Event::TLP_MODIFICATION),
        _type(TLP_DEFAULT_SIZE_MODIFIED), _elem(elem), _size(size) {}

  ViewSettingsEvent(ElementType elem, int shape)
      : Event(TulipViewSettings::instance(), Event::TLP_MODIFICATION),
        _type(TLP_DEFAULT_SHAPE_MODIFIED), _elem(elem), _shape(shape) {}

  ViewSettingsEvent(const Color &labelColor)
      : Event(TulipViewSettings::instance(), Event::TLP_MODIFICATION),
        _type(TLP_DEFAULT_LABEL_COLOR_MODIFIED), _color(labelColor) {}

  ViewSettingsEventType getType() const {
    return _type;
  }

  ElementType getElementType() const {
    return _elem;
  }

  Color getColor() const {
    return _color;
  }

  Size getSize() const {
    return _size;
  }

  int getShape() const {
    return _shape;
  }

private:
  ViewSettingsEventType _type;
  ElementType _elem;
  Color _color;
  Size _size;
  int _shape;
};
///@endcond
} // namespace tlp

#endif // TULIPVIEWSETTINGS_H
