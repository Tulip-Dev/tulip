/**
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
#include <tulip/TulipViewSettings.h>
#include <tulip/TlpTools.h>

namespace tlp {

static TulipViewSettings _instance;
static Color _defaultNodeColor(Color::Red);
static Color _defaultEdgeColor(Color::Black);
static Color _defaultNodeBorderColor(Color::Black);
static Color _defaultEdgeBorderColor(Color::Black);
static Color _defaultLabelColor(Color::Black);
static Color _defaultLabelBorderColor(Color::Black);
static float _defaultNodeBorderWidth(0);
static float _defaultEdgeBorderWidth(0);
static float _defaultLabelBorderWidth(1);
static int _defaultLabelPosition(LabelPosition::Center);
static Size _defaultNodeSize(Size(1, 1, 1));
static Size _defaultEdgeSize(Size(0.125, 0.125, 0.5));
static int _defaultNodeShape(NodeShape::Circle);
static int _defaultEdgeShape(EdgeShape::Polyline);
static int _defaultEdgeExtremitySrcShape(EdgeExtremityShape::None);
static int _defaultEdgeExtremityTgtShape(EdgeExtremityShape::Arrow);
static Size _defaultEdgeExtremitySrcSize(Size(1, 1, 0));
static Size _defaultEdgeExtremityTgtSize(Size(1, 1, 0));
static std::string _defaultFontFile;
static int _defaultFontSize(18);

TulipViewSettings &TulipViewSettings::instance() {
  return _instance;
}

Color TulipViewSettings::defaultColor(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeColor;
  } else {
    return _defaultEdgeColor;
  }
}

void TulipViewSettings::setDefaultColor(ElementType elem, const Color &color) {
  if (color == defaultColor(elem))
    return;

  if (elem == NODE) {
    _defaultNodeColor = color;
  } else {
    _defaultEdgeColor = color;
  }

  _instance.sendEvent(ViewSettingsEvent(elem, color));
}

Color TulipViewSettings::defaultBorderColor(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeBorderColor;
  } else {
    return _defaultEdgeBorderColor;
  }
}

void TulipViewSettings::setDefaultBorderColor(ElementType elem, const Color &color) {
  if (elem == NODE) {
    _defaultNodeBorderColor = color;
  } else {
    _defaultEdgeBorderColor = color;
  }
}

float TulipViewSettings::defaultBorderWidth(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeBorderWidth;
  } else {
    return _defaultEdgeBorderWidth;
  }
}

void TulipViewSettings::setdefaultBorderWidth(ElementType elem, float borderWidth) {
  if (elem == NODE) {
    _defaultNodeBorderWidth = borderWidth;
  } else {
    _defaultEdgeBorderWidth = borderWidth;
  }
}

Color TulipViewSettings::defaultLabelColor() {
  return _defaultLabelColor;
}

void TulipViewSettings::setDefaultLabelColor(const Color &color) {
  if (color == _defaultLabelColor)
    return;

  _defaultLabelColor = color;
  _instance.sendEvent(ViewSettingsEvent(color));
}

Color TulipViewSettings::defaultLabelBorderColor() {
  return _defaultLabelBorderColor;
}

void TulipViewSettings::setDefaultLabelBorderColor(const Color &color) {
  _defaultLabelBorderColor = color;
}

float TulipViewSettings::defaultLabelBorderWidth() {
  return _defaultLabelBorderWidth;
}

void TulipViewSettings::setDefaultLabelBorderWidth(float borderWidth) {
  _defaultLabelBorderWidth = borderWidth;
}

TulipViewSettings::labelmap TulipViewSettings::POSITION_LABEL_MAP = {
    {LabelPosition::Center, "Center"},
    {LabelPosition::Top, "Top"},
    {LabelPosition::Bottom, "Bottom"},
    {LabelPosition::Left, "Left"},
    {LabelPosition::Right, "Right"}};

int TulipViewSettings::defaultLabelPosition() {
  return _defaultLabelPosition;
}

void TulipViewSettings::setDefaultLabelPosition(int position) {
  _defaultLabelPosition = position;
}

Size TulipViewSettings::defaultSize(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeSize;
  } else {
    return _defaultEdgeSize;
  }
}

void TulipViewSettings::setDefaultSize(ElementType elem, const Size &size) {
  if (size == defaultSize(elem))
    return;

  if (elem == NODE) {
    _defaultNodeSize = size;
  } else {
    _defaultEdgeSize = size;
  }

  _instance.sendEvent(ViewSettingsEvent(elem, size));
}

int TulipViewSettings::defaultShape(ElementType elem) {
  if (elem == NODE) {
    return _defaultNodeShape;
  } else {
    return _defaultEdgeShape;
  }
}

void TulipViewSettings::setDefaultShape(ElementType elem, int shape) {
  if (shape == defaultShape(elem))
    return;

  if (elem == NODE) {
    _defaultNodeShape = shape;
  } else {
    _defaultEdgeShape = shape;
  }

  _instance.sendEvent(ViewSettingsEvent(elem, shape));
}

int TulipViewSettings::defaultEdgeExtremitySrcShape() {
  return _defaultEdgeExtremitySrcShape;
}

void TulipViewSettings::setDefaultEdgeExtremitySrcShape(int shape) {
  _defaultEdgeExtremitySrcShape = shape;
}

int TulipViewSettings::defaultEdgeExtremityTgtShape() {
  return _defaultEdgeExtremityTgtShape;
}

void TulipViewSettings::setDefaultEdgeExtremityTgtShape(int shape) {
  _defaultEdgeExtremityTgtShape = shape;
}

Size TulipViewSettings::defaultEdgeExtremitySrcSize() {
  return _defaultEdgeExtremitySrcSize;
}

void TulipViewSettings::setDefaultEdgeExtremitySrcSize(const Size &size) {
  _defaultEdgeExtremitySrcSize = size;
}

Size TulipViewSettings::defaultEdgeExtremityTgtSize() {
  return _defaultEdgeExtremityTgtSize;
}

void TulipViewSettings::setDefaultEdgeExtremityTgtSize(const Size &size) {
  _defaultEdgeExtremityTgtSize = size;
}

std::string TulipViewSettings::defaultFontFile() {
  // _defaultFontFile initialization must be delayed
  // until TulipBitmapDir is set
  if (_defaultFontFile.empty())
    _defaultFontFile = tlp::TulipBitmapDir + "font.ttf";
  return _defaultFontFile;
}

void TulipViewSettings::setDefaultFontFile(const std::string &fontFile) {
  _defaultFontFile = fontFile;
}

int TulipViewSettings::defaultFontSize() {
  return _defaultFontSize;
}

void TulipViewSettings::setDefaultFontSize(int fontSize) {
  _defaultFontSize = fontSize;
}

} // namespace tlp
