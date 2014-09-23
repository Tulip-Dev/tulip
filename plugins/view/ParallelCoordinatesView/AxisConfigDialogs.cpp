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

#include "AxisConfigDialogs.h"

#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QString>

using namespace std;

namespace tlp {

QuantitativeAxisConfigDialog::QuantitativeAxisConfigDialog(QuantitativeParallelAxis *axis) : QDialog(), axis(axis) {
  setWindowTitle("Quantitative Axis Configuration");

  QVBoxLayout *dialogLayout = new QVBoxLayout();
  QHBoxLayout *nbGradsLayout = new QHBoxLayout();
  QHBoxLayout *axisMinLayout = new QHBoxLayout();
  QHBoxLayout *axisMaxLayout = new QHBoxLayout();
  QHBoxLayout *axisOrderLayout = new QHBoxLayout();
  QHBoxLayout *okButtonLayout = new QHBoxLayout();

  nbGrads = new QSpinBox();
  okButton = new QPushButton("OK");
  axisOrder = new QComboBox();
  log10Scale = new QCheckBox("Logarithmic scale (base 10)");
  log10Scale->setChecked(axis->hasLog10Scale());

  nbGrads->setRange(10, axis->getAxisHeight());
  nbGrads->setValue(axis->getNbAxisGrad());
  nbGradsLayout->addWidget(new QLabel("Number of graduations : "));
  nbGradsLayout->addWidget(nbGrads);

  axisMinLayout->addWidget(new QLabel("Axis min value : "));

  if (axis->getAxisDataTypeName() == "int") {
    intAxisMinValue = new QSpinBox();
    intAxisMinValue->setMaximum((int)axis->getAssociatedPropertyMinValue());
    intAxisMinValue->setMinimum(INT_MIN);
    intAxisMinValue->setValue((int)axis->getAxisMinValue());
    axisMinLayout->addWidget(intAxisMinValue);
  }
  else {
    doubleAxisMinValue = new QDoubleSpinBox();
    doubleAxisMinValue->setMaximum(axis->getAssociatedPropertyMinValue());
    doubleAxisMinValue->setMinimum(-DBL_MAX);
    doubleAxisMinValue->setValue(axis->getAxisMinValue());
    axisMinLayout->addWidget(doubleAxisMinValue);
  }

  axisMaxLayout->addWidget(new QLabel("Axis max value : "));

  if (axis->getAxisDataTypeName() == "int") {
    intAxisMaxValue = new QSpinBox();
    intAxisMaxValue->setMinimum((int)axis->getAssociatedPropertyMaxValue());
    intAxisMaxValue->setMaximum(INT_MAX);
    intAxisMaxValue->setValue((int)axis->getAxisMaxValue());
    axisMaxLayout->addWidget(intAxisMaxValue);
  }
  else {
    doubleAxisMaxValue = new QDoubleSpinBox();
    doubleAxisMaxValue->setMinimum(axis->getAssociatedPropertyMaxValue());
    doubleAxisMaxValue->setMaximum(DBL_MAX);
    doubleAxisMaxValue->setValue(axis->getAxisMaxValue());
    axisMaxLayout->addWidget(doubleAxisMaxValue);
  }

  axisOrder->addItem("ascending");
  axisOrder->addItem("descending");

  if (axis->hasAscendingOrder()) {
    axisOrder->setCurrentIndex(0);
  }
  else {
    axisOrder->setCurrentIndex(1);
  }

  axisOrderLayout->addWidget(new QLabel("Axis order : "));
  axisOrderLayout->addWidget(axisOrder);

  dialogLayout->addLayout(nbGradsLayout);
  dialogLayout->addLayout(axisMinLayout);
  dialogLayout->addLayout(axisMaxLayout);
  dialogLayout->addLayout(axisOrderLayout);
  dialogLayout->addWidget(log10Scale);
  okButtonLayout->addStretch(1);
  okButtonLayout->addWidget(okButton);
  okButtonLayout->addStretch(1);
  dialogLayout->addLayout(okButtonLayout);

  setLayout(dialogLayout);
  connect(okButton,SIGNAL(clicked()),this,SLOT(close()));

  resize(300, 100);

}

void QuantitativeAxisConfigDialog::closeEvent (QCloseEvent*) {
  axis->setNbAxisGrad(nbGrads->value());
  axis->setAscendingOrder(axisOrder->currentText() == "ascending");
  axis->setLog10Scale(log10Scale->isChecked());

  if (axis->getAxisDataTypeName() == "int") {
    axis->setAxisMinMaxValues(intAxisMinValue->value(), intAxisMaxValue->value());
  }
  else {
    axis->setAxisMinMaxValues(doubleAxisMinValue->value(), doubleAxisMaxValue->value());
  }

  axis->redraw();
}

NominalAxisConfigDialog::NominalAxisConfigDialog(NominalParallelAxis *axis) : QDialog(), axis(axis) {

  setWindowTitle("Nominal Axis Configuration");

  QVBoxLayout *dialogLayout = new QVBoxLayout();
  QVBoxLayout *buttonsUpDownLayout = new QVBoxLayout();
  QHBoxLayout *labelsOrderLayout = new QHBoxLayout();
  QHBoxLayout *okButtonLayout = new QHBoxLayout();

  okButton = new QPushButton("OK");
  axisLabelsOrder = new ItemsListWidget(this);
  QPushButton* up = new QPushButton(QIcon(":/tulip/gui/icons/16/go-up.png"),"");
  QPushButton* lexOrder = new QPushButton(QString("Lexicographic") + QChar(QChar::LineSeparator) +  QString("     order"));
  QPushButton *down = new QPushButton(QIcon(":/tulip/gui/icons/16/go-down.png"),"");


  buttonsUpDownLayout->addWidget(up);
  buttonsUpDownLayout->addWidget(lexOrder);
  buttonsUpDownLayout->addWidget(down);

  labelsOrderLayout->addWidget(axisLabelsOrder);
  labelsOrderLayout->addLayout(buttonsUpDownLayout);

  dialogLayout->addWidget(new QLabel(tr("Axis Labels Order :")));
  dialogLayout->addLayout(labelsOrderLayout);
  okButtonLayout->addStretch(1);
  okButtonLayout->addWidget(okButton);
  okButtonLayout->addStretch(1);
  dialogLayout->addLayout(okButtonLayout);

  vector<string> labelsOrder = axis->getLabelsOrder();
  vector<string>::reverse_iterator it;

  for (it = labelsOrder.rbegin() ; it != labelsOrder.rend() ; ++it) {
    axisLabelsOrder->addItem(QString::fromUtf8((*it).c_str()));
  }

  setLayout(dialogLayout);

  connect(okButton,SIGNAL(clicked()),this,SLOT(close()));
  connect(up,SIGNAL(clicked()),this,SLOT(pressButtonUp()));
  connect(down,SIGNAL(clicked()),this,SLOT(pressButtonDown()));
  connect(lexOrder,SIGNAL(clicked()),this,SLOT(pressButtonLexOrder()));
}

void NominalAxisConfigDialog::closeEvent (QCloseEvent *) {
  vector<string> labelsOrder;

  for (int i = 0; i < axisLabelsOrder->count(); ++i) {
    labelsOrder.push_back(axisLabelsOrder->item(i)->text().toUtf8().data());
  }

  reverse(labelsOrder.begin(), labelsOrder.end());
  axis->setLabelsOrder(labelsOrder);
  axis->redraw();
}

void NominalAxisConfigDialog::pressButtonUp() {
  if (axisLabelsOrder->count() > 0) {
    int row = axisLabelsOrder->currentRow();

    if (row > 0) {
      QString s = axisLabelsOrder->currentItem()->text();
      QString s2 = axisLabelsOrder->item(row - 1)->text();
      axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row - 1));
      axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row - 1));
      axisLabelsOrder->insertItem(row - 1, s2);
      axisLabelsOrder->insertItem(row - 1, s);
      axisLabelsOrder->setCurrentRow(row - 1);
    }
  }
}

