/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <QMessageBox>
#include <QInputDialog>

#include <tulip/RenamePropertyDialog.h>
#include <tulip/TlpQtTools.h>
#include <tulip/Graph.h>

bool tlp::RenamePropertyDialog::renameProperty(tlp::PropertyInterface *prop, QWidget *parent) {
  if (prop == NULL) {
    QMessageBox::critical(parent, "Renaming error", "The property is NULL");
    return false;
  }

  bool valid = false;

  while (!valid) {
    QString errorMsg;
    QString pName = QInputDialog::getText(
        parent, "Renaming property '" + tlp::tlpStringToQString(prop->getName()) + "'",
        "New name: ", QLineEdit::Normal, tlpStringToQString(prop->getName()), &valid);

    if (!valid)
      return false;

    // Check if parameters are valid.
    std::string propertyName = tlp::QStringToTlpString(pName);

    if (propertyName.empty()) {
      valid = false;
      errorMsg = "Cannot rename a property with an empty name";
    } else if (prop->getGraph()->existLocalProperty(propertyName)) {
      valid = false;
      errorMsg =
          "A local property named '" + tlpStringToQString(propertyName) + "'\n already exists";
    }

    if (valid) {
      errorMsg = "Unable to rename the property";
      valid = prop->rename(propertyName);
    }

    if (!valid)
      QMessageBox::critical(parent, "Error when renaming property", errorMsg);
    else
      return true;
  }

  return false;
}
