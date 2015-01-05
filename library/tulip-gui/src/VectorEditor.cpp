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

#include <tulip/VectorEditor.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/TulipMetaTypes.h>

#include "ui_VectorEditor.h"

using namespace tlp;

VectorEditor::VectorEditor(QWidget *parent): QDialog(parent), _ui(new Ui::VectorEditor), _userType(0) {
  _ui->setupUi(this);
  _ui->list->setItemDelegate(new TulipItemDelegate(_ui->list));
}

VectorEditor::~VectorEditor() {
  delete _ui;
}

void VectorEditor::setVector(const QVector<QVariant> &d, int userType) {
  _userType = userType;
  _ui->list->clear();

  foreach(QVariant v, d) {
    QListWidgetItem* i = new QListWidgetItem();
    i->setData(Qt::DisplayRole,v);
    i->setFlags(i->flags() | Qt::ItemIsEditable);
    _ui->list->addItem(i);
  }
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
  currentVector = d;
}

void VectorEditor::add() {
  QListWidgetItem* i = new QListWidgetItem();

  if (_userType == qMetaTypeId<std::string>()) {
    // workaround to indicate there is a new string to edit
    // because the height of the line if very small with an empty string
    std::string str("edit this string");
    i->setData(Qt::DisplayRole, QVariant(_userType, &str));
  }
  else
    i->setData(Qt::DisplayRole, QVariant(_userType,(const void*)NULL));

  // needed for color
  i->setSizeHint(QSize(i->sizeHint().width(), 15));
  i->setFlags(i->flags() | Qt::ItemIsEditable);
  _ui->list->addItem(i);
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}

void VectorEditor::remove() {
  foreach(QListWidgetItem* i, _ui->list->selectedItems())
  delete i;
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}

void VectorEditor::done(int r) {
  if (r == QDialog::Accepted) {
    QAbstractItemModel* model = _ui->list->model();
    currentVector.clear();

    for (int i=0; i<model->rowCount(); ++i) {
      currentVector.push_back(model->data(model->index(i,0)));
    }
  }

  QDialog::done(r);
}
