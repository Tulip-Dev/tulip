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
  vector<string> ret;

  for (const string &propertyName : graph->getProperties()) {
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
  connect(_ui->createLatLngPropsCB, SIGNAL(clicked(bool)), _ui->resetLatLngValuesCB,
          SLOT(setEnabled(bool)));
}

GeolocalisationConfigWidget::~GeolocalisationConfigWidget() {
  delete _ui;
}

void GeolocalisationConfigWidget::setGraph(Graph *graph) {
  _ui->addressPropCB->clear();
  vector<string> &&stringProperties = getGraphPropertiesListAccordingToType(graph, "string");

  for (auto &sp : stringProperties) {
    _ui->addressPropCB->addItem(tlpStringToQString(sp));

    // set viewLabel as the default address property
    if (sp == "viewLabel")
      _ui->addressPropCB->setCurrentIndex(_ui->addressPropCB->count() - 1);
  }

  _ui->latPropCB->clear();
  _ui->lngPropCB->clear();
  vector<string> &&doubleProperties = getGraphPropertiesListAccordingToType(graph, "double");

  for (auto &dp : doubleProperties) {
    _ui->latPropCB->addItem(tlpStringToQString(dp));
    _ui->lngPropCB->addItem(tlpStringToQString(dp));
  }

  _ui->edgesPathsPropertyCB->clear();
  vector<string> &&doubleVectorProperties =
      getGraphPropertiesListAccordingToType(graph, "vector<double>");

  for (auto &dvp : doubleVectorProperties) {
    _ui->edgesPathsPropertyCB->addItem(tlpStringToQString(dvp));
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
    _ui->edgesPathsCB->setChecked(true);
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

bool GeolocalisationConfigWidget::resetLatAndLngValues() const {
  return _ui->resetLatLngValuesCB->isChecked();
}

bool GeolocalisationConfigWidget::automaticChoice() const {
  return _ui->automaticChoiceCB->isChecked();
}

void GeolocalisationConfigWidget::enableDisableComboBoxes() {
  bool byAddress = _ui->addressLocRB->isChecked();
  _ui->addressPropCB->setEnabled(byAddress);
  _ui->automaticChoiceCB->setEnabled(byAddress);
  _ui->createLatLngPropsCB->setEnabled(byAddress);
  _ui->resetLatLngValuesCB->setEnabled(byAddress);
  _ui->latPropCB->setEnabled(!byAddress);
  _ui->lngPropCB->setEnabled(!byAddress);
  _ui->edgesControlPointsGB->setEnabled(!byAddress);
}

bool GeolocalisationConfigWidget::useEdgesPaths() const {
  return _ui->edgesPathsCB->isChecked();
}

std::string GeolocalisationConfigWidget::getEdgesPathsPropertyName() const {
  return QStringToTlpString(_ui->edgesPathsPropertyCB->currentText());
}
