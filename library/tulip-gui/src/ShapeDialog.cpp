#include <tulip/ShapeDialog.h>

#include "ui_ShapeDialog.h"

using namespace tlp;

ShapeDialog::ShapeDialog(std::list<std::pair<QString, QPixmap> > &nodeShapes, QWidget *parent)
    : QDialog(parent), _ui(new Ui::ShapeDialog), shapes(nodeShapes) {

  _ui->setupUi(this);

  updateShapeList();
}

ShapeDialog::~ShapeDialog() {
  delete _ui;
}

void ShapeDialog::updateShapeList() {
  _ui->shapeListWidget->clear();

  for (std::list<std::pair<QString, QPixmap> >::const_iterator it = shapes.begin();
       it != shapes.end(); ++it) {
    _ui->shapeListWidget->addItem(new QListWidgetItem(it->second, it->first));
  }

  if (_ui->shapeListWidget->count() > 0) {
    _ui->shapeListWidget->setCurrentRow(0);
  }
}

QString ShapeDialog::getSelectedShapeName() const {
  return _selectedShapeName;
}

void ShapeDialog::setSelectedShapeName(const QString &shapeName) {
  QList<QListWidgetItem *> items = _ui->shapeListWidget->findItems(shapeName, Qt::MatchExactly);

  if (!items.isEmpty()) {
    _ui->shapeListWidget->setCurrentItem(items.at(0));
    _selectedShapeName = shapeName;
  }
}

void ShapeDialog::accept() {
  if (_ui->shapeListWidget->count() > 0) {
    _selectedShapeName = _ui->shapeListWidget->currentItem()->text();
  }

  QDialog::accept();
}

void ShapeDialog::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  _selectedShapeName = _ui->shapeListWidget->currentItem()->text();

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}
