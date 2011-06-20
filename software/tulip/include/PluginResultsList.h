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

public slots:
  void setTypeFilter(const QStringList &,bool autoRefresh=true);
  void setTypeFilter(const QString &,bool autoRefresh=true);
  void setNameFilter(const QString &,bool autoRefresh=true);
  void refreshResults();

protected slots:
  void changeFocus();
  void switchToDetailedInformations();
  void restoreResultsList();

private:
  tlp::PluginManager *_pluginManager;
  QWidget *_focusedItem;
  QWidget *_resultsListCache;

  QStringList _typeFilter;
  QString _nameFilter;

};

#endif // PLUGINRESULTSLIST_H
