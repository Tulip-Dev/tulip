#include "tulip/FramedGraphicsProxyWidget.h"

#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QPalette>

FramedGraphicsProxyWidget::FramedGraphicsProxyWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags): QGraphicsProxyWidget(parent,wFlags) {
}

FramedGraphicsProxyWidget::~FramedGraphicsProxyWidget() {
}

void FramedGraphicsProxyWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  painter->setBrush(QApplication::palette().color(QPalette::Window));
  QPen pen(QApplication::palette().color(QPalette::Shadow));
  pen.setWidth(1);
  painter->setPen(pen);
  painter->drawRoundedRect(boundingRect(),6,6);
  QGraphicsProxyWidget::paint(painter,option,widget);
}

QRectF FramedGraphicsProxyWidget::boundingRect() const {
  return QRectF(QPointF(-5,-5),QGraphicsProxyWidget::boundingRect().size() + QSizeF(10,10));
}
