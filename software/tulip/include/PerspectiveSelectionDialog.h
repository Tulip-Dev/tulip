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

#ifndef PERSPECTIVESELECTIONDIALOG_H
#define PERSPECTIVESELECTIONDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class PerspectiveSelectionDialog;
}

class PerspectiveSelectionDialog : public QDialog {
  Q_OBJECT

  Ui::PerspectiveSelectionDialog* _ui;
  QString _perspective;
public:
  explicit PerspectiveSelectionDialog(QWidget *parent = 0);
  QString perspectiveName() const;

protected slots:
  void selectionChanged(const QModelIndex& a, const QModelIndex& b);
};

#endif // PERSPECTIVESELECTIONDIALOG_H
