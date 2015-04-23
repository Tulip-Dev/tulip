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

#ifndef AXISCONFIGDIALOGS_H_
#define AXISCONFIGDIALOGS_H_

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <QDialog>

#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include <QComboBox>
#include <QCheckBox>

#include <tulip/ItemsListWidget.h>

#include "QuantitativeParallelAxis.h"
#include "NominalParallelAxis.h"

namespace tlp {

// A really simple dialog which allows user to set number of graduations
// and data ordering (ascendent or not) for a particular quantitative axis
class QuantitativeAxisConfigDialog : public QDialog {

  Q_OBJECT

public :

  QuantitativeAxisConfigDialog(QuantitativeParallelAxis *axis);
  void closeEvent (QCloseEvent * event);

private :

  void initAxisPartitionsTable();

  QuantitativeParallelAxis *axis;
  QSpinBox *nbGrads;
  QDoubleSpinBox *doubleAxisMinValue, *doubleAxisMaxValue;
  QSpinBox *intAxisMinValue, *intAxisMaxValue;
  QComboBox *axisOrder;
  QPushButton *okButton;
  QCheckBox *log10Scale;
};


// A really simple dialog which allows user to set labels order
// on a particular nominative axis
class NominalAxisConfigDialog : public QDialog {

  Q_OBJECT

public :

  NominalAxisConfigDialog(NominalParallelAxis *axis);
  void closeEvent (QCloseEvent * event);

public slots :

  void pressButtonUp();
  void pressButtonDown();
  void pressButtonLexOrder();

private :

  NominalParallelAxis *axis;
  QPushButton *okButton;
  QHash<QString,itemInfo > hashDataBase;
  ItemsListWidget *axisLabelsOrder;
};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif /* AXISCONFIGDIALOGS_H_ */

