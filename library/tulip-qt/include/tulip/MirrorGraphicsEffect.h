#ifndef MIRRORGRAPHICSEFFECT_H
#define MIRRORGRAPHICSEFFECT_H

#include <QtGui/QGraphicsEffect>

class MirrorGraphicsEffect: public QGraphicsEffect {
public:
  MirrorGraphicsEffect(int yoffset=0, int height=0);

  virtual QRectF boundingRectFor(const QRectF &rect) const;

protected:
  virtual void draw(QPainter *painter);

private:
  int _yoffset;
  int _height;
};

#endif // MIRRORGRAPHICSEFFECT_H
