#ifndef PLUGINRESULTSLIST_H
#define PLUGINRESULTSLIST_H

#include <QtGui/QScrollArea>

namespace tlp {
class PluginManager;
}

class PluginResultsList : public QScrollArea {
  Q_OBJECT
public:
  explicit PluginResultsList(QWidget *parent=0);
  tlp::PluginManager *pluginManager() const { return _pluginManager; }

signals:
  void showHtml(const QString &);
  void hideHtml();

public slots:
  void setTypeFilter(const QStringList &,bool autoRefresh=true);
  void setTypeFilter(const QString &,bool autoRefresh=true);
  void setNameFilter(const QString &,bool autoRefresh=false);
  void refreshResults();

protected slots:
  void changeFocus();

private:
  tlp::PluginManager *_pluginManager;
  QWidget *_focusedItem;

  QStringList _typeFilter;
  QString _nameFilter;

};

#endif // PLUGINRESULTSLIST_H
