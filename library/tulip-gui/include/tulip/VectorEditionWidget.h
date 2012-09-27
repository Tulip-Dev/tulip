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
///@cond DOXYGEN_HIDDEN


#ifndef VECTOREDITIONWIDGET_H
#define VECTOREDITIONWIDGET_H

#include <QtGui/QWidget>

#include <tulip/tulipconf.h>

namespace Ui {
class VectorEditionWidget;
}

class TLP_QT_SCOPE VectorEditionWidget:public QWidget {
  Q_OBJECT
  Ui::VectorEditionWidget* _ui;

  int _userType;

public:
  explicit VectorEditionWidget(QWidget* parent=0);
  void setVector(const QVector<QVariant>& d, int userType);
  QVector<QVariant> vector() const;

public slots:
  void add();
  void remove();
};

#endif // VECTOREDITIONWIDGET_H
///@endcond