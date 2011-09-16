#ifndef PLUGINSCENTER_
#define PLUGINSCENTER_

#include <QtGui/QWidget>
#include <QtCore/QMap>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class PluginsCenterData;
}

namespace tlp {
class PluginInformations;
}

/**
  @brief The widget managing plugins installation.
  This widget is meant to list plugins available from several plugins repositories. Order them by categories and provide with a way to easily download and install them.
  The plugin center also displays detailed information about each plugin.

  @see PluginInformations
  */
class PluginsCenter: public QWidget {
  Q_OBJECT
public:
  explicit PluginsCenter(QWidget *parent=0);

public slots:
  void showHomePage();
  void showSearchPage();
  void showErrorsPage();
  void showReposPage();
  void reportPluginErrors(const QMap<QString,QString> &);
  void fetch(tlp::PluginInformations *);
  void remove(tlp::PluginInformations *);
  void addRemoteLocation(const QString &url=QString::null);
  void removeRemoteLocation();
  void removeRemoteLocation(const QString &url);

protected slots:
  void showPage(QWidget *page);
  void listItemSelected();

  void browseAll();
  void browseAlgorithms();
  void browseImportExport();
  void browseGlyphs();
  void browseViews();
  void browseInteractors();
  void browsePerspectives();

  void setPluginNameFilter(const QString &);
  void remoteLocationAdded(const QString &);
  void remoteLocationError(QNetworkReply::NetworkError,const QString &);

private:
  QVector<const char *> _typeSlots;
  Ui::PluginsCenterData *_ui;
  QStringList indexType(int) const;
};

#endif // PLUGINSCENTER_
