/*
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

///@cond DOXYGEN_HIDDEN

#ifndef SHAPEDIALOG_H
#define SHAPEDIALOG_H

#include <tulip/tulipconf.h>

#include <QDialog>
#include <QString>
#include <QPixmap>

namespace Ui {
class ShapeDialog;
}

namespace tlp {

class TLP_QT_SCOPE ShapeDialog : public QDialog {

  Q_OBJECT

  Ui::ShapeDialog *_ui;
  QString _selectedShapeName;
  std::list<std::pair<QString, QPixmap>> shapes;

public:
  ShapeDialog(std::list<std::pair<QString, QPixmap>> &nodeShapes, QWidget *parent = nullptr);
  ~ShapeDialog() override;

  QString getSelectedShapeName() const;

  void setSelectedShapeName(const QString &shapeName);

  void accept() override;

  void showEvent(QShowEvent *) override;

protected slots:
  void updateShapeList();
};
} // namespace tlp

#endif

///@endcond
