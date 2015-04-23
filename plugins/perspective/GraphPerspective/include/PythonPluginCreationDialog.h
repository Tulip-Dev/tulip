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

#ifndef PLUGINCREATIONDIALOG_H_
#define PLUGINCREATIONDIALOG_H_

#include <QDialog>

namespace Ui {
class PythonPluginCreationDialog;
}

class PythonPluginCreationDialog : public QDialog  {

  Q_OBJECT
  Ui::PythonPluginCreationDialog *_ui;


public:

  PythonPluginCreationDialog(QWidget *parent = 0);
  ~PythonPluginCreationDialog();
  QString getPluginFileName() const;

  QString getPluginType() const;

  QString getPluginClassName() const;

  QString getPluginName() const;

  QString getPluginAuthor() const;

  QString getPluginDate() const;

  QString getPluginInfos() const;

  QString getPluginRelease() const;

  QString getPluginGroup() const;

protected slots:

  void validateForm();

  void selectPluginSourceFile();

};

#endif /* PLUGINCREATIONDIALOG_H_ */
