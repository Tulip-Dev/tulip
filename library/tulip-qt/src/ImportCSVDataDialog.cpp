/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/ImportCSVDataDialog.h"
#include <tulip/Graph.h>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <cassert>
using namespace tlp;
ImportCSVDataDialog::ImportCSVDataDialog(ImportCSVDataWidget* importDataWidget, tlp::Graph* graph,
    QWidget * parent) :
  QDialog(parent), importDataWidget(importDataWidget), buttonBox(new QDialogButtonBox(this)) {
  assert(importDataWidget != NULL);
  setLayout(new QVBoxLayout());
  buttonBox->addButton(QDialogButtonBox::Ok);
  buttonBox->addButton(QDialogButtonBox::Cancel);
  layout()->addWidget(importDataWidget);
  layout()->addWidget(buttonBox);
  connect(importDataWidget, SIGNAL(validStateChange(bool)), this, SLOT(importWidgetValidStateChanged(bool)));
  connect(buttonBox, SIGNAL(accepted ()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected ()), this, SLOT(reject()));

  importDataWidget->setGraph(graph);
}

ImportCSVDataDialog::~ImportCSVDataDialog() {
}

void ImportCSVDataDialog::importWidgetValidStateChanged(bool state) {
  buttonBox->button(QDialogButtonBox::Ok)->setEnabled(state);
}

void ImportCSVDataDialog::accept() {
  if (importDataWidget->loadDataIntoGraph()) {
    QDialog::accept();
  }
  else {
    QDialog::reject();
  }
}
