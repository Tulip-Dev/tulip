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

protected:
  void focusInEvent(QFocusEvent *);

signals:
  void gotFocus();

public slots:
  void expand();
  void collapse();

private:
  Ui::PluginInformationsListItemData *_ui;

};

#endif // PLUGININFORMATIONSLISTITEM_H
