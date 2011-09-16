#include "PluginInformationsListItem.h"
#include <QtGui/QPainter>
#include <tulip/PluginInformations.h>
#include "ui_PluginInformationsListItem.h"
#include <QtNetwork/QNetworkReply>

using namespace tlp;

PluginInformationsListItem::PluginInformationsListItem(tlp::PluginInformations *infos, QWidget *parent): QWidget(parent), _ui(new Ui::PluginInformationsListItemData), _pluginInformations(infos) {
  _ui->setupUi(this);

  if(infos->isInstalled()) {
    _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-installed-updated.svg"));

    if(infos->updateAvailable()) {
      _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-upgrade.svg"));
    }
  }

  _ui->errorFrame->hide();
  _ui->downloadProgress->hide();

  _ui->installButton->setVisible(!infos->isInstalled() || infos->updateAvailable());
  if (infos->isInstalled() && infos->updateAvailable()) {
    _ui->installButton->setIcon(QIcon(":/tulip/app/icons/package-upgrade.svg"));
    _ui->installButton->setText(trUtf8("Upgrade to ") + infos->latestVersion());
  }

  _ui->removeButton->setVisible(infos->isInstalled());

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
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(225, 225, 225, 255), stop:1 rgba(205,205,205, 255));  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");
  _ui->bottomFrame->show();

  if(!_ui->errorLabel->text().isEmpty()) {
    _ui->errorFrame->show();
  }
}

void PluginInformationsListItem::collapse() {
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(245, 245, 245, 255), stop:1 rgba(225,225,225, 255));  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");
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
