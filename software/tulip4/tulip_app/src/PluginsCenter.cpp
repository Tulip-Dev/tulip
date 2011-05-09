#include "PluginsCenter.h"
#include "PluginErrorReport.h"

#include "ui_PluginsCenter.h"

PluginsCenter::PluginsCenter(QWidget *parent) :
    QWidget(parent), _ui(new Ui::PluginsCenterData()) {
  _ui->setupUi(this);

  connect(_ui->repoButton,SIGNAL(clicked()),this,SLOT(showReposPage()));
  connect(_ui->homeButton,SIGNAL(clicked()),this,SLOT(showHomePage()));
  connect(_ui->pluginsSideList,SIGNAL(itemSelectionChanged()),this,SLOT(listItemSelected()));
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
  showPage(_ui->searchPage);
}

void PluginsCenter::showReposPage() {
  showPage(_ui->reposPage);
}

void PluginsCenter::showPage(QWidget *page) {
  QStackedWidget *stackedParent = dynamic_cast<QStackedWidget *>(parent());
  if (!stackedParent)
    return;
  stackedParent->setCurrentWidget(this);
  _ui->pluginsContent->setCurrentWidget(page);
}

void PluginsCenter::listItemSelected() {
  if (_ui->pluginsSideList->currentRow() == 6)
    showErrorsPage();
}

void PluginsCenter::setPluginsError(const QMap<QString, QString> &errors) {
  QListWidgetItem *errorsItem = _ui->pluginsSideList->item(6);
  if (errors.isEmpty())
    errorsItem->setFlags(!Qt::ItemIsEnabled);
  else {
    QFont f = errorsItem->font();
    f.setBold(true);
    errorsItem->setFont(f);
  }

  for (QMap<QString,QString>::const_iterator it = errors.begin(); it != errors.end(); ++it)
    _ui->errorsLogAreaLayout->addWidget(new PluginErrorReport(it.key(),it.value()));
}
