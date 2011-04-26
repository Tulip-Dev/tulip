#ifndef CONFIGURATIONWIDGETCHOOSERITEM_H
#define CONFIGURATIONWIDGETCHOOSERITEM_H

#include <QtGui/QGraphicsItemGroup>
#include <QtCore/QObject>

class QGraphicsProxyWidget;
namespace tlp {
class PushButtonItem;

class ConfigurationWidgetChooserItem: public QObject, public QGraphicsItemGroup {
Q_OBJECT
public:
  ConfigurationWidgetChooserItem(const std::list<std::pair<QWidget *,std::string> > &configurationWidgets, QGraphicsItem *parent=0);
  virtual ~ConfigurationWidgetChooserItem();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  QRectF boundingRect() const;

  QVariant itemChange(GraphicsItemChange change, const QVariant &value);

protected slots:
  void resetPos();
  void toggleWidgetList();

private:
  std::list<std::pair<QWidget *, std::string> > _configurationWidgets;

  PushButtonItem *_settingsButton;
  QGraphicsProxyWidget *_widgetList;
  QGraphicsProxyWidget *_visibleWidget;

  QColor _bgColor;
  QColor _borderColor;
};

}
#endif // CONFIGURATIONWIDGETCHOOSERITEM_H