void NominalAxisConfigDialog::pressButtonDown() {
  if (axisLabelsOrder->count() > 0) {
    int row = axisLabelsOrder->currentRow();

    if (row != -1 && row < (axisLabelsOrder->count() - 1)) {
      QString s = axisLabelsOrder->currentItem()->text();
      QString s2 = axisLabelsOrder->item(row + 1)->text();
      axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row));
      axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row));
      axisLabelsOrder->insertItem(row, s);
      axisLabelsOrder->insertItem(row, s2);
      axisLabelsOrder->setCurrentRow(row + 1);
    }
  }
}

void NominalAxisConfigDialog::pressButtonLexOrder() {
  axisLabelsOrder->clear();
  static unsigned int clickCount = 0;
  vector<string> labelsOrder = axis->getLabelsOrder();
  std::sort(labelsOrder.begin(), labelsOrder.end());

  if (++clickCount % 2 == 1) {
    vector<string>::iterator it;

    for (it = labelsOrder.begin() ; it != labelsOrder.end() ; ++it) {
      axisLabelsOrder->addItem(QString::fromUtf8((*it).c_str()));
    }
  }
  else {
    vector<string>::reverse_iterator it;

    for (it = labelsOrder.rbegin() ; it != labelsOrder.rend() ; ++it) {
      axisLabelsOrder->addItem(QString::fromUtf8((*it).c_str()));
    }
  }

}

}
