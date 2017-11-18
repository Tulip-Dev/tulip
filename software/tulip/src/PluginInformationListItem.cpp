/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include "PluginInformationListItem.h"
#include "ui_PluginInformationListItem.h"

#include <tulip/TlpTools.h>
#include <tulip/TlpQtTools.h>

#include <QPainter>
#include <QNetworkReply>

using namespace tlp;

PluginInformationListItem::PluginInformationListItem(PluginInformation info, QWidget *parent)
    : QWidget(parent), _ui(new Ui::PluginInformationListItemData), _info(info) {
  _ui->setupUi(this);
  _ui->progressFrame->hide();
  _ui->installFrame->hide();
  _ui->removeFrame->hide();
  _ui->rebootFrame->hide();
  _ui->installationControls->hide();

  PluginVersionInformation versionInfo(info.installedVersion);

  if (!versionInfo.isValid) {
    versionInfo = info.availableVersion;
    _ui->statusIcon->hide();
    _ui->installFrame->show();
  } else {
    if (info.availableVersion.isValid && info.availableVersion.version != versionInfo.version) {
      _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/16/package-upgrade.png"));
      _ui->installFrame->show();
    }

    else
      _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/16/package-installed-updated.png"));

    // only show the remove frame for downloaded plugins
    if (!versionInfo.libraryLocation.isEmpty() &&
        !versionInfo.libraryLocation.contains(tlp::TulipLibDir.c_str())) {
      _ui->removeFrame->show();
    }
  }

  if (!versionInfo.icon.isEmpty())
    _ui->icon->setPixmap(QPixmap(versionInfo.icon).scaled(32, 32));

  _ui->name->setText(info.name + " " + versionInfo.version);
  _ui->desc->setText(versionInfo.description + "\n\n" + trUtf8("Author: ") + versionInfo.author);
  _ui->installButton->setText(trUtf8("Install ") + info.availableVersion.version);
}

PluginInformationListItem::~PluginInformationListItem() {
  delete _ui;
}

QWidget *PluginInformationListItem::description() {
  return _ui->desc;
}

void PluginInformationListItem::focusOut() {
  _ui->installationControls->hide();
  _ui->contentsFrame->setProperty("highlighted", false);
  _ui->contentsFrame->setStyleSheet(_ui->contentsFrame->styleSheet());
}

void PluginInformationListItem::focusIn() {
  _ui->installationControls->show();
  _ui->contentsFrame->setProperty("highlighted", true);
  _ui->contentsFrame->setStyleSheet(_ui->contentsFrame->styleSheet());
}

void PluginInformationListItem::install() {
  _ui->installFrame->hide();
  _ui->progressFrame->show();
  PluginManager::markForInstallation(_info.name, this, SLOT(downloadProgress(qint64, qint64)));
  _ui->rebootFrame->show();
}

void PluginInformationListItem::remove() {
  _ui->removeFrame->hide();
  PluginManager::markForRemoval(_info.installedVersion.libraryLocation);
  _ui->rebootFrame->show();
}

void PluginInformationListItem::enterEvent(QEvent *) {
  emit focused();
}

void PluginInformationListItem::downloadProgress(qint64 received, qint64 total) {
  _ui->progressBar->setMaximum(total);
  _ui->progressBar->setValue(received);
}
