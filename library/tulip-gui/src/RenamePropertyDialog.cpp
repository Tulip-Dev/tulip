/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <QString>
#include <QStringList>
#include <QListView>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QTabWidget>

#include "ui_RenamePropertyDialog.h"

#include <tulip/RenamePropertyDialog.h>
#include <tulip/TlpQtTools.h>
#include <tulip/ForEach.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

//=============================================================================
RenamePropertyDialog::RenamePropertyDialog(QWidget* parent)
  : QDialog(parent),ui(new Ui::RenamePropertyDialogData()) {
  ui->setupUi(this);
  connect(ui->buttonOK , SIGNAL(clicked()), this, SLOT(accept()) );
  connect(ui->buttonCancel , SIGNAL(clicked()), this, SLOT(reject()) );
}

RenamePropertyDialog::~RenamePropertyDialog() {
  delete ui;
}

bool RenamePropertyDialog::renameProperty(PropertyInterface* prop,
    QWidget* parent) {
  if (prop == NULL) {
    QMessageBox::critical(parent,tr("Error during the renaming"), "The property is NULL");
    return false;
  }

  RenamePropertyDialog dialog(parent);
  dialog.setWindowTitle(tr("Rename property '") + tlpStringToQString(prop->getName()) + "'");

  while (dialog.exec() != QDialog::Rejected) {
    QString errorMsg;
    std::string propertyName;
    bool valid = true;

    //Check if parameters are valid.
    propertyName = QStringToTlpString(dialog.ui->propertyNameLineEdit->text());

    if (propertyName.empty()) {
      valid = false;
      errorMsg = tr("Cannot rename a property with an empty name");
    }
    else if (prop->getGraph()->existLocalProperty(propertyName)) {
      valid = false;
      errorMsg = tr("A local property named '") + tlpStringToQString(propertyName) + "'\n already exists";
    }

    if (valid) {
      errorMsg = "Unable to rename the property";
      valid = prop->rename(propertyName);
    }

    if (!valid)
      QMessageBox::critical(parent,tr("Error during the renaming"),errorMsg);
    else
      return true;
  }

  return false;
}
