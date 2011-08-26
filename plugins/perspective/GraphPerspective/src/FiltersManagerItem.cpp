#include "FiltersManagerItem.h"

#include "ui_FiltersManagerItem.h"

#include <tulip/PluginLister.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Algorithm.h>

FiltersManagerItem::FiltersManagerItem(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerItemData), _removable(true) {
  _ui->setupUi(this);
}

FiltersManagerItem::~FiltersManagerItem() {
  delete _ui;
}

void FiltersManagerItem::setRemovable(bool f) {
  _removable = f;
}

void FiltersManagerItem::expand() {
}
