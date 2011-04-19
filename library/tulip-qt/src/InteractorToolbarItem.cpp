#include "tulip/InteractorToolbarItem.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QApplication>

#include <tulip/Interactor.h>
#include "tulip/ToolbarItem.h"

using namespace std;

namespace tlp {
InteractorToolbarItem::InteractorToolbarItem(list<Interactor *> interactors, QGraphicsItem *parent,QGraphicsScene *scene):
    QGraphicsItemGroup(parent,scene), _interactors(interactors) {
}

InteractorToolbarItem::~InteractorToolbarItem() {
}

void InteractorToolbarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//  if (_snapArea == Qt::TopToolBarArea || _snapArea == Qt::LeftToolBarArea)
//    _toolbar->setPos(_margin,_margin);
//  else if (_snapArea == Qt::BottomToolBarArea)
//    _toolbar->setPos(_margin,_margin+_round+20);
//  else
//    _toolbar->setPos(_margin+_round+20,_margin);

//  painter->setBrush(QColor(0,255,0,50));
//  painter->drawRect(boundingRect());

//  QColor fgColor = QApplication::palette().color(QPalette::Shadow);
//  QColor bgColor = QApplication::palette().color(QPalette::Window);
//  fgColor.setAlpha(150);
//  bgColor.setAlpha(150);
//  painter->setPen(QPen(fgColor));
//  painter->setBrush(bgColor);

//  QRectF tbrect = toolbarBoundingRect();
//  QRectF bgRect;
//  if (_snapArea == Qt::TopToolBarArea) {
//    bgRect = QRectF(0,-_round*1.5,tbrect.width(),tbrect.height()+_round);
//    _configureRect = QRectF(tbrect.width()/2 - _toolbar->iconSize().width()/2, tbrect.height(), _toolbar->iconSize().width(),10);
//  }
//  else if (_snapArea == Qt::BottomToolBarArea) {
//    bgRect = QRectF(0,_round/2,tbrect.width(),tbrect.height()+_round);
//     _configureRect = QRectF(tbrect.width()/2 - _toolbar->iconSize().width()/2, 0, _toolbar->iconSize().width(),10);
//  }
//  else if (_snapArea == Qt::LeftToolBarArea) {
//    bgRect = QRectF(-_round*1.5,0,tbrect.width()+_round,tbrect.height());
//     _configureRect = QRectF(tbrect.width(), tbrect.height() / 2 - _toolbar->iconSize().height()/2, 10,_toolbar->iconSize().height());
//  }
//  else {
//    bgRect = QRectF(_round/2,0,tbrect.width()+_round,tbrect.height());
//    _configureRect = QRectF(0, tbrect.height() / 2 - _toolbar->iconSize().height()/2, 10,_toolbar->iconSize().height());
//  }
}

QRectF InteractorToolbarItem::boundingRect() const {
}

}
