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

#include <tulip/TulipFontWidget.h>
#include "ui_TulipFontWidget.h"

#include <tulip/TulipFontDialog.h>

using namespace tlp;

TulipFontWidget::TulipFontWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TulipFontWidget) {
  ui->setupUi(this);
  connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(editFont()));
}

TulipFontWidget::~TulipFontWidget() {
  delete ui;
}

void TulipFontWidget::setFont(const TulipFont& font) {
  ui->lineEdit->setText(font.fontFile());
}

TulipFont TulipFontWidget::font()const {
  return TulipFont::fromFile(ui->lineEdit->text());
}

void TulipFontWidget::editFont() {
  TulipFont newFont = TulipFontDialog::getFont(this,font());

  if(newFont.exists()) {
    setFont(newFont);
  }
}
