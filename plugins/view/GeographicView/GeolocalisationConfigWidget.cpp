/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/TlpQtTools.h>

using namespace std;
using namespace tlp;

const string viewPropertiesName[] = {
    "viewBorderColor",   "viewBorderWidth", "viewColor",     "viewFont",     "viewLabelColor",
    "viewLabelPosition", "viewLayout",      "viewMetaGraph", "viewRotation", "viewSelection",
    "viewShape",         "viewSize",        "viewTexture",   "viewMetric"};

const unsigned int nbViewProperties = sizeof(viewPropertiesName) / sizeof(string);

const vector<string> graphViewProperties(viewPropertiesName, viewPropertiesName + nbViewProperties);

static vector<string> getGraphPropertiesListAccordingToType(Graph *graph, const string &typeName) {
  Iterator<string> *propertiesIt = graph->getProperties();
  vector<string> ret;

  while (propertiesIt->hasNext()) {
    string propertyName = propertiesIt->next();

    if (std::find(graphViewProperties.begin(), graphViewProperties.end(), propertyName) ==
        graphViewProperties.end()) {
      if (graph->getProperty(propertyName)->getTypename() == typeName) {
        ret.push_back(propertyName);
      }
    }
  }

  return ret;
}

GeolocalisationConfigWidget::GeolocalisationConfigWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::GeolocalisationConfigWidgetData) {
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

  for (size_t i = 0; i < stringProperties.size(); ++i) {
    _ui->addressPropCB->addItem(tlpStringToQString(stringProperties[i]));

    // set viewLabel as the default adress property
    if (stringProperties[i] == "viewLabel")
      _ui->addressPropCB->setCurrentIndex(i);
  }

  _ui->latPropCB->clear();
  _ui->lngPropCB->clear();
  vector<string> doubleProperties = getGraphPropertiesListAccordingToType(graph, "double");

  for (unsigned int i = 0; i < doubleProperties.size(); ++i) {
    _ui->latPropCB->addItem(tlpStringToQString(doubleProperties[i]));
    _ui->lngPropCB->addItem(tlpStringToQString(doubleProperties[i]));
  }

  _ui->edgesPathsPropertyCB->clear();
  vector<string> doubleVectorProperties =
      getGraphPropertiesListAccordingToType(graph, "vector<double>");

  for (size_t i = 0; i < doubleVectorProperties.size(); ++i) {
    _ui->edgesPathsPropertyCB->addItem(tlpStringToQString(doubleVectorProperties[i]));
  }
}

void GeolocalisationConfigWidget::setLatLngGeoLocMethod(const std::string &latitudePropertyName,
                                                        const std::string &longitudePropertyName) {
  _ui->latLngRB->setChecked(true);
  int latPropIndex = _ui->latPropCB->findText(tlpStringToQString(latitudePropertyName));
  int lngPropIndex = _ui->lngPropCB->findText(tlpStringToQString(longitudePropertyName));

  if (latPropIndex != -1 && lngPropIndex != -1) {
    _ui->latPropCB->setCurrentIndex(latPropIndex);
    _ui->lngPropCB->setCurrentIndex(lngPropIndex);
  }
}

void GeolocalisationConfigWidget::setEdgesPathsPropertyName(
    const std::string &edgesPathsPropertyName) {
  int edgesPathsPropertyIndex =
      _ui->edgesPathsPropertyCB->findText(tlpStringToQString(edgesPathsPropertyName));

  if (edgesPathsPropertyIndex != -1) {
    _ui->edgesControlPointsGB->setChecked(true);
    _ui->edgesPathsPropertyCB->setCurrentIndex(edgesPathsPropertyIndex);
  }
}

bool GeolocalisationConfigWidget::geolocateByAddress() const {
  return _ui->addressLocRB->isChecked();
}

string GeolocalisationConfigWidget::getAddressGraphPropertyName() const {
  return QStringToTlpString(_ui->addressPropCB->currentText());
}

string GeolocalisationConfigWidget::getLatitudeGraphPropertyName() const {
  return QStringToTlpString(_ui->latPropCB->currentText());
}

string GeolocalisationConfigWidget::getLongitudeGraphPropertyName() const {
  return QStringToTlpString(_ui->lngPropCB->currentText());
}

bool GeolocalisationConfigWidget::createLatAndLngProperties() const {
  return _ui->createLatLngPropsCB->isChecked();
}

void GeolocalisationConfigWidget::enableDisableComboBoxes() {
  if (_ui->addressLocRB->isChecked()) {
    _ui->addressPropCB->setEnabled(true);
    _ui->latPropCB->setEnabled(false);
    _ui->lngPropCB->setEnabled(false);
  } else {
    _ui->addressPropCB->setEnabled(false);
    _ui->latPropCB->setEnabled(true);
    _ui->lngPropCB->setEnabled(true);
  }
}

bool GeolocalisationConfigWidget::useEdgesPaths() const {
  return _ui->edgesControlPointsGB->isChecked();
}

std::string GeolocalisationConfigWidget::getEdgesPathsPropertyName() const {
  return QStringToTlpString(_ui->edgesPathsPropertyCB->currentText());
}
