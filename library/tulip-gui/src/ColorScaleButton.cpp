#include "tulip/ColorScaleButton.h"

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <tulip/TlpQtTools.h>

using namespace tlp;

ColorScaleButton::ColorScaleButton(ColorScale colorScale, QWidget* parent): QPushButton(parent), _colorScale(colorScale), _dlg(NULL) {
  connect(this,SIGNAL(clicked()),this,SLOT(showDialog()));
}

ColorScaleButton::~ColorScaleButton() {
}

ColorScale ColorScaleButton::colorScale() const {
  return _colorScale;
}

void ColorScaleButton::setColorScale(const ColorScale& colorScale) {
  _colorScale = colorScale;

  if (_dlg)
    _dlg->setColorScale(colorScale);
}

void ColorScaleButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QPainter painter(this);
  QRect rect = event->rect();
  rect.setX(rect.x()+2);
  rect.setY(rect.y()+2);
  rect.setWidth(rect.width()-4);
  rect.setHeight(rect.height()-4);

  if (colorScale().isGradient()) {
    QLinearGradient grad(QPointF(rect.x(),rect.y()),QPointF(rect.x()+rect.width(),rect.y()));
    std::map<float,Color> stops = colorScale().getColorMap();

    for (std::map<float,Color>::iterator it = stops.begin(); it != stops.end(); ++it)
      grad.setColorAt(it->first,colorToQColor(it->second));

    painter.setBrush(QBrush(grad));
  }

  painter.drawRect(rect);
}

void ColorScaleButton::showDialog() {
  _dlg = new ColorScaleConfigDialog(_colorScale,this);
  _dlg->exec();
  _colorScale = _dlg->getColorScale();
  delete _dlg;
  _dlg = NULL;
}
