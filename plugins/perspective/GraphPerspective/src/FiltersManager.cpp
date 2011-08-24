#include "FiltersManager.h"

#include "ui_FiltersManager.h"

FiltersManager::FiltersManager(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerData) {
  _ui->setupUi(this);
  _ui->header->setMenus(QStringList()
                        << trUtf8("Filtering (no pre-selection)")
                        << trUtf8("Filtering (pre-select nodes)")
                        << trUtf8("Filtering (pre-select edges)")
                        << trUtf8("Filtering (pre-select all)"));

}
