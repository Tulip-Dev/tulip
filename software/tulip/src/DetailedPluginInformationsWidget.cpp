#include "DetailedPluginInformationsWidget.h"

#include <QtWebKit/QWebFrame>
#include <tulip/PluginInformations.h>
#include "ui_DetailedPluginInformationsWidget.h"

DetailedPluginInformationsWidget::DetailedPluginInformationsWidget(tlp::PluginInformations *infos,QWidget *parent)
  : QWidget(parent), _ui(new Ui::DetailedPluginInformationsWidgetData) {
  _ui->setupUi(this);
  _ui->icon->setPixmap(QPixmap(infos->iconPath()));
  _ui->nameAndAuthor->setText("<span style=\"font-size: large;\"><b>" + infos->name() + "</b></span><span style=\"color:#626262;\"> by " + infos->author() + "</span>");
  _ui->shortDescriptionAndVersion->setText("<span style=\"color:#626262; font-size:small\">" + infos->shortDescription() + "</span>");
  _ui->instalButton->setText("Install");
  _ui->instalButton->setEnabled(!infos->isInstalled() || infos->updateAvailable());
  _ui->removeButton->setEnabled(infos->isInstalled());
  if (infos->isInstalled() && infos->updateAvailable())
    _ui->instalButton->setText("Update to");
  _ui->instalButton->setText(_ui->instalButton->text() + " " + infos->latestVersion());
  _ui->detailedDescriptionView->setUrl(infos->longDescriptionPath());
  _ui->detailedDescriptionView->adjustSize();
  _ui->detailedDescriptionView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
  _ui->detailedDescriptionView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
  connect(_ui->backButton,SIGNAL(clicked()),this,SIGNAL(goBack()));
}
