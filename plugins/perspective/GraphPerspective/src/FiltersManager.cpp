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

#include "ui_FiltersManager.h"
#include "FiltersManagerItem.h"

FiltersManager::FiltersManager(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerData) {
  _ui->setupUi(this);
  _ui->filtersListContents->setLayout(new QVBoxLayout);
  _ui->filtersListContents->layout()->setMargin(0);
  _ui->filtersListContents->layout()->setAlignment(Qt::AlignTop);

  for (int i=0; i<30; ++i)
    _ui->filtersListContents->layout()->addWidget(new FiltersManagerItem());

}

FiltersManager::~FiltersManager() {
  delete _ui;
}
