#include "PluginsCenter.h"
#include "PluginErrorReport.h"

#include "ui_PluginsCenter.h"

#include <tulip/PluginManager.h>

PluginsCenter::PluginsCenter(QWidget *parent) :
    QWidget(parent), _ui(new Ui::PluginsCenterData()) {
  _ui->setupUi(this);
  _ui->pluginDetailsFrame->setVisible(false);

  connect(_ui->pluginsSearchList,SIGNAL(showHtml(QString)),this,SLOT(displayHtmlDescription(QString)));
  connect(_ui->pluginsSearchList,SIGNAL(hideHtml()),_ui->pluginDetailsFrame,SLOT(hide()));
  connect(_ui->repoButton,SIGNAL(clicked()),this,SLOT(showReposPage()));
  connect(_ui->homeButton,SIGNAL(clicked()),this,SLOT(showHomePage()));
  connect(_ui->pluginsSideList,SIGNAL(itemSelectionChanged()),this,SLOT(listItemSelected()));
  connect(_ui->browseAlgorithmsButton,SIGNAL(clicked()),this,SLOT(browseAlgorithms()));
  connect(_ui->browseImportExportButton,SIGNAL(clicked()),this,SLOT(browseImportExport()));
  connect(_ui->browseGlyphsButton,SIGNAL(clicked()),this,SLOT(browseGlyphs()));
  connect(_ui->browseViewsButton,SIGNAL(clicked()),this,SLOT(browseViews()));
  connect(_ui->browseInteractorsButton,SIGNAL(clicked()),this,SLOT(browseInteractors()));
  connect(_ui->browsePerspectivesButton,SIGNAL(clicked()),this,SLOT(browsePerspectives()));
}

void PluginsCenter::showDownloadsPage() {
  showPage(_ui->downloadsPage);
}

void PluginsCenter::showErrorsPage() {
  showPage(_ui->errorsPage);
}

void PluginsCenter::showHomePage() {
  showPage(_ui->homePage);
}

void PluginsCenter::showSearchPage() {
  showPage(_ui->pluginsListPage);
}

void PluginsCenter::showReposPage() {
  showPage(_ui->reposPage);
}

void PluginsCenter::showPage(QWidget *page) {
  QStackedWidget *stackedParent = static_cast<QStackedWidget *>(parent());
  if (!stackedParent)
    return;
  stackedParent->setCurrentWidget(this);
  _ui->pluginsContent->setCurrentWidget(page);
}

void PluginsCenter::listItemSelected() {
  int row = _ui->pluginsSideList->currentRow();
  switch (row) {
  case 0:
    browseAll();
    break;
  case 1:
    browseAlgorithms();
    break;
  case 2:
    browseImportExport();
    break;
  case 3:
    browseGlyphs();
    break;
  case 4:
    browseViews();
    break;
  case 5:
    browseInteractors();
    break;
  case 6:
    browsePerspectives();
    break;
  case 7: // separator
    break;
  case 8:
    showErrorsPage();
    break;
  case 9:
    showDownloadsPage();
    break;
  }
}

void PluginsCenter::reportPluginError(const QString &filename, const QString &errormsg) {
  _ui->errorsLogAreaLayout->addWidget(new PluginErrorReport(filename,errormsg));
  QListWidgetItem *errorsItem = _ui->pluginsSideList->item(8);
  errorsItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  QFont f = errorsItem->font();
  f.setBold(true);
  errorsItem->setFont(f);
}

void PluginsCenter::browseAll() {
  _ui->pluginsSearchList->setTypeFilter(QStringList());
  showSearchPage();
}

void PluginsCenter::browseAlgorithms() {
  _ui->pluginsSearchList->setTypeFilter(QStringList() << ALGORITHM_PLUGIN_NAME << PROPERTY_ALGORITHM_PLUGIN_NAME);
  showSearchPage();
}

void PluginsCenter::browseImportExport() {
  _ui->pluginsSearchList->setTypeFilter(QStringList() << IMPORT_PLUGIN_NAME << EXPORT_PLUGIN_NAME);
  showSearchPage();
}

void PluginsCenter::browseGlyphs() {
  _ui->pluginsSearchList->setTypeFilter(QStringList() << EE_GLYPH_PLUGIN_NAME << GLYPH_PLUGIN_NAME);
  showSearchPage();
}

void PluginsCenter::browseViews() {
  _ui->pluginsSearchList->setTypeFilter(VIEW_PLUGIN_NAME);
  showSearchPage();
}

void PluginsCenter::browseInteractors() {
  _ui->pluginsSearchList->setTypeFilter(INTERACTOR_PLUGIN_NAME);
  showSearchPage();
}

void PluginsCenter::browsePerspectives() {
  _ui->pluginsSearchList->setTypeFilter(PERSPECTIVE_PLUGIN_NAME);
  showSearchPage();
}

void PluginsCenter::displayHtmlDescription(const QString &url) {
  _ui->pluginDetailsPage->setUrl(url);
  _ui->pluginDetailsFrame->setVisible(true);
  _ui->pluginDetailsPage->setVisible(true);
}
