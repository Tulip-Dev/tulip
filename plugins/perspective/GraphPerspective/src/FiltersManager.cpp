#include "FiltersManager.h"

#include "FiltersManagerItem.h"

#include "ui_FiltersManager.h"

FiltersManager::FiltersManager(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerData) {
  _ui->setupUi(this);
  _ui->header->setMenus(QStringList()
                        << trUtf8("Filtering (no pre-selection)")
                        << trUtf8("Filtering (on current selection)")
                        << trUtf8("Filtering (pre-select nodes)")
                        << trUtf8("Filtering (pre-select edges)")
                        << trUtf8("Filtering (pre-select all)"));

  _ui->filtersListContents->setLayout(new QVBoxLayout);
  _ui->filtersListContents->layout()->setMargin(0);
  _ui->filtersListContents->layout()->setAlignment(Qt::AlignTop);
}

FiltersManager::~FiltersManager() {
  delete _ui;
}
