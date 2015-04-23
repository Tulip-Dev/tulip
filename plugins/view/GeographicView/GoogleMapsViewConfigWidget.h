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

#ifndef GOOGLEMAPSVIEWCONFIGWIDGET_H_
#define GOOGLEMAPSVIEWCONFIGWIDGET_H_

#include <QWidget>

#include <tulip/DataSet.h>

namespace Ui {
class GoogleMapsViewConfigWidgetData;
}

namespace tlp {
class GoogleMapsViewConfigWidget : public QWidget {

  Q_OBJECT

  Ui::GoogleMapsViewConfigWidgetData* _ui;

public :

  enum PolyFileType {
    None=-1,
    Default=0,
    CsvFile,
    PolyFile
  };

  GoogleMapsViewConfigWidget(QWidget *parent = 0);
  ~GoogleMapsViewConfigWidget();

  bool useSharedLayoutProperty() const;
  bool useSharedSizeProperty() const;
  bool useSharedShapeProperty() const;

  PolyFileType polyFileType() const;
  void setPolyFileType(PolyFileType &fileType);
  QString getCsvFile() const;
  QString getPolyFile() const;

  bool polyOptionsChanged();

  void setState(const DataSet &dataSet);

  DataSet state() const;

signals :

  void mapToPolygonSignal();

public slots:
  void openCsvFileBrowser();
  void openPolyFileBrowser();
  void openCsvHelp();
  void openPolyHelp();

  void mapToPolygonSlot() {
    emit mapToPolygonSignal();
  }

protected :

  PolyFileType _oldPolyFileType;
  std::string _oldFileLoaded;

};
}

#endif /* GOOGLEMAPSVIEWCONFIGWIDGET_H_ */
