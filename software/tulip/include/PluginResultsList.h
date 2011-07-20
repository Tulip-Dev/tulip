#ifndef PLUGINRESULTSLIST_H
#define PLUGINRESULTSLIST_H

#include <QtGui/QScrollArea>
#include <QtCore/QMap>

namespace tlp {
class PluginManager;
class PluginInformations;
}
class PluginInformationsListItem;

class PluginResultsList : public QScrollArea {
  Q_OBJECT
public:
  explicit PluginResultsList(QWidget *parent=0);

public slots:
  void setTypeFilter(const QStringList &,bool autoRefresh=true);
  void setTypeFilter(const QString &,bool autoRefresh=true);
  void setNameFilter(const QString &,bool autoRefresh=true);
  void initPluginsCache();
  void refreshResults();

protected slots:
  void changeFocus();
  void switchToDetailedInformations();
  void restoreResultsList();

  void pluginFetch();
  void pluginRemove();

signals:
  void fetch(tlp::PluginInformations *);
  void remove(tlp::PluginInformations *);

private:
  QWidget *_focusedItem;
  QWidget *_resultsListCache;
  QMap<tlp::PluginInformations *,PluginInformationsListItem *> _pluginWidgetsCache;

  QStringList _typeFilter;
  QString _nameFilter;
};

#endif // PLUGINRESULTSLIST_H
