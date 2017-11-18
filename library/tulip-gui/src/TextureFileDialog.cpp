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
#include <QFileDialog>

#include <tulip/TextureFileDialog.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Perspective.h>

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
  if (res) {
    if (ui->localFilesystemRB->isChecked()) {
      _data.texturePath = ui->fileOrDirLineEdit->text();
    } else {
      _data.texturePath = ui->urlLineEdit->text();
    }
  }

  QDialog::done(res);
}

void TextureFileDialog::setData(const TextureFile &tf) {
  _data = tf;
  setWindowTitle("Choose a texture file");

  if (tf.texturePath.startsWith("http")) {
    ui->httpUrlRB->setChecked(true);
    ui->urlLineEdit->setText(tf.texturePath);
  } else {
    ui->localFilesystemRB->setChecked(true);
    ui->fileOrDirLineEdit->setText(tf.texturePath);
  }

  setModal(true);
  move(QCursor::pos() - QPoint(250, 40));
}

void TextureFileDialog::browse() {
  QString result = QFileDialog::getOpenFileName(parentWidget(), "Choose a texture file",
                                                _data.texturePath, "Images (*.jpg *.jpeg *.png)");

  if (!result.isEmpty())
    ui->fileOrDirLineEdit->setText(result);
}
