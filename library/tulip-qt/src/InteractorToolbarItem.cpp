#include "tulip/InteractorToolbarItem.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QApplication>

#include <tulip/Interactor.h>
#include "tulip/ToolbarItem.h"

using namespace std;

namespace tlp {
InteractorToolbarItem::InteractorToolbarItem(list<Interactor *> interactors, QGraphicsItem *parent,QGraphicsScene *scene):
    _interactors(interactors) {
  _toolbar = new ToolbarItem();
  for (list<Interactor *>::iterator it = _interactors.begin(); it != _interactors.end(); ++it)
    _toolbar->addAction((*it)->getAction());
}

InteractorToolbarItem::~InteractorToolbarItem() {
}

void InteractorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}

QRectF InteractorToolbarItem::boundingRect() const {
  QRectF result(_toolbar->pos(),_toolbar->boundingRect().size());
  return result;
}

}
