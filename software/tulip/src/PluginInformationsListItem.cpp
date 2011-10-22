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
#include "PluginInformationsListItem.h"
#include <QtGui/QPainter>
#include <tulip/PluginInformations.h>
#include "ui_PluginInformationsListItem.h"
#include <QtNetwork/QNetworkReply>

using namespace tlp;

PluginInformationsListItem::PluginInformationsListItem(tlp::PluginInformations *infos, QWidget *parent): QWidget(parent), _ui(new Ui::PluginInformationsListItemData), _pluginInformations(infos) {
  _ui->setupUi(this);
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color:" + backgroundCode() + ";  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");

  if(infos->isInstalled()) {
    _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-installed-updated.svg"));

    if(infos->updateAvailable()) {
      _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-upgrade.svg"));
    }
  }

  _ui->errorFrame->hide();
  _ui->downloadProgress->hide();
  _ui->icon->setPixmap(QPixmap(infos->iconPath()));
  _ui->name->setText("<p><span style=\"font-size:large;\"><b>" + infos->name() + "</b></span>");
  _ui->shortDescription->setText("<p><span style=\"font-size:small; color:#626262;\">" + infos->shortDescription() + "</span>");

  if (!hasFocus())
    _ui->bottomFrame->hide();

  connect(_ui->infosButton,SIGNAL(clicked()),this,SIGNAL(showDetailedInformations()));
  connect(_ui->installButton,SIGNAL(clicked()),this,SIGNAL(fetch()));
  connect(_ui->removeButton,SIGNAL(clicked()),this,SIGNAL(remove()));
  connect(_ui->removeButton,SIGNAL(clicked()),this, SLOT(markedForRemoval()));
  connect(infos, SIGNAL(DownloadStarted(QNetworkReply*)), this, SLOT(DownloadStarted(QNetworkReply*)));
}

void PluginInformationsListItem::focusInEvent(QFocusEvent *) {
  emit gotFocus();
}

void PluginInformationsListItem::expand() {
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color:" + backgroundCode() + ";  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");

  _ui->bottomFrame->show();

  if(!_ui->errorLabel->text().isEmpty()) {
    _ui->errorFrame->show();
  }

  _ui->installButton->setVisible(!_pluginInformations->isInstalled() || _pluginInformations->updateAvailable());
  _ui->removeButton->setVisible(_pluginInformations->isInstalled());

  _ui->infosButton->setIcon(QIcon(":/tulip/app/icons/help-contents.svg"));

  if (_pluginInformations->isInstalled() && _pluginInformations->updateAvailable()) {
    _ui->installButton->setIcon(QIcon(":/tulip/app/icons/package-upgrade.svg"));
    _ui->installButton->setText(trUtf8("Upgrade to ") + _pluginInformations->latestVersion());
  }
  else
    _ui->installButton->setIcon(QIcon(":/tulip/app/icons/list-add.svg"));

  _ui->removeButton->setIcon(QIcon(":/tulip/app/icons/package-purge.svg"));
}

void PluginInformationsListItem::collapse() {
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color:" + backgroundCode() + ";  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");

  _ui->bottomFrame->hide();
  _ui->errorFrame->hide();
}

void PluginInformationsListItem::markedForRemoval() {
  _ui->removeButton->setEnabled(false);
  _ui->removeButton->setText("Plugin has been marked for removal");
}

void PluginInformationsListItem::DownloadStarted(QNetworkReply* reply) {
  _ui->downloadProgress->show();
  connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
  connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

void PluginInformationsListItem::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
  _ui->downloadProgress->setMaximum(bytesTotal);
  _ui->downloadProgress->setValue(bytesReceived);
}

void PluginInformationsListItem::downloadFinished() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

  if(reply) {
    _ui->downloadProgress->hide();

    if(reply->error() == QNetworkReply::NoError) {
      _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-installed-updated.svg"));
      _ui->installButton->hide();
      _ui->removeButton->show();
    }
    else {
      _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-purge.svg"));
      _ui->errorFrame->show();
      _ui->errorLabel->setText(qPrintable(reply->errorString()));
    }
  }
}

QString PluginInformationsListItem::backgroundCode() {
  if (hasFocus())
    return "rgb(232, 238, 244)";
  else if (_pluginInformations->isInstalled() && !_pluginInformations->updateAvailable())
    return "#E9E9E9";

  return "white";
}
