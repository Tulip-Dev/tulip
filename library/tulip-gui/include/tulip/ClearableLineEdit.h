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


#ifndef CLEARABLELINEEDIT_H
#define CLEARABLELINEEDIT_H

#include <QLineEdit>

#include <tulip/tulipconf.h>


class TLP_QT_SCOPE ClearableLineEdit : public QLineEdit {
  static QPixmap* CLEAR_PIXMAP;

  bool _clearButtonHovered;
  static void initPixmap();
  QRect pixmapRect();
public:
  explicit ClearableLineEdit(QWidget *parent = NULL);

protected:
  void paintEvent(QPaintEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);

};


#endif // CLEARABLELINEEDIT_H
///@endcond
