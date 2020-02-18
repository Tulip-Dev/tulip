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

#ifndef GEOLOCALISATION_CONFIG_WIDGET_H
#define GEOLOCALISATION_CONFIG_WIDGET_H

#include <QWidget>

namespace Ui {
class GeolocalisationConfigWidgetData;
}

namespace tlp {

class Graph;

class GeolocalisationConfigWidget : public QWidget {

  Q_OBJECT
  Ui::GeolocalisationConfigWidgetData *_ui;

public:
  GeolocalisationConfigWidget(QWidget *parent = nullptr);
  ~GeolocalisationConfigWidget() override;

  void setGraph(Graph *graph);

  void setLatLngGeoLocMethod(const std::string &latitudePropertyName,
                             const std::string &longitudePropertyName);

  void setEdgesPathsPropertyName(const std::string &edgesPathsPropertyName);

  bool geolocateByAddress() const;

  bool createLatAndLngProperties() const;

  bool resetLatAndLngValues() const;

  bool automaticChoice() const;

  std::string getAddressGraphPropertyName() const;

  std::string getLatitudeGraphPropertyName() const;

  std::string getLongitudeGraphPropertyName() const;

  bool useEdgesPaths() const;

  std::string getEdgesPathsPropertyName() const;

public slots:

  void enableDisableComboBoxes();

signals:
  void computeGeoLayout();
};
} // namespace tlp

#endif // GEOLOCALISATION_CONFIG_WIDGET_H
