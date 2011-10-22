/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
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
  connect(_ui->installButton,SIGNAL(clicked()),this,SLOT(fetchButtonClicked()));
  connect(_ui->removeButton,SIGNAL(clicked()),this,SLOT(removeButtonClicked()));
}

void DetailedPluginInformationsWidget::hideNavigationBar() {
  _ui->topFrame->hide();
}

void DetailedPluginInformationsWidget::fetchButtonClicked() {
  emit fetch(_pluginInformations);
}

void DetailedPluginInformationsWidget::removeButtonClicked() {
  emit remove(_pluginInformations);
}
