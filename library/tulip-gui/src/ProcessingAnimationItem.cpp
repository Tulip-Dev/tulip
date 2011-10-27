#include "tulip/ProcessingAnimationItem.h"
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionGraphicsItem>

ProcessingAnimationItem::ProcessingAnimationItem(const QPixmap &pixmap, const QSize &size, QGraphicsItem *parent, QGraphicsScene *scene)
  : QGraphicsPixmapItem(parent,scene), _currentFrame(0) {
  int index=0;
  for (int y=0;y<pixmap.height();y+=size.height()) {
    for (int x=0;x<pixmap.width();x+=size.width()) {
      _pixmaps.push_back(pixmap.copy(x,y,size.width(),size.height()));
    }
  }

  _animationTimer.setInterval(50);
  _animationTimer.setSingleShot(false);
  connect(&_animationTimer,SIGNAL(timeout()),this,SLOT(animationTimeout()));
  _animationTimer.start();
}

ProcessingAnimationItem::~ProcessingAnimationItem() {
}

void ProcessingAnimationItem::animationTimeout() {
  _currentFrame = (_currentFrame+1)%_pixmaps.size();
  setPixmap(_pixmaps[_currentFrame]);
  update();
}
