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

#ifndef GEOGRAPHIC_VIEW_CONFIG_WIDGET_H
#define GEOGRAPHIC_VIEW_CONFIG_WIDGET_H

#include <QWidget>

#include <tulip/DataSet.h>

namespace Ui {
class GeographicViewConfigWidgetData;
}

namespace tlp {
class GeographicViewConfigWidget : public QWidget {

  Q_OBJECT

  Ui::GeographicViewConfigWidgetData *_ui;

public:
  enum PolyFileType { None = -1, Default = 0, CsvFile, PolyFile };

  GeographicViewConfigWidget(QWidget *parent = nullptr);
  ~GeographicViewConfigWidget() override;

  bool useSharedLayoutProperty() const;
  bool useSharedSizeProperty() const;
  bool useSharedShapeProperty() const;

  PolyFileType polyFileType() const;
  void setPolyFileType(PolyFileType &fileType);
  QString getCsvFile() const;
  QString getPolyFile() const;

  bool polyOptionsChanged();

  QString getCustomTileLayerUrl() const;
  QString getCustomTilesAttribution() const;

  void setState(const DataSet &dataSet);

  DataSet state() const;

signals:

  void mapToPolygonSignal();

public slots:
  void openCsvFileBrowser();
  void openPolyFileBrowser();
  void openCsvHelp();
  void openPolyHelp();
  void openTileServerUrlHelp();
  void openTilesAttributionHelp();

  void mapToPolygonSlot() {
    emit mapToPolygonSignal();
  }

protected:
  PolyFileType _oldPolyFileType;
  std::string _oldFileLoaded;
};
} // namespace tlp

#endif // GEOGRAPHIC_VIEW_CONFIG_WIDGET_H
