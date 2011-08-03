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

#include "PythonInterpreter.h"

#include "PluginCreationDialog.h"


#include <QtCore/QDate>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include <iostream>

PluginCreationDialog::PluginCreationDialog(QWidget *parent) : QDialog(parent) {
  setupUi(this);
  setModal(true);
  connect(okButton, SIGNAL(clicked()), this, SLOT(validateForm()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(browseButton, SIGNAL(clicked()), this, SLOT(selectPluginSourceFile()));
  QDate currentDate = QDate::currentDate();
  date->setText(currentDate.toString("dd/MM/yyyy"));
}

void PluginCreationDialog::validateForm() {
  if (pluginFileName->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No file has been selected to save the plugin source code.");
    return;
  }

  QString moduleName = pluginFileName->text().mid(pluginFileName->text().lastIndexOf("/")+1);
  moduleName = moduleName.mid(0, moduleName.length() - 3);

  if (moduleName.at(0).isNumber()) {
    QMessageBox::critical(this, "Error", "Python does not allow a module name to begin with a number.");
    return;
  }

  if (moduleName.contains(" ")) {
    QMessageBox::critical(this, "Error", "The Python module name can not contain any whitespace.");
    return;
  }

  int i = 0;

  while (pythonReservedCharacters[i]) {
    if (moduleName.contains(pythonReservedCharacters[i++])) {
      QMessageBox::critical(this, "Error", "The Python module name contains an invalid character.");
      return;
    }
  }

  if (pluginClassName->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No class name has been provided for the plugin.");
    return;
  }

  if (pluginClassName->text().at(0).isNumber()) {
    QMessageBox::critical(this, "Error", "Python does not allow a class name to begin with a number.");
    return;
  }

  if (pluginClassName->text().contains(" ")) {
    QMessageBox::critical(this, "Error", "The Python class name can not contain any whitespace.");
    return;
  }

  i = 0;

  while (pythonReservedCharacters[i]) {
    if (pluginClassName->text().contains(pythonReservedCharacters[i++])) {
      QMessageBox::critical(this, "Error", "The Python class name contains an invalid character.");
      return;
    }
  }

  if (pluginName->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No name has been provided for the plugin.");
    return;
  }

  accept();
}

void PluginCreationDialog::selectPluginSourceFile() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Set Plugin source filen"),"","Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  pluginFileName->setText(fileName);
}

QString PluginCreationDialog::getPluginFileName() const {
  return pluginFileName->text();
}

QString PluginCreationDialog::getPluginType() const {
  return pluginType->currentText();
}

QString PluginCreationDialog::getPluginClassName() const {
  return pluginClassName->text();
}

QString PluginCreationDialog::getPluginName() const {
  return pluginName->text();
}

QString PluginCreationDialog::getPluginAuthor() const {
  return author->text();
}

QString PluginCreationDialog::getPluginDate() const {
  return date->text();
}

QString PluginCreationDialog::getPluginInfos() const {
  return infos->text();
}

QString PluginCreationDialog::getPluginRelease() const {
  return release->text();
}

QString PluginCreationDialog::getPluginGroup() const {
  return group->text();
}

