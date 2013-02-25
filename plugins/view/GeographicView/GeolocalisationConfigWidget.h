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

#ifndef GEOLOCALISATIONCONFIGWIDGET_H_
#define GEOLOCALISATIONCONFIGWIDGET_H_

#include <tulip/Graph.h>

#include "ui_GeolocalisationConfigWidget.h"

using namespace tlp;

class GeolocalisationConfigWidget : public QWidget, public Ui::GeolocalisationConfigWidgetData {

	Q_OBJECT

public :

  GeolocalisationConfigWidget(QWidget *parent = 0);

	void setGraph(Graph *graph);

	void setLatLngGeoLocMethod();

	bool geolocateByAddress() const;

	bool createLatAndLngProperties() const;

	std::string getAddressGraphPropertyName() const;

	std::string getLatitudeGraphPropertyName() const;

	std::string getLongitudeGraphPropertyName() const;

public slots :

	void enableDisableComboBoxes();


};


#endif /* GOOGLEMAPSVIEWCONFIGWIDGET_H_ */
