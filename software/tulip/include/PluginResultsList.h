#ifndef PLUGINRESULTSLIST_H
#define PLUGINRESULTSLIST_H

#include <QtGui/QWidget>

namespace tlp {
class PluginManager;
}

class PluginResultsList : public QWidget {
  Q_OBJECT
public:
  explicit PluginResultsList(QWidget *parent=0);
  tlp::PluginManager *pluginManager() const { return _pluginManager; }

signals:

public slots:
  void setTypeFilter(const QString &);
  void setNameFilter(const QString &);
  void refreshResults();

protected slots:
  void changeFocus();

private:
  tlp::PluginManager *_pluginManager;
  QWidget *_focusedItem;

};

#endif // PLUGINRESULTSLIST_H
