#include <tulip/VectorEditionWidget.h>

#include <tulip/TulipItemDelegate.h>

#include "ui_VectorEditionWidget.h"

using namespace tlp;

VectorEditionWidget::VectorEditionWidget(QWidget *parent): QWidget(parent), _ui(new Ui::VectorEditionWidget), _userType(0) {
  _ui->setupUi(this);
  _ui->list->setItemDelegate(new TulipItemDelegate(_ui->list));
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
  i->setData(Qt::DisplayRole,QVariant(_userType,NULL));
  i->setFlags(i->flags() | Qt::ItemIsEditable);
  _ui->list->addItem(i);
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}

void VectorEditionWidget::remove() {
  foreach(QListWidgetItem* i, _ui->list->selectedItems())
  delete i;
  _ui->countLabel->setText(QString::number(_ui->list->model()->rowCount()));
}
