/*
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
///@cond DOXYGEN_HIDDEN


#ifndef COORDEDITOR_H
#define COORDEDITOR_H

#include <QtGui/QDialog>
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
public:
  explicit CoordEditor(QWidget *parent = 0);
  ~CoordEditor();

  tlp::Coord coord() const;

public slots:
  void setCoord(const tlp::Coord& s);

signals:
  void coordChanged(tlp::Coord coord);

private slots:
  void coordUpdated();

};
}

#endif // COORDEDITOR_H
///@endcond
