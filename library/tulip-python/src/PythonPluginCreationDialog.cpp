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

#include <tulip/PythonInterpreter.h>

#include <QDate>
#include <QFileDialog>
#include <QMessageBox>

#include <tulip/PythonPluginCreationDialog.h>
#include "ui_PythonPluginCreationDialog.h"

using namespace tlp;

PythonPluginCreationDialog::PythonPluginCreationDialog(QWidget *parent)
    : QDialog(parent,
              Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
      _ui(new Ui::PythonPluginCreationDialog) {
  _ui->setupUi(this);

  connect(_ui->browseButton, SIGNAL(clicked()), this, SLOT(selectPluginSourceFile()));
  QDate currentDate = QDate::currentDate();
  _ui->date->setText(currentDate.toString("dd/MM/yyyy"));
  _ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  _ui->pluginFileName->installEventFilter(this);
  _ui->pluginClassName->installEventFilter(this);
  _ui->pluginName->installEventFilter(this);
}

PythonPluginCreationDialog::~PythonPluginCreationDialog() {
  delete _ui;
}

void PythonPluginCreationDialog::accept() {
  QString fileName =
      _ui->pluginFileName->text().mid(_ui->pluginFileName->text().lastIndexOf("/") + 1);
  fileName = fileName.mid(0, fileName.length() - 3);

  if (fileName.at(0).isNumber()) {
    QMessageBox::critical(this, "Error",
                          "Python does not allow a file name to begin with a number.");
    return;
  }

  if (fileName.contains(" ")) {
    QMessageBox::critical(this, "Error", "The plugin file name cannot contain any whitespace.");
    return;
  }

  int i = 0;

  while (PythonInterpreter::pythonReservedCharacters[i]) {
    if (fileName.contains(PythonInterpreter::pythonReservedCharacters[i++])) {
      QMessageBox::critical(this, "Error", "The plugin file name contains an invalid character.");
      return;
    }
  }

  auto className = _ui->pluginClassName->text();
  auto nameIt = className.begin();
  while (nameIt != className.end()) {
    QChar ch = *nameIt;
    if (nameIt == className.begin()) {
      if (ch.isLetter() || ch == '_') {
        ++nameIt;
        continue;
      }
    } else if (ch.isLetterOrNumber() || ch == '_') {
      ++nameIt;
      continue;
    }
    QMessageBox::critical(this, "Invalid plugin class name",
                          "A plugin class name must start with a letter or _ and\ncan only contain "
                          "A-z, 0-9, or _ characters.");
    return;
  }

  QDialog::accept();
}

void PythonPluginCreationDialog::selectPluginSourceFile() {
  QString fileName =
      QFileDialog::getSaveFileName(this, tr("Set Plugin source file"), "", "Python script (*.py)");

  if (fileName.isEmpty())
    return;

  if (!fileName.endsWith(".py"))
    fileName += ".py";

  _ui->pluginFileName->setText(fileName);
}

QString PythonPluginCreationDialog::getPluginFileName() const {
  return _ui->pluginFileName->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginType() const {
  return _ui->pluginType->currentText();
}

QString PythonPluginCreationDialog::getPluginClassName() const {
  return _ui->pluginClassName->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginName() const {
  return _ui->pluginName->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginAuthor() const {
  return _ui->author->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginDate() const {
  return _ui->date->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginInfo() const {
  return _ui->info->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginRelease() const {
  return _ui->release->text().trimmed();
}

QString PythonPluginCreationDialog::getPluginGroup() const {
  return _ui->group->text().trimmed();
}

bool PythonPluginCreationDialog::eventFilter(QObject *, QEvent *ev) {
  if (ev->type() == QEvent::KeyRelease) {
    auto okButton = _ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(!getPluginFileName().isEmpty() && !getPluginClassName().isEmpty() &&
                         !getPluginName().isEmpty());
  }
  return false;
}
