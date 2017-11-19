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

#ifndef PLUGINCREATIONDIALOG_H_
#define PLUGINCREATIONDIALOG_H_

#include <QDialog>

#include <tulip/tulipconf.h>

namespace Ui {
class PythonPluginCreationDialog;
}

namespace tlp {

class TLP_PYTHON_SCOPE PythonPluginCreationDialog : public QDialog {

  Q_OBJECT
  Ui::PythonPluginCreationDialog *_ui;

public:
  PythonPluginCreationDialog(QWidget *parent = nullptr);
  ~PythonPluginCreationDialog() override;
  QString getPluginFileName() const;

  QString getPluginType() const;

  QString getPluginClassName() const;

  QString getPluginName() const;

  QString getPluginAuthor() const;

  QString getPluginDate() const;

  QString getPluginInfo() const;

  QString getPluginRelease() const;

  QString getPluginGroup() const;

protected slots:

  void accept() override;

  void selectPluginSourceFile();
};
}

#endif /* PLUGINCREATIONDIALOG_H_ */
