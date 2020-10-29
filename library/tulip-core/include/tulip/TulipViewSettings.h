/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

class TLP_SCOPE NodeShape {

public:
  enum NodeShapes {
    Billboard = 7,
    BottomShadowedSphere = 21,
    ChristmasTree = 28,
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
    FontAwesomeIcon = 20,
    Icon = 20
  };
};

class TLP_SCOPE EdgeShape {

public:
  enum EdgeShapes { Polyline = 0, BezierCurve = 4, CatmullRomCurve = 8, CubicBSplineCurve = 16 };
};

class TLP_SCOPE EdgeExtremityShape {

public:
  enum EdgeExtremityShapes {
    None = -1,
    Arrow = 50,
    Circle = 14,
    Cone = 3,
    Cross = 8,
    Cube = 0,
    CubeOutlinedTransparent = 9,
    Cylinder = 6,
    Diamond = 5,
    GlowSphere = 16,
    Hexagon = 13,
    Pentagon = 12,
    Ring = 15,
    Sphere = 2,
    Square = 4,
    Star = 19,
    FontAwesomeIcon = 20,
    Icon = 20
  };
};

class TLP_SCOPE LabelPosition {

public:
  enum LabelPositions { Center = 0, Top, Bottom, Left, Right };
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
