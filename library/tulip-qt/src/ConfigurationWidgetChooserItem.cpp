#include "tulip/ConfigurationWidgetChooserItem.h"

#include "tulip/PushButtonItem.h"
#include <QtGui/QAction>
#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsProxyWidget>

using namespace std;

namespace tlp {
ConfigurationWidgetChooserItem::ConfigurationWidgetChooserItem(const std::list<std::pair<QWidget *, std::string> > &configurationWidgets, QGraphicsItem *parent):
  QObject(0), QGraphicsItemGroup(parent),_configurationWidgets(configurationWidgets),
  _settingsButton(0),_visibleWidget(0),_widgetList(0),
  _bgColor(QApplication::palette().color(QPalette::Window)), _borderColor(QApplication::palette().color(QPalette::Mid)) {

  _bgColor.setAlpha(200);
  _borderColor.setAlpha(200);
  setHandlesChildEvents(false);

  _settingsButton = new PushButtonItem(new QAction(QIcon(":/tulip/icons/document-properties.svg"),trUtf8("Toggle view's configuration widgets"),0),QSize(24,24),this,true);
  _settingsButton->setButtonShape(PushButtonItem::SquareShape);
//  _settingsButton->setBackgroundColor(_bgColor);
//  _settingsButton->setBorderColor(_borderColor);
//  _settingsButton->setBorderWidth(2);
  _settingsButton->setFadeout(false);
}

ConfigurationWidgetChooserItem::~ConfigurationWidgetChooserItem() {
}

void ConfigurationWidgetChooserItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}

QRectF ConfigurationWidgetChooserItem::boundingRect() const {
  QSizeF size(0,0);
  size+=_settingsButton->boundingRect().size();
  return QRectF(QPointF(0,0),size);
}

QVariant ConfigurationWidgetChooserItem::itemChange(GraphicsItemChange change, const QVariant &value) {
  if (change == QGraphicsItem::ItemSceneHasChanged && scene()) {
    connect(scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(resetPos()));
    resetPos();
  }
  else if (change == QGraphicsItem::ItemSceneChange && scene())
    disconnect(scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(resetPos()));
  return QGraphicsItemGroup::itemChange(change,value);
}

void ConfigurationWidgetChooserItem::resetPos() {
  if (!scene())
    return;
  QRectF brect(boundingRect());
  setPos(scene()->width()-brect.width(),scene()->height()-brect.height());
}

void ConfigurationWidgetChooserItem::toggleWidgetList() {
}

}
