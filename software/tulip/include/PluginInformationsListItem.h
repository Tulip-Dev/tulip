#ifndef PLUGININFORMATIONSLISTITEM_H
#define PLUGININFORMATIONSLISTITEM_H

#include <QtGui/QWidget>

namespace Ui {
class PluginInformationsListItemData;
}
namespace tlp {
class PluginInformations;
}

class PluginInformationsListItem: public QWidget {
  Q_OBJECT
public:
  explicit PluginInformationsListItem(tlp::PluginInformations *, QWidget *parent = 0);
  tlp::PluginInformations *pluginInformations() const { return _pluginInformations; }

protected:
  void focusInEvent(QFocusEvent *);

signals:
  void gotFocus();
  void showDetailedInformations();

public slots:
  void expand();
  void collapse();

private:
  Ui::PluginInformationsListItemData *_ui;
  tlp::PluginInformations *_pluginInformations;

};

#endif // PLUGININFORMATIONSLISTITEM_H
