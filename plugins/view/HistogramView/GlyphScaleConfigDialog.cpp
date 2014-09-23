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

#include <tulip/Glyph.h>
#include <tulip/Iterator.h>

#include <QComboBox>
#include <QPushButton>

#include "GlyphScaleConfigDialog.h"
#include "ui_GlyphScaleConfigDialog.h"

using namespace std;
using namespace tlp;

GlyphScaleConfigDialog::GlyphScaleConfigDialog(QWidget *parent) : QDialog(parent),_ui(new Ui::GlyphScaleConfigDialogData) {
  _ui->setupUi(this);
  _ui->tableWidget->setRowCount(5);
  list<string> pluginsList = PluginLister::instance()->availablePlugins<Glyph>();

  for(list<string>::iterator it=pluginsList.begin(); it!=pluginsList.end(); ++it) {
    string glyphName=*it;
    glyphsNameList << QString(glyphName.c_str());
  }

  for (int i = 0 ; i < 5 ; ++i) {
    QComboBox *glyphNameComboBox = new QComboBox(this);
    glyphNameComboBox->addItems(glyphsNameList);
    glyphNameComboBox->setCurrentIndex(i);
    _ui->tableWidget->setCellWidget(i, 0, glyphNameComboBox);
  }

  connect(_ui->nbGlyphsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(nbGlyphsSpinBoxValueChanged(int)));
}

GlyphScaleConfigDialog::~GlyphScaleConfigDialog() {
  delete _ui;
}

vector<int> GlyphScaleConfigDialog::getSelectedGlyphsId() const {
  vector<int> ret;

  for (int i = 0 ; i < _ui->tableWidget->rowCount() ; ++i) {
    string glyphName = ((QComboBox *)_ui->tableWidget->cellWidget(i, 0))->currentText().toStdString();
    ret.push_back(PluginLister::pluginInformation(glyphName).id());
  }

  reverse(ret.begin(), ret.end());
  return ret;
}



void GlyphScaleConfigDialog::nbGlyphsSpinBoxValueChanged(int value) {
  if (value > glyphsNameList.size()) {
    _ui->nbGlyphsSpinBox->setValue(value-1);
  }

  int lastCount = _ui->tableWidget->rowCount();
  _ui->tableWidget->setRowCount(value);

  if (lastCount < value) {
    QComboBox *glyphNameComboBox = new QComboBox(this);
    glyphNameComboBox->addItems(glyphsNameList);
    _ui->tableWidget->setCellWidget(value-1, 0, glyphNameComboBox);
  }
}

void GlyphScaleConfigDialog::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  _ui->tableWidget->setColumnWidth(0, _ui->tableWidget->width() - 23);
}
