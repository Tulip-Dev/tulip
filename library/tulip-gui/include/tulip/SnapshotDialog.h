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

#ifndef _Tulip_SNAPSHOTDIALOG_H
#define _Tulip_SNAPSHOTDIALOG_H

#include <QDialog>

#include <tulip/tulipconf.h>

namespace Ui {
class SnapshotDialogData;
}

class QGraphicsScene;
class QGraphicsPixmapItem;

namespace tlp {

class LockLabel;
class View;

class TLP_QT_SCOPE SnapshotDialog : public QDialog {

  Q_OBJECT



public :

  SnapshotDialog(const View *v,QWidget *parent=NULL);
  ~SnapshotDialog();

public slots :

  void accept();

protected slots :

  void widthSpinBoxValueChanged(int value);
  void heightSpinBoxValueChanged(int value);

  void fileNameTextChanged(const QString &);
  void copyClicked();

protected :

  void resizeEvent(QResizeEvent *);

  void sizeSpinBoxValueChanged();

  Ui::SnapshotDialogData *ui;
  const View *view;

  QGraphicsScene *scene;
  QGraphicsPixmapItem *pixmapItem;
  LockLabel *lockLabel;

  float ratio;

  bool inSizeSpinBoxValueChanged;
};

}

#endif






///@endcond
