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


#ifndef VECTOREDITOR_H
#define VECTOREDITOR_H

#include <QDialog>
#include <QAbstractButton>

#include <tulip/tulipconf.h>

namespace Ui {
class VectorEditor;
}

class TLP_QT_SCOPE VectorEditor:public QDialog {
  Q_OBJECT
  Ui::VectorEditor* _ui;

  int _userType;
  QVector<QVariant> currentVector;

public:
  explicit VectorEditor(QWidget* parent=0);
  ~VectorEditor();
  void setVector(const QVector<QVariant>& d, int userType);
  const QVector<QVariant>& vector() const {
    return currentVector;
  }

public slots:
  void add();
  void remove();
  void done(int r);
};

#endif // VECTOREDITIONWIDGET_H
///@endcond
