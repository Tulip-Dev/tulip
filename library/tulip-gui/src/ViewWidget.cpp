/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

struct TulipGraphicsView : public QGraphicsView {
  ViewWidget *_viewWidget;
  QGraphicsItem *_centralItem;

  TulipGraphicsView(ViewWidget *w) :
    QGraphicsView(new QGraphicsScene()), _viewWidget(w), _centralItem(nullptr) {
    setAcceptDrops(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }

  ~TulipGraphicsView() override {
    delete scene();
  }

  void resizeEvent(QResizeEvent *event) override {
    QGraphicsView::resizeEvent(event);

    if (scene()) {
      scene()->setSceneRect(QRect(QPoint(0, 0), size()));
    }

    GlMainWidgetGraphicsItem *glMainWidgetItem =
        dynamic_cast<GlMainWidgetGraphicsItem *>(_centralItem);
    QGraphicsProxyWidget *proxyWidget = dynamic_cast<QGraphicsProxyWidget *>(_centralItem);

    if (glMainWidgetItem)
      glMainWidgetItem->resize(width(), height());
    else if (proxyWidget)
      proxyWidget->resize(width(), height());

    _viewWidget->graphicsViewResized(width(), height());

    if (scene())
      scene()->update();

    // Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed
    // when maximizing or minimizing the graphics view)
    QMouseEvent eventModif(QEvent::MouseMove, QPoint(size().width() / 2, size().height() / 2),
                           Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    QApplication::sendEvent(this, &eventModif);
  }
};

ViewWidget::ViewWidget()
    : View(), _graphicsView(nullptr), _centralWidget(nullptr), _centralWidgetItem(nullptr) {}

ViewWidget::~ViewWidget() {
  // ensure to uninstall event filter of current interactor to
  // avoid possible crashes when closing the view
  if (currentInteractor()) {
    currentInteractor()->uninstall();
  }

  if (_centralWidgetItem) {
    _graphicsView->scene()->removeItem(_centralWidgetItem);
    delete _centralWidgetItem;
  }
}

QGraphicsView *ViewWidget::graphicsView() const {
  return _graphicsView;
}

void ViewWidget::setupUi() {
  // instantiate the graphicsView
  // (will be deleted by the WorkspacePanel destructor)
  _graphicsView = new TulipGraphicsView(this);
  _graphicsView->setFrameStyle(QFrame::NoFrame);
  _graphicsView->scene()->setBackgroundBrush(Qt::white);
  setupWidget();
  assert(_centralWidget);
}

void ViewWidget::currentInteractorChanged(tlp::Interactor *i) {
  if (i)
    i->install(_centralWidget);
}

void ViewWidget::graphDeleted(Graph *parentGraph) {
  setGraph(parentGraph);
}

void ViewWidget::setCentralWidget(QWidget *w, bool deleteOldCentralWidget) {
  assert(w);
  QGraphicsItem *oldCentralItem = _centralWidgetItem;

  if (currentInteractor())
    currentInteractor()->uninstall();

  _centralWidget = w;

  if (currentInteractor())
    currentInteractor()->install(w);

  GlMainWidget *glMainWidget = dynamic_cast<GlMainWidget *>(w);

  if (glMainWidget) {
    _graphicsView->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing |
                                  QPainter::TextAntialiasing);
    _graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    GlMainWidgetGraphicsItem *glMainWidgetItem =
        dynamic_cast<GlMainWidgetGraphicsItem *>(_centralWidgetItem);

    if (glMainWidgetItem) {
      deleteOldCentralWidget = false;
      glMainWidgetItem->setGlMainWidget(glMainWidget);
    } else {
      glMainWidgetItem = new GlMainWidgetGraphicsItem(glMainWidget, _graphicsView->width(),
                                                      _graphicsView->height());

      if (_centralWidgetItem)
        _graphicsView->scene()->removeItem(_centralWidgetItem);

      _centralWidgetItem = glMainWidgetItem;
      _graphicsView->scene()->addItem(_centralWidgetItem);
    }

    glMainWidgetItem->resize(_graphicsView->width(), _graphicsView->height());
  } else {
    _graphicsView->setRenderHints(QPainter::TextAntialiasing);
    _graphicsView->setViewport(nullptr);
    _graphicsView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    _centralWidgetItem = _graphicsView->scene()->addWidget(w);
    _centralWidget->resize(_graphicsView->width(), _graphicsView->height());
  }

  static_cast<TulipGraphicsView *>(_graphicsView)->_centralItem = _centralWidgetItem;

  _centralWidgetItem->setPos(0, 0);

  _centralWidgetItem->setZValue(0);

  refreshItemsParenthood();

  if (deleteOldCentralWidget) {
    delete oldCentralItem;
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

QGraphicsItem *ViewWidget::centralItem() const {
  return _centralWidgetItem;
}

QPixmap ViewWidget::snapshot(const QSize &outputSize) const {
  if (_centralWidget == nullptr)
    return QPixmap();

  QPixmap result(_centralWidget->size());
  _centralWidget->render(&result);

  if (outputSize.isValid()) {
    return result.scaled(outputSize);
  }

  return result;
}
