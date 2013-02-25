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

#include <algorithm>
#include <tulip/AbstractProperty.h>

#include "GeolocalisationConfigWidget.h"

using namespace std;

const string viewPropertiesName[] = {"viewBorderColor", "viewBorderWidth", "viewColor", "viewLabel",
		"viewLabelColor", "viewLabelPosition", "viewLayout", "viewMetaGraph",
		"viewRotation", "viewSelection", "viewShape", "viewSize",
		"viewTexture", "viewMetric"};

const unsigned int nbViewProperties = sizeof(viewPropertiesName) / sizeof(string);

const vector<string> graphViewProperties(viewPropertiesName, viewPropertiesName + nbViewProperties);

vector<string> getGraphPropertiesListAccordingToType(Graph *graph, string typeName) {
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

GeolocalisationConfigWidget::GeolocalisationConfigWidget(QWidget *parent) : QWidget(parent) {
	setupUi(this);
	connect(addressLocRB, SIGNAL(toggled(bool)), this, SLOT(enableDisableComboBoxes()));
	connect(latLngRB, SIGNAL(toggled(bool)), this, SLOT(enableDisableComboBoxes()));
}

void GeolocalisationConfigWidget::setGraph(Graph *graph) {
	addressPropCB->clear();
	vector<string> stringProperties = getGraphPropertiesListAccordingToType(graph, "string");
	for (unsigned int i = 0 ; i < stringProperties.size() ; ++i) {
		addressPropCB->addItem(QString(stringProperties[i].c_str()));
	}
	latPropCB->clear();
	lngPropCB->clear();
	vector<string> doubleProperties = getGraphPropertiesListAccordingToType(graph, "double");
	for (unsigned int i = 0 ; i < doubleProperties.size() ; ++i) {
		latPropCB->addItem(QString(doubleProperties[i].c_str()));
		lngPropCB->addItem(QString(doubleProperties[i].c_str()));
	}
}

void GeolocalisationConfigWidget::setLatLngGeoLocMethod() {
	latLngRB->setChecked(true);
	latPropCB->setCurrentIndex(latPropCB->findText("latitude"));
	lngPropCB->setCurrentIndex(lngPropCB->findText("longitude"));
}

bool GeolocalisationConfigWidget::geolocateByAddress() const {
	return addressLocRB->isChecked();
}

string GeolocalisationConfigWidget::getAddressGraphPropertyName() const {
	return addressPropCB->currentText().toStdString();
}

string GeolocalisationConfigWidget::getLatitudeGraphPropertyName() const {
	return latPropCB->currentText().toStdString();
}

string GeolocalisationConfigWidget::getLongitudeGraphPropertyName() const {
	return lngPropCB->currentText().toStdString();
}

bool GeolocalisationConfigWidget::createLatAndLngProperties() const {
	return createLatLngPropsCB->isChecked();
}

void GeolocalisationConfigWidget::enableDisableComboBoxes() {
	if (addressLocRB->isChecked()) {
		addressPropCB->setEnabled(true);
		latPropCB->setEnabled(false);
		lngPropCB->setEnabled(false);
	} else {
		addressPropCB->setEnabled(false);
		latPropCB->setEnabled(true);
		lngPropCB->setEnabled(true);
	}
}
