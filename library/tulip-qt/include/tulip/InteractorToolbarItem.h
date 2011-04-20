#ifndef INTERACTORTOOLBARITEM_H
#define INTERACTORTOOLBARITEM_H

#include <QtGui/QGraphicsItemGroup>

namespace tlp {
class ToolbarItem;
class Interactor;

class InteractorToolbarItem: public QObject {
  Q_OBJECT
public:
  InteractorToolbarItem(std::list<Interactor *> interactors, QGraphicsItem *parent=0, QGraphicsScene *scene=0);
  virtual ~InteractorToolbarItem();

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget=0);
  virtual QRectF boundingRect() const;

private:
  std::list<Interactor *> _interactors;
  ToolbarItem *_toolbar;
};
}

#endif // INTERACTORTOOLBARITEM_H
