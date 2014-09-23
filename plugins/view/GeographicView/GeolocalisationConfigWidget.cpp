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

#include "GeolocalisationConfigWidget.h"
#include "ui_GeolocalisationConfigWidget.h"

#include <tulip/ImportModule.h>
#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

const string viewPropertiesName[] = {"viewBorderColor", "viewBorderWidth", "viewColor", "viewFont",
                                     "viewLabelColor", "viewLabelPosition", "viewLayout", "viewMetaGraph",
                                     "viewRotation", "viewSelection", "viewShape", "viewSize",
                                     "viewTexture", "viewMetric"
                                    };

const unsigned int nbViewProperties = sizeof(viewPropertiesName) / sizeof(string);

const vector<string> graphViewProperties(viewPropertiesName, viewPropertiesName + nbViewProperties);

static vector<string> getGraphPropertiesListAccordingToType(Graph *graph, const string& typeName) {
  Iterator<string> *propertiesIt = graph->getProperties();
  vector<string> ret;

  while (propertiesIt->hasNext()) {
    string propertyName = propertiesIt->next();

    if (std::find(graphViewProperties.begin(), graphViewProperties.end(), propertyName) == graphViewProperties.end()) {
      if (graph->getProperty(propertyName)->getTypename() == typeName) {
        ret.push_back(propertyName);
      }
    }
  }

  return ret;
}

GeolocalisationConfigWidget::GeolocalisationConfigWidget(QWidget *parent) : QWidget(parent),_ui(new Ui::GeolocalisationConfigWidgetData) {
  _ui->setupUi(this);
  connect(_ui->addressLocRB, SIGNAL(toggled(bool)), this, SLOT(enableDisableComboBoxes()));
  connect(_ui->latLngRB, SIGNAL(toggled(bool)), this, SLOT(enableDisableComboBoxes()));
  connect(_ui->genLayoutButton, SIGNAL(clicked()), this, SIGNAL(computeGeoLayout()));
}

GeolocalisationConfigWidget::~GeolocalisationConfigWidget() {
  delete _ui;
}

void GeolocalisationConfigWidget::setGraph(Graph *graph) {
  _ui->addressPropCB->clear();
  vector<string> stringProperties = getGraphPropertiesListAccordingToType(graph, "string");

  for (unsigned int i = 0 ; i < stringProperties.size() ; ++i) {
    _ui->addressPropCB->addItem(QString::fromUtf8(stringProperties[i].c_str()));
  }

  _ui->latPropCB->clear();
  _ui->lngPropCB->clear();
  vector<string> doubleProperties = getGraphPropertiesListAccordingToType(graph, "double");

  for (unsigned int i = 0 ; i < doubleProperties.size() ; ++i) {
    _ui->latPropCB->addItem(QString::fromUtf8(doubleProperties[i].c_str()));
    _ui->lngPropCB->addItem(QString::fromUtf8(doubleProperties[i].c_str()));
  }
}

void GeolocalisationConfigWidget::setLatLngGeoLocMethod() {
  _ui->latLngRB->setChecked(true);
  _ui->latPropCB->setCurrentIndex(_ui->latPropCB->findText("latitude"));
  _ui->lngPropCB->setCurrentIndex(_ui->lngPropCB->findText("longitude"));
}

bool GeolocalisationConfigWidget::geolocateByAddress() const {
  return _ui->addressLocRB->isChecked();
}

string GeolocalisationConfigWidget::getAddressGraphPropertyName() const {
  return string(_ui->addressPropCB->currentText().toUtf8().data());
}

string GeolocalisationConfigWidget::getLatitudeGraphPropertyName() const {
  return string(_ui->latPropCB->currentText().toUtf8().data());
}

string GeolocalisationConfigWidget::getLongitudeGraphPropertyName() const {
  return string(_ui->lngPropCB->currentText().toUtf8().data());
}

bool GeolocalisationConfigWidget::createLatAndLngProperties() const {
  return _ui->createLatLngPropsCB->isChecked();
}

void GeolocalisationConfigWidget::enableDisableComboBoxes() {
  if (_ui->addressLocRB->isChecked()) {
    _ui->addressPropCB->setEnabled(true);
    _ui->latPropCB->setEnabled(false);
    _ui->lngPropCB->setEnabled(false);
  }
  else {
    _ui->addressPropCB->setEnabled(false);
    _ui->latPropCB->setEnabled(true);
    _ui->lngPropCB->setEnabled(true);
  }
}
