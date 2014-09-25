/*
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
///@cond DOXYGEN_HIDDEN


#ifndef COORDEDITOR_H
#define COORDEDITOR_H

#include <QDialog>

#include <tulip/Coord.h>
#include <tulip/tulipconf.h>

namespace Ui {
class CoordEditor;
}

namespace tlp {
/**
* @brief Simple Coord edition widget.
**/
class TLP_QT_SCOPE CoordEditor : public QDialog {
  Q_OBJECT
  Q_PROPERTY(Coord coord READ coord WRITE setCoord NOTIFY coordChanged)

  Ui::CoordEditor *ui;
  Coord currentCoord;

public:
  explicit CoordEditor(QWidget *parent = 0, bool editSize = false);
  ~CoordEditor();

  tlp::Coord coord() const;

  // redefinition to ensure it is shown in the center of its parent
  void showEvent(QShowEvent* ev);


public slots:
  void setCoord(const tlp::Coord& s);
  // redefinition to ensure to catch the end of input
  void done(int r);

signals:
  void coordChanged(tlp::Coord coord);

private slots:
  void coordUpdated();
};
}

#endif // COORDEDITOR_H
///@endcond
