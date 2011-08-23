#include "FiltersManager.h"

#include "ui_FiltersManager.h"

FiltersManager::FiltersManager(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerData) {
  _ui->setupUi(this);
}
