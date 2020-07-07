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

#include <tulip/TulipFontIconDialog.h>
#include <tulip/TulipFontIconEngine.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>
#include <tulip/TlpQtTools.h>

#include <QRegExp>
#include <QDesktopServices>
#include <QUrl>

#include "ui_TulipFontIconDialog.h"

using namespace tlp;

TulipFontIconDialog::TulipFontIconDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::TulipFontIconDialog) {

  _ui->setupUi(this);

  _ui->iconsCreditLabel->setText(
      QString("<p style=\" font-size:11px;\">Special credit for the design "
              "of icons goes to:<br/><b>Font "
              "Awesome </b><span style=\"color:#0000ff;\"><a "
              "href=\"http://fontawesome.com\">http://fontawesome.com</a></span> "
              "(v%1)<br/><b>Material Design Icons </b><span "
              "style=\"color:#0000ff;\"><a "
              "href=\"https://materialdesignicons.com\">https://materialdesignicons.com</a></"
              "span> (v%2)</p>")
          .arg(TulipFontAwesome::getVersion().c_str())
          .arg(TulipMaterialDesignIcons::getVersion().c_str()));
  connect(_ui->iconNameFilterLineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(updateIconList()));
  connect(_ui->iconsCreditLabel, SIGNAL(linkActivated(const QString &)), this,
          SLOT(openUrlInBrowser(const QString &)));

  updateIconList();
}

void TulipFontIconDialog::updateIconList() {
  _ui->iconListWidget->clear();

  QRegExp regexp(_ui->iconNameFilterLineEdit->text());

  std::vector<std::string> iconNames = TulipFontAwesome::getSupportedIcons();

  for (auto &it : iconNames) {
    QString iconName = tlpStringToQString(it);

    if (regexp.indexIn(iconName) != -1) {
      _ui->iconListWidget->addItem(new QListWidgetItem(TulipFontIconEngine::icon(it), iconName));
    }
  }

  iconNames = TulipMaterialDesignIcons::getSupportedIcons();

  for (auto &it : iconNames) {
    QString iconName = tlpStringToQString(it);

    if (regexp.indexIn(iconName) != -1) {
      _ui->iconListWidget->addItem(new QListWidgetItem(TulipFontIconEngine::icon(it), iconName));
    }
  }

  if (_ui->iconListWidget->count() > 0) {
    _ui->iconListWidget->setCurrentRow(0);
  }
}

QString TulipFontIconDialog::getSelectedIconName() const {
  return _selectedIconName;
}

void TulipFontIconDialog::setSelectedIconName(const QString &iconName) {
  QList<QListWidgetItem *> items = _ui->iconListWidget->findItems(iconName, Qt::MatchExactly);

  if (!items.isEmpty()) {
    _ui->iconListWidget->setCurrentItem(items.at(0));
    _selectedIconName = iconName;
  }
}

void TulipFontIconDialog::accept() {
  if (_ui->iconListWidget->count() > 0) {
    _selectedIconName = _ui->iconListWidget->currentItem()->text();
  }

  QDialog::accept();
}

void TulipFontIconDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  _selectedIconName = _ui->iconListWidget->currentItem()->text();

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}

void TulipFontIconDialog::openUrlInBrowser(const QString &url) {
  QDesktopServices::openUrl(QUrl(url));
}
