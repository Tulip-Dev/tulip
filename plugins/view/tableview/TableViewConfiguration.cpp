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

#include "TableViewConfiguration.h"

#include <tulip/GraphPropertiesModel.h>
#include <tulip/BooleanProperty.h>
#include "ui_TableViewConfiguration.h"

using namespace tlp;

TableViewConfiguration::TableViewConfiguration(QWidget *parent): QWidget(parent), _ui(new Ui::TableViewConfiguration) {
  _ui->setupUi(this);
  connect(_ui->filteringPropertyCombo,SIGNAL(currentIndexChanged(int)),this,SIGNAL(settingsChanged()));
}

TableViewConfiguration::~TableViewConfiguration() {
  delete _ui;
}

void TableViewConfiguration::setGraph(tlp::Graph *g) {
  GraphPropertiesModel<BooleanProperty>* model = new GraphPropertiesModel<BooleanProperty>(trUtf8("No filtering"),g,_ui->filteringPropertyCombo);
  _ui->filteringPropertyCombo->setModel(model);
  _ui->filteringPropertyCombo->setCurrentIndex(0);
}

BooleanProperty* TableViewConfiguration::filteringProperty() const {
  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  PropertyInterface* pi = model->data(model->index(_ui->filteringPropertyCombo->currentIndex(),0),TulipModel::PropertyRole).value<PropertyInterface*>();
  return dynamic_cast<BooleanProperty*>(pi);
}

void TableViewConfiguration::setFilteringProperty(QString name) {
  GraphPropertiesModel<BooleanProperty>* model = static_cast<GraphPropertiesModel<BooleanProperty>*>(_ui->filteringPropertyCombo->model());
  int r = 0;

  if (!name.isEmpty())
    r = model->rowOf(model->graph()->getProperty<BooleanProperty>(name.toStdString()));

  if (r < 0)
    r=0;

  _ui->filteringPropertyCombo->setCurrentIndex(r);
}
