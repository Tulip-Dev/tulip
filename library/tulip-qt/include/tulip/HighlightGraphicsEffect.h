#ifndef HIGHLIGHTGRAPHICSEFFECT_H
#define HIGHLIGHTGRAPHICSEFFECT_H

#include <QtGui/QGraphicsEffect>

class HighlightGraphicsEffect: public QGraphicsEffect {
public:
  HighlightGraphicsEffect(int round, const QColor &color=QColor(255,0,0));

  virtual QRectF boundingRectFor(const QRectF &rect) const;

protected:
  virtual void draw(QPainter *painter);

private:
  int _round;
  QColor _color;
};

#endif // HIGHLIGHTGRAPHICSEFFECT_H
