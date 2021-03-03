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

#include "GeographicViewConfigWidget.h"
#include "ui_GeographicViewConfigWidget.h"
#include "GeographicView.h"

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

#include <tulip/Perspective.h>
#include <tulip/TlpQtTools.h>

using namespace std;
using namespace tlp;

GeographicViewConfigWidget::GeographicViewConfigWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::GeographicViewConfigWidgetData), _oldPolyFileType(None),
      _oldFileLoaded("") {
  _ui->setupUi(this);
}

GeographicViewConfigWidget::~GeographicViewConfigWidget() {
  delete _ui;
}

void GeographicViewConfigWidget::openCsvFileBrowser() {
  _ui->csvFile->setText(
      QFileDialog::getOpenFileName(Perspective::instance()->mainWindow()->centralWidget(),
                                   "Open csv file", "./", "cvs file (*.*)"));
}

void GeographicViewConfigWidget::openPolyFileBrowser() {
  _ui->polyFile->setText(
      QFileDialog::getOpenFileName(Perspective::instance()->mainWindow()->centralWidget(),
                                   "Open .poly file", "./", "Poly file (*.poly)"));
}

void GeographicViewConfigWidget::openCsvHelp() {
  QMessageBox::about(Perspective::instance()->mainWindow()->centralWidget(), "Map csv file format",
                     "If you want to import a csv file into this view, your file must be in the "
                     "format:\nid\tlng\tlat\nid\tlng\tlat\n...\nwith id: id of the polygon");
}

void GeographicViewConfigWidget::openPolyHelp() {
  QMessageBox::about(Perspective::instance()->mainWindow()->centralWidget(), "Map poly files",
                     ".poly files format are an open street map "
                     "format.\nYou can download .poly file on "
                     ":\nhttp://downloads.cloudmade.com/");
}

void GeographicViewConfigWidget::openTileServerUrlHelp() {
  QMessageBox::about(Perspective::instance()->mainWindow()->centralWidget(),
                     "Url of the tile server",
                     R"(
<p>You must give the url of a map tiles server.<br/>
It can be:
<ul>
<li>the url of a commercial tiles provider, needing a user registered key,
 as for example, the url of a <b>MapTiler</b> server:<br/>
<b>https://api.maptiler.com/maps/streets/{z}/{x}/{y}{r}.png?key=&lt;your own key&gt;</b>,</li>
<li>the url of a free tiles server, such as the <b>Stamen Toner</b> one:<br/>
<b>http://a.tile.stamen.com/toner/{z}/{x}/{y}.png</b>,</li>
<li>or the url of your own map tiles server.</li></ul></p>
)");
}

void GeographicViewConfigWidget::openTilesAttributionHelp() {
  QMessageBox::about(Perspective::instance()->mainWindow()->centralWidget(), "Tiles attribution",
                     R"(
<p>If you need to publish some images produced by the
 <b>Geographic View</b>, these images must display the tiles
 attribution, i.e. the name(s) of the various contributors
 of the tiles you are using, as indicated by the tiles
 provider and as it is automatically done for the tiles servers
 proposed by Tulip.<br/>
For example, if you are using the tiles provided by one of the
 <b>USGS</b> servers, you should then set the tiles attribution to<br/>
<b>Tiles courtesy of the &lt;a href="https://usgs.gov/"&gt;U.S. Geological Survey&lt;/a&gt;</b>.</p>
)");
}

bool GeographicViewConfigWidget::useSharedLayoutProperty() const {
  return _ui->layoutCheckBox->isChecked();
}

bool GeographicViewConfigWidget::useSharedSizeProperty() const {
  return _ui->sizeCheckBox->isChecked();
}

GeographicViewConfigWidget::PolyFileType GeographicViewConfigWidget::polyFileType() const {
  _ui->mapToPolygon->setEnabled(false);

  if (_ui->useDefaultShape->isChecked())
    return Default;

  if (_ui->useCsvFile->isChecked())
    return CsvFile;

  if (_ui->usePolyFile->isChecked()) {
    _ui->mapToPolygon->setEnabled(true);
    return PolyFile;
  }

  return Default;
}

