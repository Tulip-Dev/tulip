#include "tulip/ViewWidget.h"

#include <QtGui/QGraphicsProxyWidget>
#include <tulip/GlMainWidget.h>
#include <tulip/GlMainWidgetGraphicsItem.h>

using namespace tlp;

ViewWidget::ViewWidget():_widgetItem(NULL) {
}

ViewWidget::~ViewWidget() {
}

void ViewWidget::setupUi() {
  setupWidget();

  QWidget* w = widget();
  GlMainWidget* mainWidget = dynamic_cast<GlMainWidget*>(w);

  if (mainWidget != NULL) {
    GlMainWidgetGraphicsItem* glItem = new GlMainWidgetGraphicsItem(mainWidget,QSize(10,10));
    _widgetItem = glItem;
  }
  else {
    QGraphicsProxyWidget* proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(widget());
    _widgetItem = proxyWidget;
  }
}

QGraphicsItem* ViewWidget::graphicsItem() const {
  return _widgetItem;
}

void ViewWidget::activeInteractorChanged(tlp::Interactor *i) {

}

void ViewWidget::resize(const QSizeF& size) {
  GlMainWidgetGraphicsItem* glMainWidgetItem = dynamic_cast<GlMainWidgetGraphicsItem*>(_widgetItem);

  if (glMainWidgetItem != NULL)
    glMainWidgetItem->resize(QSize(size.width(),size.height()));
  else {
    QGraphicsProxyWidget* proxy = static_cast<QGraphicsProxyWidget*>(_widgetItem);
    proxy->resize(size);
  }
}
