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

#include "FiltersManagerAlgorithmItem.h"
#include "ui_FiltersManagerAlgorithmItem.h"

#include <QStandardItem>
#include <QComboBox>

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/TulipItemDelegate.h>

using namespace tlp;
using namespace std;

FiltersManagerAlgorithmItem::FiltersManagerAlgorithmItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerAlgorithmItem) {
  _ui->setupUi(this);

  fillTitle(_ui->algorithmCombo,trUtf8("Select filter"));
  list<string> booleanAlgorithms = PluginLister::instance()->availablePlugins<BooleanAlgorithm>();

  for(list<string>::const_iterator it = booleanAlgorithms.begin(); it != booleanAlgorithms.end(); ++it) {
    string s(*it);
    _ui->algorithmCombo->addItem(s.c_str());
  }

  connect(_ui->algorithmCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(algorithmSelected(int)));
  _ui->algorithmParams->setItemDelegate(new TulipItemDelegate);
  connect(_ui->algorithmParams, SIGNAL(destroyed()), _ui->algorithmParams->itemDelegate(), SLOT(deleteLater()));

  updateGraphModel(_ui->algorithmParams,QString::null,_graph);
}

FiltersManagerAlgorithmItem::~FiltersManagerAlgorithmItem() {
  delete _ui;
}

void FiltersManagerAlgorithmItem::algorithmSelected(int i) {
  _ui->algorithmParams->setEnabled(i != 0);
  QString algName = QString::null;

  if (i != 0)
    algName = _ui->algorithmCombo->itemText(i);

  updateGraphModel(_ui->algorithmParams,algName,_graph);
  emit titleChanged();
}

void FiltersManagerAlgorithmItem::applyFilter(BooleanProperty* prop) {
  if (_ui->algorithmCombo->currentIndex() == 0)
    return;

  ParameterListModel* model = dynamic_cast<ParameterListModel*>(_ui->algorithmParams->model());
  string msg;

  if (model != NULL) {
    DataSet data = model->parametersValues();
    _graph->applyPropertyAlgorithm(_ui->algorithmCombo->currentText().toStdString(),
                                   prop,
                                   msg,
                                   0,
                                   &data);
  }
  else {
    _graph->applyPropertyAlgorithm(_ui->algorithmCombo->currentText().toStdString(),
                                   prop,
                                   msg);
  }
}

QString FiltersManagerAlgorithmItem::title() const {
  if (_ui->algorithmCombo->currentIndex() == 0)
    return trUtf8("Select filtering method");

  return _ui->algorithmCombo->currentText();
}

void FiltersManagerAlgorithmItem::graphChanged() {
  if (_ui->algorithmCombo->currentIndex() != 0)
    updateGraphModel(_ui->algorithmParams,_ui->algorithmCombo->currentText(),_graph);
}
