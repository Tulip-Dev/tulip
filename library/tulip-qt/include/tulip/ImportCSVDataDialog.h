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

#ifndef IMPORTCSVDATADIALOG_H_
#define IMPORTCSVDATADIALOG_H_

#include <tulip/ImportCSVDataWidget.h>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>

namespace tlp {
class Graph;

/*
 * @brief Display an import data widget in a dialog and configure it.
 * Display an import data widget in a dialog and configure it.
 * Apply modification graph when user click on accept button. Destroy the ImportDataWidget.
 */
class TLP_QT_SCOPE ImportCSVDataDialog : public QDialog {
  Q_OBJECT
public:
  ImportCSVDataDialog(ImportCSVDataWidget* importDataWidget, tlp::Graph* graph, QWidget * parent = 0);
  virtual ~ImportCSVDataDialog();

public slots:
  void accept();

private:
  ImportCSVDataWidget *importDataWidget;
  QDialogButtonBox *buttonBox;

private slots:
  void importWidgetValidStateChanged(bool state);
};
}
#endif /* IMPORTCSVDATADIALOG_H_ */
