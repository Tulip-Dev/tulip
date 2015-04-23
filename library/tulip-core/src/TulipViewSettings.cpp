#include <tulip/TulipViewSettings.h>
#include <tulip/TlpTools.h>

namespace tlp {

TulipViewSettings *TulipViewSettings::_instance(NULL);

TulipViewSettings::TulipViewSettings() :
  _defaultNodeColor(Color::Red),
  _defaultEdgeColor(Color::Black),
  _defaultNodeBorderColor(Color::Black),
  _defaultEdgeBorderColor(Color::Black),
  _defaultLabelColor(Color::Black),
  _defaultLabelBorderColor(Color::Black),
  _defaultNodeBorderWidth(0),
  _defaultEdgeBorderWidth(1),
  _defaultLabelBorderWidth(1),
  _defaultLabelPosition(LabelPosition::Center),
  _defaultNodeSize(Size(1,1,1)),
  _defaultEdgeSize(Size(0.125,0.125,0.5)),
  _defaultNodeShape(NodeShape::Circle),
  _defaultEdgeShape(EdgeShape::Polyline),
  _defaultEdgeExtremitySrcShape(EdgeExtremityShape::None),
  _defaultEdgeExtremityTgtShape(EdgeExtremityShape::Arrow),
  _defaultEdgeExtremitySrcSize(Size(1,1,0)),
  _defaultEdgeExtremityTgtSize(Size(1,1,0)),
  _defaultFontFile(tlp::TulipBitmapDir + "font.ttf"),
  _defaultFontSize(18) {
}

TulipViewSettings &TulipViewSettings::instance() {
  if (!_instance) {
    _instance = new TulipViewSettings();
  }

  return *_instance;
}

Color TulipViewSettings::defaultColor(ElementType elem) const {
  if (elem == NODE) {
    return _defaultNodeColor;
  }
  else {
    return _defaultEdgeColor;
  }
}

void TulipViewSettings::setDefaultColor(ElementType elem, const Color& color) {
  if (color == defaultColor(elem))
    return;

  if (elem == NODE) {
    _defaultNodeColor = color;
  }
  else {
    _defaultEdgeColor = color;
  }

  sendEvent(ViewSettingsEvent(elem, color));
}

Color TulipViewSettings::defaultBorderColor(ElementType elem) const {
  if (elem == NODE) {
    return _defaultNodeBorderColor;
  }
  else {
    return _defaultEdgeBorderColor;
  }
}

void TulipViewSettings::setDefaultBorderColor(ElementType elem, const Color& color) {
  if (elem == NODE) {
    _defaultNodeBorderColor = color;
  }
  else {
    _defaultEdgeBorderColor = color;
  }
}

float TulipViewSettings::defaultBorderWidth(ElementType elem) const {
  if (elem == NODE) {
    return _defaultNodeBorderWidth;
  }
  else {
    return _defaultEdgeBorderWidth;
  }
}

void TulipViewSettings::setdefaultBorderWidth(ElementType elem, float borderWidth) {
  if (elem == NODE) {
    _defaultNodeBorderWidth = borderWidth;
  }
  else {
    _defaultEdgeBorderWidth = borderWidth;
  }
}

Color TulipViewSettings::defaultLabelColor() const {
  return _defaultLabelColor;
}

void TulipViewSettings::setDefaultLabelColor(const Color& color) {
  if (color == _defaultLabelColor)
    return;

  _defaultLabelColor = color;
  sendEvent(ViewSettingsEvent(color));
}

Color TulipViewSettings::defaultLabelBorderColor() const {
  return _defaultLabelBorderColor;
}

void TulipViewSettings::setDefaultLabelBorderColor(const Color& color) {
  _defaultLabelBorderColor = color;
}

float TulipViewSettings::defaultLabelBorderWidth() const {
  return _defaultLabelBorderWidth;
}

void TulipViewSettings::setDefaultLabelBorderWidth(float borderWidth) {
  _defaultLabelBorderWidth = borderWidth;
}

int TulipViewSettings::defaultLabelPosition() const {
  return _defaultLabelPosition;
}

void TulipViewSettings::setDefaultLabelPosition(int position) {
  _defaultLabelPosition = position;
}

Size TulipViewSettings::defaultSize(ElementType elem) const {
  if (elem == NODE) {
    return _defaultNodeSize;
  }
  else {
    return _defaultEdgeSize;
  }
}

void TulipViewSettings::setDefaultSize(ElementType elem, const Size& size) {
  if (size == defaultSize(elem))
    return;

  if (elem == NODE) {
    _defaultNodeSize = size;
  }
  else {
    _defaultEdgeSize = size;
  }

  sendEvent(ViewSettingsEvent(elem, size));
}

int TulipViewSettings::defaultShape(ElementType elem) const {
  if (elem == NODE) {
    return _defaultNodeShape;
  }
  else {
    return _defaultEdgeShape;
  }
}

void TulipViewSettings::setDefaultShape(ElementType elem, int shape) {
  if (shape == defaultShape(elem))
    return;

  if (elem == NODE) {
    _defaultNodeShape = shape;
  }
  else {
    _defaultEdgeShape = shape;
  }

  sendEvent(ViewSettingsEvent(elem, shape));
}

int TulipViewSettings::defaultEdgeExtremitySrcShape() const {
  return _defaultEdgeExtremitySrcShape;
}

void TulipViewSettings::setDefaultEdgeExtremitySrcShape(int shape) {
  _defaultEdgeExtremitySrcShape = shape;
}

int TulipViewSettings::defaultEdgeExtremityTgtShape() const {
  return _defaultEdgeExtremityTgtShape;
}

void TulipViewSettings::setDefaultEdgeExtremityTgtShape(int shape) {
  _defaultEdgeExtremityTgtShape = shape;
}

Size TulipViewSettings::defaultEdgeExtremitySrcSize() const {
  return _defaultEdgeExtremitySrcSize;
}

void TulipViewSettings::setDefaultEdgeExtremitySrcSize(const Size &size) {
  _defaultEdgeExtremitySrcSize = size;
}

Size TulipViewSettings::defaultEdgeExtremityTgtSize() const {
  return _defaultEdgeExtremityTgtSize;
}

void TulipViewSettings::setDefaultEdgeExtremityTgtSize(const Size &size) {
  _defaultEdgeExtremityTgtSize = size;
}

std::string TulipViewSettings::defaultFontFile() const {
  return _defaultFontFile;
}

void TulipViewSettings::setDefaultFontFile(const std::string &fontFile) {
  _defaultFontFile = fontFile;
}

int TulipViewSettings::defaultFontSize() const {
  return _defaultFontSize;
}

void TulipViewSettings::setDefaultFontSize(int fontSize) {
  _defaultFontSize = fontSize;
}

}