void GeographicViewConfigWidget::setPolyFileType(PolyFileType &fileType) {
  _ui->mapToPolygon->setEnabled(false);

  if (fileType == Default)
    _ui->useDefaultShape->setChecked(true);

  if (fileType == CsvFile)
    _ui->useCsvFile->setChecked(true);

  if (fileType == PolyFile) {
    _ui->usePolyFile->setChecked(true);
    _ui->mapToPolygon->setEnabled(true);
  }
}

QString GeographicViewConfigWidget::getCsvFile() const {
  return _ui->csvFile->text();
}

QString GeographicViewConfigWidget::getPolyFile() const {
  return _ui->polyFile->text();
}

bool GeographicViewConfigWidget::useSharedShapeProperty() const {
  return _ui->layoutCheckBox->isChecked();
}

bool GeographicViewConfigWidget::polyOptionsChanged() {
  if (polyFileType() != _oldPolyFileType) {
    _oldPolyFileType = polyFileType();

    switch (_oldPolyFileType) {
    case Default: {
      _oldFileLoaded = "";
      break;
    }

    case CsvFile: {
      _oldFileLoaded = QStringToTlpString(_ui->csvFile->text());
      break;
    }

    case PolyFile: {
      _oldFileLoaded = QStringToTlpString(_ui->polyFile->text());
      break;
    }

    default:
      break;
    }

    return true;
  } else {
    switch (_oldPolyFileType) {
    case CsvFile: {
      string file = QStringToTlpString(_ui->csvFile->text());

      if (_oldFileLoaded != file) {
        _oldFileLoaded = file;
        return true;
      }

      break;
    }

    case PolyFile: {
      string file = QStringToTlpString(_ui->polyFile->text());

      if (_oldFileLoaded != file) {
        _oldFileLoaded = file;
        return true;
      }

      break;
    }

    default:
      break;
    }
  }

  return false;
}

void GeographicViewConfigWidget::setState(const DataSet &dataSet) {
  {
    PolyFileType polyFileType;
    int type = 0;

    if (dataSet.get("polyFileType", type)) {
      polyFileType = static_cast<PolyFileType>(type);
      setPolyFileType(polyFileType);
    }
  }

  if (dataSet.exists("csvFileName")) {
    string fileName;
    dataSet.get("csvFileName", fileName);
    _ui->csvFile->setText(tlpStringToQString(fileName));
  }

  if (dataSet.exists("polyFileName")) {
    string fileName;
    dataSet.get("polyFileName", fileName);
    _ui->polyFile->setText(tlpStringToQString(fileName));
  }

  bool useShared = false;

  if (dataSet.get("useSharedLayout", useShared))
    _ui->layoutCheckBox->setChecked(useShared);

  if (dataSet.get("useSharedSize", useShared))
    _ui->sizeCheckBox->setChecked(useShared);

  if (dataSet.get("useSharedShape", useShared))
    _ui->shapeCheckBox->setChecked(useShared);

  string customInfos;
  if (dataSet.get("customTileLayerUrl", customInfos))
    _ui->customTileLayerUrl->setText(tlpStringToQString(customInfos));
  if (dataSet.get("customTilesAttribution", customInfos))
    _ui->customTileLayerAttribution->setText(tlpStringToQString(customInfos));
}

DataSet GeographicViewConfigWidget::state() const {
  DataSet data;
  data.set("polyFileType", int(polyFileType()));
  data.set("csvFileName", QStringToTlpString(_ui->csvFile->text()));
  data.set("polyFileName", QStringToTlpString(_ui->polyFile->text()));
  data.set("useSharedLayout", useSharedLayoutProperty());
  data.set("useSharedSize", useSharedSizeProperty());
  data.set("useSharedShape", useSharedShapeProperty());
  data.set("customTileLayerUrl", QStringToTlpString(getCustomTileLayerUrl()));
  data.set("customTilesAttribution", QStringToTlpString(getCustomTilesAttribution()));
  return data;
}

QString GeographicViewConfigWidget::getCustomTileLayerUrl() const {
  return _ui->customTileLayerUrl->text();
}

QString GeographicViewConfigWidget::getCustomTilesAttribution() const {
  return _ui->customTileLayerAttribution->text();
}
