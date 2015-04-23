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

#include "ComputeSOMWidget.h"
#include "ui_ComputeSOMWidget.h"

using namespace tlp;
using namespace std;

ComputeSOMWidget::ComputeSOMWidget(QWidget* parent):QWidget(parent),_ui(new Ui::SOMComputeWidget) {
  _ui->setupUi(this);
}

ComputeSOMWidget::~ComputeSOMWidget() {
  delete _ui;
}

unsigned ComputeSOMWidget::number() const {
  return _ui->iterationNumberSpinBox->value();
}

void ComputeSOMWidget::setNumber(unsigned uintValue) {
  return _ui->iterationNumberSpinBox->setValue(uintValue);
}

void ComputeSOMWidget::clearLists() {
  _ui->propertiesConfigurationWidget->clearLists();
}

vector<string> ComputeSOMWidget::getSelectedProperties() {
  return _ui->propertiesConfigurationWidget->getSelectedProperties();
}

void ComputeSOMWidget::setWidgetParameters(Graph *g, vector<string> &propertyFilterType) {
  _ui->propertiesConfigurationWidget->setWidgetParameters(g, propertyFilterType);
}

void ComputeSOMWidget::setOutputPropertiesList(std::vector<std::string> &properties) {
  _ui->propertiesConfigurationWidget->setOutputPropertiesList(properties);
}
