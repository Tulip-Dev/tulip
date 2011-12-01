#include "tulip/FramedGraphicsProxyWidget.h"

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QPalette>

FramedGraphicsProxyWidget::FramedGraphicsProxyWidget(QGraphicsItem *parent, Qt::WindowFlags wFlags): QGraphicsProxyWidget(parent,wFlags) {
}

FramedGraphicsProxyWidget::~FramedGraphicsProxyWidget() {
}

void FramedGraphicsProxyWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *targetWidget) {
  QColor brushColor;
  QColor penColor;
  QWidget* w = widget();
  if (w != NULL) {
    brushColor = w->palette().color(QPalette::Window);
    penColor = w->palette().color(QPalette::Shadow);
  }
  else {
    brushColor = QApplication::palette().color(QPalette::Window);
    penColor = QApplication::palette().color(QPalette::Shadow);
  }
  painter->setBrush(brushColor);
  QPen pen(penColor);
  pen.setWidth(1);
  painter->setPen(pen);
  painter->drawRoundedRect(boundingRect(),6,6);
  QGraphicsProxyWidget::paint(painter,option,targetWidget);
}

QRectF FramedGraphicsProxyWidget::boundingRect() const {
  return QRectF(QPointF(-5,-5),QGraphicsProxyWidget::boundingRect().size() + QSizeF(10,10));
}
