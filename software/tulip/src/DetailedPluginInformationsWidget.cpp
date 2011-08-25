#include "DetailedPluginInformationsWidget.h"

#include <QtWebKit/QWebFrame>
#include <tulip/PluginInformations.h>
#include "ui_DetailedPluginInformationsWidget.h"

DetailedPluginInformationsWidget::DetailedPluginInformationsWidget(tlp::PluginInformations *infos,QWidget *parent)
  : QWidget(parent), _ui(new Ui::DetailedPluginInformationsWidgetData), _pluginInformations(infos) {
  _ui->setupUi(this);
  _ui->icon->setPixmap(QPixmap(infos->iconPath()));
  _ui->nameAndAuthor->setText("<span style=\"font-size: large;\"><b>" + infos->name() + "</b></span><span style=\"color:#626262;\"> by " + infos->author() + "</span>");
  _ui->shortDescriptionAndVersion->setText("<span style=\"color:#626262; font-size:small\">" + infos->shortDescription() + "</span>");

  _ui->installButton->setVisible(!infos->isInstalled());
  _ui->upgradeButton->setVisible(infos->isInstalled() && infos->updateAvailable());
  _ui->removeButton->setVisible(infos->isInstalled());
  _ui->installButton->setText(_ui->installButton->text() + "v" + infos->latestVersion());
  _ui->upgradeButton->setText(_ui->upgradeButton->text() + "v" + infos->latestVersion());

  _ui->detailedDescriptionView->setUrl(infos->longDescriptionPath());
  _ui->detailedDescriptionView->adjustSize();
  _ui->detailedDescriptionView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  _ui->detailedDescriptionView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  connect(_ui->backButton,SIGNAL(clicked()),this,SIGNAL(goBack()));
  connect(_ui->installButton,SIGNAL(clicked()),this,SIGNAL(fetch()));
  connect(_ui->removeButton,SIGNAL(clicked()),this,SIGNAL(remove()));
}
