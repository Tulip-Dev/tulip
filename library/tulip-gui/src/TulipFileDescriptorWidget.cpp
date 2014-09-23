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
#include "tulip/TulipFileDescriptorWidget.h"

#include <QKeyEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QApplication>

TulipFileDescriptorWidget::TulipFileDescriptorWidget(QWidget* parent): QWidget(parent), _data(TulipFileDescriptor()) {
  _data.absolutePath = "";
  _data.type = TulipFileDescriptor::File;

  setLayout(new QHBoxLayout);
  layout()->setSpacing(0);
  layout()->setMargin(0);
  _display = new QLineEdit;
  static_cast<QLineEdit*>(_display)->setReadOnly(false);
  layout()->addWidget(_display);
  QPushButton* browseBtn = new QPushButton(trUtf8("..."));
  layout()->addWidget(browseBtn);
  connect(browseBtn,SIGNAL(clicked()),this,SLOT(browse()));
}

void TulipFileDescriptorWidget::browse() {
  QString result;

  if (_data.type == TulipFileDescriptor::Directory) {
    result = QFileDialog::getExistingDirectory(parentWidget(),trUtf8("Choose directory"),_data.absolutePath);
  }
  else {
    result = QFileDialog::getOpenFileName(parentWidget(),trUtf8("Choose file"),_data.absolutePath,_data.fileFilterPattern);
  }

  if (!result.isEmpty())
    _data.absolutePath = result;

  setData(_data);
  QKeyEvent ev1(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
  QKeyEvent ev2(QEvent::KeyRelease,Qt::Key_Enter,Qt::NoModifier);
  QApplication::sendEvent(_display,&ev1);
  QApplication::sendEvent(_display,&ev2);
}

void TulipFileDescriptorWidget::setData(TulipFileDescriptor desc) {
  _data = desc;
  static_cast<QLineEdit*>(_display)->setText(_data.absolutePath);
}

TulipFileDescriptor TulipFileDescriptorWidget::data() const {
  return _data;
}
