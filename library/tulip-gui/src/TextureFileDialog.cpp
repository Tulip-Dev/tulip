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
#include <QFileDialog>
#include <QImageReader>

#include <tulip/TextureFileDialog.h>

using namespace tlp;

TextureFileDialog::TextureFileDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::TextureFileDialogData()) {
  ui->setupUi(this);
  connect(ui->chooseFileOrDirButton, SIGNAL(clicked()), this, SLOT(browse()));
}

TextureFileDialog::~TextureFileDialog() {
  delete ui;
}

void TextureFileDialog::done(int res) {
  if (res)
    _data.texturePath = ui->fileOrDirLineEdit->text();
  QDialog::done(res);
}

void TextureFileDialog::setData(const TextureFile &tf) {
  _data = tf;
  setWindowTitle("Choose a texture file");
  ui->fileOrDirLabel->setText("Texture file");
  ui->fileOrDirLineEdit->setText(tf.texturePath);
  setModal(true);
  move(QCursor::pos() - QPoint(250, 40));
}

void TextureFileDialog::browse() {
  static QString filter;
  if (filter.isEmpty()) {
    // allow to choose among all supported image formats
    for (auto f : QImageReader::supportedImageFormats()) {
      if (filter.isEmpty())
        filter.append("Images (");
      else
        filter.append(' ');
      filter.append("*.");
      filter.append(QString(f).toLower());
    }
    filter.append(')');
  }
  QString result = QFileDialog::getOpenFileName(parentWidget(), "Choose a texture file",
                                                _data.texturePath, filter);

  if (!result.isEmpty())
    ui->fileOrDirLineEdit->setText(result);
}

void TextureFileDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (!inGuiTestingMode() && parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}
