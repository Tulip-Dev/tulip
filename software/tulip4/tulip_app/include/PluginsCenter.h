#ifndef PLUGINSCENTER_
#define PLUGINSCENTER_

#include <QtGui/QWidget>
#include "ui_PluginsCenter.h"

class PluginsCenter: public QWidget {
  Q_OBJECT
public:
  explicit PluginsCenter(QWidget *parent=0);

  void setPluginsError(const QMap<QString,QString> &);

public slots:
  void showHomePage();
  void showSearchPage();
  void showErrorsPage();
  void showDownloadsPage();
  void showReposPage();

protected slots:
  void showPage(QWidget *page);
  void listItemSelected();

private:
  Ui::PluginsCenterData *_ui;
  QMap<QString,QString> _pluginsErrors;
};

#endif // PLUGINSCENTER_
