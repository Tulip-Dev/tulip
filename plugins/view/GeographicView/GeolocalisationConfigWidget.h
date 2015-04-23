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

#ifndef GEOLOCALISATIONCONFIGWIDGET_H_
#define GEOLOCALISATIONCONFIGWIDGET_H_

#include <QWidget>

namespace Ui {
class GeolocalisationConfigWidgetData;
}

namespace tlp {

class Graph;

class GeolocalisationConfigWidget : public QWidget  {

  Q_OBJECT
  Ui::GeolocalisationConfigWidgetData* _ui;

public :

  GeolocalisationConfigWidget(QWidget *parent = 0);
  ~GeolocalisationConfigWidget();

  void setGraph(Graph *graph);

  void setLatLngGeoLocMethod();

  bool geolocateByAddress() const;

  bool createLatAndLngProperties() const;

  std::string getAddressGraphPropertyName() const;

  std::string getLatitudeGraphPropertyName() const;

  std::string getLongitudeGraphPropertyName() const;

public slots :

  void enableDisableComboBoxes();

signals:
  void computeGeoLayout();


};
}

#endif /* GOOGLEMAPSVIEWCONFIGWIDGET_H_ */
