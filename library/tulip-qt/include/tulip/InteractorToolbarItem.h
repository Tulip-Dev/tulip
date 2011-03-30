#ifndef INTERACTORTOOLBARITEM_H
#define INTERACTORTOOLBARITEM_H

#include <QtGui/QGraphicsItemGroup>

class ToolbarItem;
class PushButtonItem;

class InteractorToolbarItem: public QGraphicsItemGroup {
public:
  InteractorToolbarItem(QGraphicsItem *parent=0, QGraphicsScene *scene=0);
  virtual ~InteractorToolbarItem();

private:
  ToolbarItem *_toolbar;
  PushButtonItem *_configurationWidgetButton;

  Qt::Orientation _orientation;

  QPointF settingsButtonPos() const;
  QPointF configurationWidgetPos() const;
  QPointF toolbarPos() const;
};

#endif // INTERACTORTOOLBARITEM_H
