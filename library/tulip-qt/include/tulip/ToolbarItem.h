#ifndef TOOLBARITEM_H
#define TOOLBARITEM_H

#include <QtGui/QGraphicsItemGroup>
#include <QtCore/QEasingCurve>

class QAction;

namespace tlp {
class PushButtonItem;

class ToolbarItem: public QGraphicsItemGroup {
public:
  ToolbarItem(QGraphicsItem *parent=0, QGraphicsScene *scene=0);
  virtual ~ToolbarItem();

  void addAction(QAction *);
  void removeAction(QAction *);
  void setActiveAction(QAction *);

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
  virtual QRectF boundingRect() const;

private:
  QVector<QAction *> _actions;
  QMap<QAction *,PushButtonItem *> _actionButton;
  QAction *_activeAction;
  PushButtonItem *_activeButton;

  // Display parameters
  QSize _iconSize;
  qreal _margin;
  Qt::Orientation _orientation;
  int _animationMsec;
  QEasingCurve _animationEasing;

  QPointF translationVector() const;
  QSizeF hoveredIconSize() const;
};
}

#endif // TOOLBARITEM_H
