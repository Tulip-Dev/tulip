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

#include <tulip/PythonInterpreter.h>

#include <QDate>
#include <QFileDialog>
#include <QMessageBox>

#include "PythonPluginCreationDialog.h"
#include "ui_PythonPluginCreationDialog.h"

using namespace tlp;

PythonPluginCreationDialog::PythonPluginCreationDialog(QWidget *parent) : QDialog(parent), _ui(new Ui::PythonPluginCreationDialog) {
  _ui->setupUi(this);
  setModal(true);
  connect(_ui->okButton, SIGNAL(clicked()), this, SLOT(validateForm()));
  connect(_ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(_ui->browseButton, SIGNAL(clicked()), this, SLOT(selectPluginSourceFile()));
  QDate currentDate = QDate::currentDate();
  _ui->date->setText(currentDate.toString("dd/MM/yyyy"));
}

PythonPluginCreationDialog::~PythonPluginCreationDialog() {
  delete _ui;
}

void PythonPluginCreationDialog::validateForm() {
  if (_ui->pluginFileName->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No file has been selected to save the plugin source code.");
    return;
  }

  QString moduleName = _ui->pluginFileName->text().mid(_ui->pluginFileName->text().lastIndexOf("/")+1);
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

  while (PythonInterpreter::pythonReservedCharacters[i]) {
    if (moduleName.contains(PythonInterpreter::pythonReservedCharacters[i++])) {
      QMessageBox::critical(this, "Error", "The Python module name contains an invalid character.");
      return;
    }
  }

  if (_ui->pluginClassName->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No class name has been provided for the plugin.");
    return;
  }

  if (_ui->pluginClassName->text().at(0).isNumber()) {
    QMessageBox::critical(this, "Error", "Python does not allow a class name to begin with a number.");
    return;
  }

  if (_ui->pluginClassName->text().contains(" ")) {
    QMessageBox::critical(this, "Error", "The Python class name can not contain any whitespace.");
    return;
  }

  i = 0;

  while (PythonInterpreter::pythonReservedCharacters[i]) {
    if (_ui->pluginClassName->text().contains(PythonInterpreter::pythonReservedCharacters[i++])) {
      QMessageBox::critical(this, "Error", "The Python class name contains an invalid character.");
      return;
    }
  }

  if (_ui->pluginName->text().isEmpty()) {
    QMessageBox::critical(this, "Error", "No name has been provided for the plugin.");
    return;
  }

  accept();
}

void PythonPluginCreationDialog::selectPluginSourceFile() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Set Plugin source file"),"","Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  _ui->pluginFileName->setText(fileName);
}

QString PythonPluginCreationDialog::getPluginFileName() const {
  return _ui->pluginFileName->text();
}

QString PythonPluginCreationDialog::getPluginType() const {
  return _ui->pluginType->currentText();
}

QString PythonPluginCreationDialog::getPluginClassName() const {
  return _ui->pluginClassName->text();
}

QString PythonPluginCreationDialog::getPluginName() const {
  return _ui->pluginName->text();
}

QString PythonPluginCreationDialog::getPluginAuthor() const {
  return _ui->author->text();
}

QString PythonPluginCreationDialog::getPluginDate() const {
  return _ui->date->text();
}

QString PythonPluginCreationDialog::getPluginInfos() const {
  return _ui->infos->text();
}

QString PythonPluginCreationDialog::getPluginRelease() const {
  return _ui->release->text();
}

QString PythonPluginCreationDialog::getPluginGroup() const {
  return _ui->group->text();
}

