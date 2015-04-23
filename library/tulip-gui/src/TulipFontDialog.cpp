/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include "tulip/TulipFontDialog.h"
#include "ui_TulipFontDialog.h"

#include <QFontDatabase>
#include <QPushButton>
#include <QDebug>

using namespace tlp;

TulipFontDialog::TulipFontDialog(QWidget *parent): QDialog(parent), _ui(new Ui::TulipFontDialog), previousFont(), ok(QDialog::Rejected) {
  _ui->setupUi(this);

  foreach(QString font, TulipFont::installedFontNames())
  _ui->nameList->addItem(font);

  bool hasFont = _ui->nameList->count()>0;
  _ui->settingsWidget->setEnabled(hasFont);
  _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasFont);

  if (hasFont) {
    _ui->nameList->setCurrentItem(_ui->nameList->item(0));
    fontChanged();
  }

}

TulipFontDialog::~TulipFontDialog() {
  delete _ui;
}

TulipFont TulipFontDialog::font() const {
  TulipFont result;
  result.setFontName(_ui->nameList->currentItem()->text());
  result.setBold(_ui->styleList->currentIndex().row() == 1 || _ui->styleList->currentIndex().row() == 3);
  result.setItalic(_ui->styleList->currentIndex().row() == 2 || _ui->styleList->currentIndex().row() == 3);
  return result;
}

void TulipFontDialog::fontChanged() {
  if (_ui->nameList->currentItem()==NULL || _ui->styleList->currentItem()==NULL)
    return;

  if (sender() == _ui->sizeList) {
    _ui->sizeSpin->setValue(_ui->sizeList->currentItem()->text().toInt());
    return;
  }

  TulipFont selectedFont = font();
  _ui->preview->setStyleSheet("font-family: " + selectedFont.fontFamily() + "; "
                              + (selectedFont.isItalic() ? "font-style: italic; " : "")
                              + (selectedFont.isBold() ? "font-weight: bold; " : "")
                              + "font-size: " + QString::number(_ui->sizeSpin->value()) + "px; "
                             );
}
int TulipFontDialog::fontSize() const {
  return _ui->sizeSpin->value();
}

void TulipFontDialog::selectFont(const TulipFont& f) {
  QList<QListWidgetItem*> items = _ui->nameList->findItems(f.fontName(),Qt::MatchExactly);

  if (items.empty())
    return;

  _ui->nameList->setCurrentItem(items[0]);

  if (f.isBold()) {
    if (f.isItalic())
      _ui->styleList->setCurrentRow(3);
    else
      _ui->styleList->setCurrentRow(1);
  }
  else if (f.isItalic())
    _ui->styleList->setCurrentRow(2);
  else
    _ui->styleList->setCurrentRow(0);

  previousFont = f;
}

TulipFont TulipFontDialog::getFont(QWidget* /*parent*/ ,const TulipFont& selectedFont) {
  TulipFontDialog dlg;
  dlg.selectFont(selectedFont);

  if (dlg.exec() != QDialog::Accepted || !dlg.font().exists())
    return TulipFont();
  else
    return dlg.font();
}
