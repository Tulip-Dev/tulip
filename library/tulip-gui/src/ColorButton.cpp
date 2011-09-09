#include "tulip/ColorButton.h"

#include <QtGui/QColorDialog>
#include <tulip/TlpQtTools.h>
#include <QtGui/QStylePainter>

using namespace tlp;

ColorButton::ColorButton(QWidget *parent): QPushButton(parent), _color(Qt::black) {
  connect(this,SIGNAL(clicked()),this,SLOT(chooseColor()));
}

QColor ColorButton::color() const {
  return _color;
}

tlp::Color ColorButton::tulipColor() const {
  return QColorToColor(_color);
}

void ColorButton::paintEvent(QPaintEvent *event) {
  QPushButton::paintEvent(event);
  QStylePainter p(this);
  float tickW = width() / 4.;
  float tickH = height() / 4;
  p.setPen(Qt::black);
  p.setBrush(_color);
  p.drawRect(tickW, tickH, tickW * 2, tickH * 2);
}

void ColorButton::setColor(const QColor& c) {
  _color = c;
  repaint();
  emit colorChanged(_color);
  emit tulipColorChanged(QColorToColor(_color));
}

void ColorButton::setTulipColor(const tlp::Color& c) {
  setColor(colorToQColor(c));
}

void ColorButton::chooseColor() {
  QColor c = QColorDialog::getColor(_color,this,trUtf8("Choose a color"),QColorDialog::ShowAlphaChannel);

  if (c.isValid())
    setColor(c);
}
