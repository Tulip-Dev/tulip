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

#ifndef SIZESCALECONFIGDIALOG_H_
#define SIZESCALECONFIGDIALOG_H_

#include <QDialog>

namespace Ui {
class SizeScaleConfigDialogData;
}

class QWidget;

namespace tlp {
class SizeScaleConfigDialog : public QDialog {

  Q_OBJECT

  Ui::SizeScaleConfigDialogData* _ui;

public :

  SizeScaleConfigDialog(QWidget *d = 0);
  ~SizeScaleConfigDialog();

  bool doMappingOnViewSize() const;

  float getMinSize() const;
  float getMaxSize() const;

  bool applySizeMappingOnX() const;
  bool applySizeMappingOnY() const;
  bool applySizeMappingOnZ() const;

private slots :

  void minSizeValueChanged(double newValue);
  void maxSizeValueChanged(double newValue);
  void viewSizeRadioButtonToggled(bool checked);

};
}
#endif /* SIZESCALECONFIGDIALOG_H_ */
