/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#ifndef PLUGINDOCDIALOG_H
#define PLUGINDOCDIALOG_H

#include <QDialog>

#include "ui_PluginDocDialog.h"
#include <tulip/ParameterListModel.h>

class PluginDocDialog : public QDialog {
  Ui_PluginDocDialog *ui;

public:
  PluginDocDialog(QWidget *parent) : QDialog(parent) {
    ui = new Ui_PluginDocDialog();
    ui->setupUi(this);
  }

  ~PluginDocDialog() {
    delete ui;
  }

  static void showDoc(QWidget *parent, QString pluginName, QString pluginHelp,
                      tlp::ParameterListModel *model);
};

#endif // PLUGINDOCDIALOG_H
