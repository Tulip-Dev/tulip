/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <tulip/VectorEditionWidget.h>
#include <tulip/TulipItemDelegate.h>
#include <tulip/TulipMetaTypes.h>

#include "ui_VectorEditionWidget.h"

using namespace tlp;

VectorEditionWidget::VectorEditionWidget(QWidget *parent): QWidget(parent), _ui(new Ui::VectorEditionWidget), _userType(0) {
  _ui->setupUi(this);
  _ui->list->setItemDelegate(new TulipItemDelegate(_ui->list));
}

VectorEditionWidget::~VectorEditionWidget() {
  delete _ui;
}

void VectorEditionWidget::setVector(const QVector<QVariant> &d, int userType) {
  _userType = userType;
  _ui->list->clear();

  foreach(QVariant v, d) {
    QListWidgetItem* i = new QListWidgetItem();
    i->setData(Qt::DisplayRole,v);
    i->setFlags(i->flags() | Qt::ItemIsEditable);
    _ui->list->addItem(i);
  }
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}

QVector<QVariant> VectorEditionWidget::vector() const {
  QVector<QVariant> result;
  QAbstractItemModel* model = _ui->list->model();

  for (int i=0; i<model->rowCount(); ++i) {
    result.push_back(model->data(model->index(i,0)));
  }

  return result;
}

void VectorEditionWidget::add() {
  QListWidgetItem* i = new QListWidgetItem();
  i->setData(Qt::DisplayRole,QVariant(_userType,(const void*)NULL));
  i->setFlags(i->flags() | Qt::ItemIsEditable);
  _ui->list->addItem(i);
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}

void VectorEditionWidget::remove() {
  foreach(QListWidgetItem* i, _ui->list->selectedItems())
  delete i;
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}
