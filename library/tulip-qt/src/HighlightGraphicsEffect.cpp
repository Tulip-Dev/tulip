#include "tulip/HighlightGraphicsEffect.h"

#include <QtGui/QPainter>

HighlightGraphicsEffect::HighlightGraphicsEffect(int round, const QColor &color): _round(round), _color(color) {
}

QRectF HighlightGraphicsEffect::boundingRectFor(const QRectF &rect) const {
  return rect;
}

void HighlightGraphicsEffect::draw(QPainter *painter) {
  QRectF sourceRect = sourceBoundingRect();
  painter->setBrush(_color);
  painter->drawRoundedRect(sourceRect,_round,_round);
  drawSource(painter);
}
