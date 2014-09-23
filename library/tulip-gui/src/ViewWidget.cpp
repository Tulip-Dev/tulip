/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include "tulip/ViewWidget.h"

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <tulip/GlMainWidget.h>
#include <tulip/GlMainWidgetGraphicsItem.h>
#include <tulip/Interactor.h>

using namespace tlp;

struct MyGraphicsView: public QGraphicsView {
  QGraphicsItem* _centralItem;

  MyGraphicsView(): QGraphicsView(new QGraphicsScene()), _centralItem(NULL) {
    setAcceptDrops(false);
  }

  void resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);

    if (scene()) {
      scene()->setSceneRect(QRect(QPoint(0, 0), size()));
    }

    GlMainWidgetGraphicsItem* glMainWidgetItem = dynamic_cast<GlMainWidgetGraphicsItem*>(_centralItem);
    QGraphicsProxyWidget* proxyWidget = dynamic_cast<QGraphicsProxyWidget*>(_centralItem);

    if (glMainWidgetItem)
      glMainWidgetItem->resize(width(),height());
    else if (proxyWidget)
      proxyWidget->resize(width(),height());

    if (scene())
      scene()->update();

    // Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed when maximizing or minimizing the graphics view)
    QMouseEvent eventModif(QEvent::MouseMove,QPoint(size().width()/2, size().height()/2), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(this, &eventModif);
  }
};

ViewWidget::ViewWidget()
  : View(), _graphicsView(NULL), _centralWidget(NULL), _centralWidgetItem(NULL) {
}

ViewWidget::~ViewWidget() {
  delete _centralWidgetItem;
}

QGraphicsView* ViewWidget::graphicsView() const {
  return _graphicsView;
}

void ViewWidget::setupUi() {
  _graphicsView = new MyGraphicsView();
  _graphicsView->setFrameStyle(QFrame::NoFrame);
  _graphicsView->scene()->setBackgroundBrush(Qt::green);
  setupWidget();
  assert(_centralWidget);
}

void ViewWidget::currentInteractorChanged(tlp::Interactor *i) {
  i->install(_centralWidget);
}

void ViewWidget::graphDeleted(Graph *parentGraph) {
  setGraph(parentGraph);
}

void ViewWidget::setCentralWidget(QWidget* w,bool deleteOldCentralWidget) {
  assert(w);
  QGraphicsItem *oldCentralItem = _centralWidgetItem;
  QWidget *oldCentralWidget = _centralWidget;

  if (currentInteractor())
    currentInteractor()->uninstall();

  _centralWidget = w;

  if (currentInteractor())
    currentInteractor()->install(w);

  if (_centralWidgetItem)
    _graphicsView->scene()->removeItem(_centralWidgetItem);

  GlMainWidget *glMainWidget = dynamic_cast<GlMainWidget *>(w);

  if (glMainWidget) {
    _graphicsView->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    _graphicsView->setViewport(new GlMainWidget());
    _graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    GlMainWidgetGraphicsItem* glMainWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, _graphicsView->width(), _graphicsView->height());
    _centralWidgetItem = glMainWidgetItem;
    _graphicsView->scene()->addItem(_centralWidgetItem);
    glMainWidgetItem->resize(_graphicsView->width(),_graphicsView->height());
  }
  else {
    _graphicsView->setRenderHints(QPainter::TextAntialiasing);
    _graphicsView->setViewport(NULL);
    _graphicsView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    _centralWidgetItem = _graphicsView->scene()->addWidget(w);
    _centralWidget->resize(_graphicsView->width(),_graphicsView->height());
  }

  static_cast<MyGraphicsView*>(_graphicsView)->_centralItem = _centralWidgetItem;

  _centralWidgetItem->setPos(0,0);

  _centralWidgetItem->setZValue(0);

  refreshItemsParenthood();

  if(deleteOldCentralWidget) {
    delete oldCentralItem;
    delete oldCentralWidget;
  }
}

void ViewWidget::addToScene(QGraphicsItem *item) {
  if (_items.contains(item)) {
#ifndef NDEBUG
    qWarning("Trying to double-add an item to an AbstractGraphicsView");
#endif
    return;
  }

  _items.insert(item);
  item->setParentItem(_centralWidgetItem);
}

void ViewWidget::removeFromScene(QGraphicsItem *item) {
  if (!_items.contains(item))
    return;

  _items.remove(item);

  if (_graphicsView->scene())
    _graphicsView->scene()->removeItem(item);
}

void ViewWidget::refreshItemsParenthood() {
  for (QSet<QGraphicsItem *>::iterator it = _items.begin(); it != _items.end(); ++it) {
    QGraphicsItem *item = *it;
    item->setParentItem(_centralWidgetItem);
  }
}

QGraphicsItem* ViewWidget::centralItem() const {
  return _centralWidgetItem;
}

QPixmap ViewWidget::snapshot(const QSize &outputSize) const {
  if (_centralWidget == NULL)
    return QPixmap();

  QPixmap result(_centralWidget->size());
  _centralWidget->render(&result);



  if (outputSize.isValid()) {
    return result.scaled(outputSize);
  }

  return result;
}
