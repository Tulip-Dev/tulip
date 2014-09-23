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

#ifndef PYTHONTABWIDGET_H
#define PYTHONTABWIDGET_H

#include <QTabWidget>

#include <tulip/tulipconf.h>

class TLP_PYTHON_SCOPE PythonTabWidget : public QTabWidget {

public :

  explicit PythonTabWidget(QWidget *parent = 0);

  void setDrawTabBarBgGradient(const bool drawGradient) {
    _drawGradient = drawGradient;
  }

  void setTextColor(const QColor &textColor) {
    _textColor = textColor;
  }

protected:

  void paintEvent(QPaintEvent * event);

  bool _drawGradient;
  QColor _textColor;

};

#endif // PYTHONTABWIDGET_H
