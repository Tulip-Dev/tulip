#include "tulip/MirrorGraphicsEffect.h"

#include <QtGui/QPainter>

MirrorGraphicsEffect::MirrorGraphicsEffect(int yoffset, int height): _yoffset(yoffset), _height(height) {
}

QRectF MirrorGraphicsEffect::boundingRectFor(const QRectF &rect) const {
  return rect;
}

void MirrorGraphicsEffect::draw(QPainter *painter) {
  // Original pixmap
  painter->drawPixmap(0,0,sourcePixmap());
  QImage img = sourcePixmap().toImage();

  // Mirror effect
  QLinearGradient gradient(0,0,0,img.height());
  gradient.setColorAt(0.4,Qt::black);
  gradient.setColorAt(0,Qt::white);
  QImage reflexion = img.mirrored();
  QImage mask = img;
  QPainter p(&mask);
  p.fillRect(img.rect(), gradient);
  p.end();
  reflexion.setAlphaChannel(mask);

  // paint
  int h = (_height == 0 ? reflexion.height() : _height);
  painter->drawPixmap(0,sourcePixmap().height()+_yoffset,QPixmap::fromImage(reflexion.copy(0,0,reflexion.width(),h)));
}
