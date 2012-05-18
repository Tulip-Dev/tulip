#ifndef FRAMEDGRAPHICSPROXYWIDGET_H
#define FRAMEDGRAPHICSPROXYWIDGET_H

#include <QtGui/QGraphicsProxyWidget>

class FramedGraphicsProxyWidget: public QGraphicsProxyWidget {
public:
  FramedGraphicsProxyWidget(QGraphicsItem *parent=NULL, Qt::WindowFlags wFlags=0);
  virtual ~FramedGraphicsProxyWidget();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  QRectF boundingRect() const;
};

#endif // FRAMEDGRAPHICSPROXYWIDGET_H
