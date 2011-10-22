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
#include "FiltersManager.h"

#include "ui_FiltersManagerItem.h"
#include "ui_FiltersManager.h"

// **********************************************
// Helper classes
// **********************************************
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
// **********************************************

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
