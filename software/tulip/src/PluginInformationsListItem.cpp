#include "PluginInformationsListItem.h"
#include <QtGui/QPainter>
#include <tulip/PluginInformations.h>
#include "ui_PluginInformationsListItem.h"

using namespace tlp;

PluginInformationsListItem::PluginInformationsListItem(tlp::PluginInformations *infos, QWidget *parent): QWidget(parent), _ui(new Ui::PluginInformationsListItemData), _pluginInformations(infos) {
  _ui->setupUi(this);

  if (infos->updateAvailable()) {
    _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-upgrade.svg"));
    _ui->statusIcon->setToolTip(trUtf8("Update available"));
    _ui->installButton->setText(trUtf8("Update"));
    _ui->installButton->setIcon(QIcon(":/tulip/app/icons/package-upgrade.svg"));
  }
  else if (infos->isInstalled()) {
    _ui->statusIcon->setPixmap(QPixmap(":/tulip/app/icons/package-installed-updated.svg"));
    _ui->statusIcon->setToolTip(trUtf8("Latest version installed on ") + infos->installDate().toString());
    _ui->installButton->setEnabled(false);
  }
  if (!infos->isInstalled()) // TODO: check if plugin is in a tulip library
    _ui->removeButton->setEnabled(false);

  _ui->icon->setPixmap(QPixmap(infos->iconPath()));
  _ui->name->setText("<p><span style=\"font-size:large;\"><b>" + infos->name() + "</b></span>");
  _ui->shortDescription->setText("<p><span style=\"font-size:small; color:#626262;\">" + infos->shortDescription() + "</span>");
  if (!hasFocus())
    _ui->bottomFrame->hide();

  connect(_ui->infosButton,SIGNAL(clicked()),this,SIGNAL(showDetailedInformations()));
}

void PluginInformationsListItem::focusInEvent(QFocusEvent *) {
  emit gotFocus();
}

void PluginInformationsListItem::expand() {
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(225, 225, 225, 255), stop:1 rgba(205,205,205, 255));  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");
  _ui->bottomFrame->show();
}

void PluginInformationsListItem::collapse() {
  _ui->mainFrame->setStyleSheet("#mainFrame { background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(245, 245, 245, 255), stop:1 rgba(225,225,225, 255));  border-left: 1px solid \"#C9C9C9\";  border-right: 1px solid \"#C9C9C9\";  border-top: 1px solid \"#C9C9C9\";  border-bottom: 1px solid \"#C9C9C9\";}");
  _ui->bottomFrame->hide();
}
