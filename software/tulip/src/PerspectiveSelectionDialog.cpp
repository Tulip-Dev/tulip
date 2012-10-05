#include "PerspectiveSelectionDialog.h"

#include "ui_PerspectiveSelectionDialog.h"

#include <tulip/PluginModel.h>
#include <tulip/Perspective.h>

PerspectiveSelectionDialog::PerspectiveSelectionDialog(QWidget *parent): QDialog(parent), _ui(new Ui::PerspectiveSelectionDialog), _perspective("") {
  _ui->setupUi(this);
  _ui->perspectiveList->setModel(new tlp::PluginModel<tlp::Perspective>(_ui->perspectiveList));
  _ui->perspectiveList->setRootIndex(_ui->perspectiveList->model()->index(0,0));
  connect(_ui->perspectiveList->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

QString PerspectiveSelectionDialog::perspectiveName() const {
  return _perspective;
}

void PerspectiveSelectionDialog::selectionChanged(const QModelIndex &a, const QModelIndex &) {
  _perspective = a.data().toString();
}
