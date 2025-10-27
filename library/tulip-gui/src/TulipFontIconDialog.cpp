/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/TulipIconicFont.h>
#include <tulip/TulipFontAwesome.h>
#include <tulip/TulipMaterialDesignIcons.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipSettings.h>

#include <QBuffer>
#include <QClipboard>
#include <QCursor>
#include <QHelpEvent>
#include <QGuiApplication>
#include <QRegularExpression>
#include <QToolTip>
#include <QUrl>

#include "ui_TulipFontIconDialog.h"

using namespace tlp;

TulipFontIconDialog::TulipFontIconDialog(QWidget *parent)
    : QDialog(parent), _ui(new Ui::TulipFontIconDialog) {

  _ui->setupUi(this);
  _ui->iconListWidget->installEventFilter(this);
  _ui->iconsCreditLabel->setText(
      QString("<p style=\" font-size:11px;\">Special credit for the design "
              "of icons goes to:<br/><b>Font Awesome </b><a "
              "href=\"https://fontawesome.com\" style=\"color:" HTML_LINK_COLOR
              ";\">fontawesome.com</"
              "a> (v%1)<br/><b>Material Design Icons </b><a "
              "href=\"https://materialdesignicons.com\" style=\"color:" HTML_LINK_COLOR
              ";\">materialdesignicons.com</"
              "a> (v%2)</p>")
          .arg(TulipFontAwesome::getVersion().c_str())
          .arg(TulipMaterialDesignIcons::getVersion().c_str()));
  connect(_ui->iconNameFilterLineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(updateIconList()));

  updateIconList();
}

void TulipFontIconDialog::updateIconList() {
  _ui->iconListWidget->clear();

  QRegularExpression regexp(_ui->iconNameFilterLineEdit->text());

  std::vector<std::string> iconNames = TulipFontAwesome::getSupportedIcons();
  bool darkMode = TulipSettings::isDisplayInDarkMode();

  for (auto &it : iconNames) {
    QString iconName = tlpStringToQString(it);

    if (iconName.indexOf(regexp) != -1) {
      _ui->iconListWidget->addItem(
          new QListWidgetItem(TulipFontIconEngine::icon(it, darkMode), iconName));
    }
  }

  iconNames = TulipMaterialDesignIcons::getSupportedIcons();

  for (auto &it : iconNames) {
    QString iconName = tlpStringToQString(it);

    if (iconName.indexOf(regexp) != -1) {
      _ui->iconListWidget->addItem(
          new QListWidgetItem(TulipFontIconEngine::icon(it, darkMode), iconName));
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
  } else {
    items = _ui->iconListWidget->findItems(
        iconName.left(2).append('*').append(iconName.mid(iconName.indexOf('-') + 1)),
        Qt::MatchWildcard);
    // select the one with the shortest size
    int minSize = 256;
    int best = 0;
    for (int i = 0; i < items.size(); ++i) {
      int size = items.at(i)->text().size();
      if (size < minSize) {
        best = i;
        minSize = size;
      }
    }
    if (minSize < 256)
      _ui->iconListWidget->setCurrentItem(items.at(best));
  }

  _selectedIconName = iconName;
}

void TulipFontIconDialog::accept() {
  if (_ui->iconListWidget->count() > 0) {
    _selectedIconName = _ui->iconListWidget->currentItem()->text();
  }

  QDialog::accept();
}

void TulipFontIconDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}

bool TulipFontIconDialog::eventFilter(QObject *, QEvent *event) {
  if (event->type() == QEvent::ToolTip) {
    QHelpEvent *he = static_cast<QHelpEvent *>(event);
    auto lwi = _ui->iconListWidget->itemAt(he->pos().x(), he->pos().y());
    if (lwi) {
      // show a 48 pixel height icon
      auto qimg = lwi->icon().pixmap(48).toImage();
      QByteArray bytes;
      QBuffer buf(&bytes);
      qimg.save(&buf, "png", 100);
      QString ttip;
      ttip = QString("<center><img src='data:image/png;base64, %0'/></center><br/>")
                 .arg(QString(bytes.toBase64()))
                 .append(lwi->text());
      QToolTip::showText(he->globalPos(), ttip);
      event->accept();
      return true;
    }
  } else if (event->type() == QEvent::QEvent::KeyPress) {
    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
    auto lwi = _ui->iconListWidget->currentItem();
    if (ke->matches(QKeySequence::Copy) && lwi) {
      // copy the name of the currently selected icon to clipboard
      QClipboard *clipboard = QGuiApplication::clipboard();
      clipboard->setText(lwi->text());
      event->accept();
      // confirm copy with a message
      QToolTip::showText(
          QCursor::pos(),
          QString("<font size=-1><pre><b>%0</b> copied</pre></font>").arg(lwi->text()), nullptr,
          QRect(), 500);
      return true;
    }
  }
  return false;
}
