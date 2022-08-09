/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/ColorScalesManager.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TlpTools.h>
#include <tulip/TulipSettings.h>

#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QVariantMap>
#include <QColor>

using namespace std;
using namespace tlp;

void ColorScalesManager::getColorScalesFromDir(const string &colorScalesDir,
                                               list<string> &colorScalesList) {

  QFileInfo colorscaleDirectory(tlpStringToQString(colorScalesDir));

  if (colorscaleDirectory.exists() && colorscaleDirectory.isDir()) {
    QDir dir(colorscaleDirectory.absoluteFilePath());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);

      if (fileInfo.isDir()) {
        getColorScalesFromDir(QStringToTlpString(fileInfo.absoluteFilePath()), colorScalesList);
      } else if (fileInfo.suffix() == "png") {
        QString colorScaleName = fileInfo.fileName();
        colorScaleName.replace(".png", "");
        colorScalesList.push_back(QStringToTlpString(colorScaleName));
      }
    }
  }
}

list<string> ColorScalesManager::getColorScalesList() {

  list<string> ret;
  getColorScalesFromDir(TulipBitmapDir + "colorscales", ret);

  TulipSettings::instance().beginGroup("ColorScales");
  QStringList savedColorScalesIdList = TulipSettings::instance().childKeys();

  for (int i = 0; i < savedColorScalesIdList.size(); ++i) {
    if (!savedColorScalesIdList.at(i).contains("_gradient?"))
      ret.push_back(QStringToTlpString(savedColorScalesIdList.at(i)));
  }

  TulipSettings::instance().endGroup();

  TulipSettings::instance().beginGroup("ColorScalesNoRegular");
  savedColorScalesIdList = TulipSettings::instance().childKeys();

  for (int i = 0; i < savedColorScalesIdList.size(); ++i) {
    if (!savedColorScalesIdList.at(i).contains("_gradient?"))
      ret.push_back(QStringToTlpString(savedColorScalesIdList.at(i)));
  }

  TulipSettings::instance().endGroup();

  return ret;
}

static ColorScale getColorScaleFromImageFile(const QString &imageFilePath) {
  QImage gradientImage(imageFilePath);
  unsigned int imageHeight = gradientImage.height();

  unsigned int step = 1;

  if (imageHeight > 50)
    step = 10;

  vector<Color> colors;

  for (unsigned int i = 0; i < imageHeight; i += step) {
    QRgb pixelValue = gradientImage.pixel(0, i);
    colors.emplace_back(qRed(pixelValue), qGreen(pixelValue), qBlue(pixelValue),
                        qAlpha(pixelValue));
  }

  if (imageHeight % step != 0) {
    QRgb pixelValue = gradientImage.pixel(0, imageHeight - 1);
    colors.emplace_back(qRed(pixelValue), qGreen(pixelValue), qBlue(pixelValue),
                        qAlpha(pixelValue));
  }

  reverse(colors.begin(), colors.end());
  return ColorScale(colors);
}

string ColorScalesManager::findColorScaleFile(const string &rootDir, const string &colorScaleName) {
  QFileInfo colorscaleDirectory(tlpStringToQString(rootDir));

  if (colorscaleDirectory.exists() && colorscaleDirectory.isDir()) {
    QDir dir(colorscaleDirectory.absoluteFilePath());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);

      if (fileInfo.isDir()) {
        string &&ret =
            findColorScaleFile(QStringToTlpString(fileInfo.absoluteFilePath()), colorScaleName);

        if (!ret.empty()) {
          return std::move(ret);
        }
      } else if (fileInfo.suffix() == "png" &&
                 QStringToTlpString(fileInfo.baseName()) == colorScaleName) {
        return QStringToTlpString(fileInfo.absoluteFilePath());
      }
    }
  }

  return string();
}

ColorScale ColorScalesManager::getColorScale(const string &colorScaleName) {
  string &&colorScaleFile = findColorScaleFile(TulipBitmapDir + "colorscales", colorScaleName);

  if (!colorScaleFile.empty()) {
    return getColorScaleFromImageFile(tlpStringToQString(colorScaleFile));
  }

  map<float, Color> colorsMap;
  bool gradient = true;

  TulipSettings::instance().beginGroup("ColorScales");

  if (TulipSettings::instance().contains(colorScaleName.c_str())) {
    QList<QVariant> colorsListv = TulipSettings::instance().value(colorScaleName.c_str()).toList();
    std::reverse(colorsListv.begin(), colorsListv.end());

    for (int i = 0; i < colorsListv.size(); ++i) {
      QColor color = colorsListv.at(i).value<QColor>();
      float stop = i / float(colorsListv.size() - 1);
      colorsMap[stop] = QColorToColor(color);
    }
  }

  QString gradientScaleId = tlpStringToQString(colorScaleName) + "_gradient?";
  gradient = TulipSettings::instance().value(gradientScaleId).toBool();
  TulipSettings::instance().endGroup();

  TulipSettings::instance().beginGroup("ColorScalesNoRegular");

  if (TulipSettings::instance().contains(colorScaleName.c_str())) {
    QVariantMap colorsMapv = TulipSettings::instance().value(colorScaleName.c_str()).toMap();

    for (QVariantMap::iterator it = colorsMapv.begin(); it != colorsMapv.end(); ++it) {
      colorsMap[(it.key()).toDouble()] = QColorToColor(it.value().value<QColor>());
    }

    gradient = TulipSettings::instance().value(gradientScaleId).toBool();
  }

  return ColorScale(colorsMap, gradient);
}

void ColorScalesManager::registerColorScale(const string &colorScaleName,
                                            const ColorScale &colorScale) {
  list<string> colorScalesList = ColorScalesManager::getColorScalesList();

  if (find(colorScalesList.begin(), colorScalesList.end(), colorScaleName) !=
      colorScalesList.end()) {
    if (colorScale.hasRegularStops()) {
      QList<QVariant> colorsVector;

      for (unsigned int i = 0; i < const_cast<ColorScale &>(colorScale).getStopsCount(); ++i) {
        float stop = i / float(const_cast<ColorScale &>(colorScale).getStopsCount() - 1);
        Color color = colorScale.getColorAtPos(stop);
        colorsVector.push_front(QVariant(colorToQColor(color)));
      }

      TulipSettings::instance().beginGroup("ColorScales");
      TulipSettings::instance().setValue(tlpStringToQString(colorScaleName), colorsVector);
      QString gradientId = tlpStringToQString(colorScaleName) + "_gradient?";
      TulipSettings::instance().setValue(gradientId, colorScale.isGradient());
      TulipSettings::instance().endGroup();
    } else {
      QVariantMap colorsMap;

      for (const auto &it : colorScale.getColorMap()) {
        colorsMap[QString::number(it.first)] = colorToQColor(it.second);
      }

      TulipSettings::instance().beginGroup("ColorScalesNoRegular");
      TulipSettings::instance().setValue(tlpStringToQString(colorScaleName), colorsMap);
      QString gradientId = tlpStringToQString(colorScaleName) + "_gradient?";
      TulipSettings::instance().setValue(gradientId, colorScale.isGradient());
      TulipSettings::instance().endGroup();
    }
  }
}

void ColorScalesManager::removeColorScale(const string &colorScaleName) {
  TulipSettings::instance().beginGroup("ColorScales");
  QString savedColorScaleId = tlpStringToQString(colorScaleName);

  if (TulipSettings::instance().contains(savedColorScaleId)) {
    TulipSettings::instance().remove(savedColorScaleId);
    TulipSettings::instance().remove(savedColorScaleId + "_gradient?");
  }

  TulipSettings::instance().endGroup();
  TulipSettings::instance().beginGroup("ColorScalesNoRegular");
  savedColorScaleId = tlpStringToQString(colorScaleName);

  if (TulipSettings::instance().contains(savedColorScaleId)) {
    TulipSettings::instance().remove(savedColorScaleId);
    TulipSettings::instance().remove(savedColorScaleId + "_gradient?");
  }

  TulipSettings::instance().endGroup();
}

void ColorScalesManager::setLatestColorScale(ColorScale &cs) {
  QList<QVariant> colors;
  QList<QVariant> stops;

  for (const auto &it : cs.getColorMap()) {
    const Color &c = it.second;
    QColor qc(c.getR(), c.getG(), c.getB(), c.getA());
    colors.push_back(QVariant(qc));
    stops.push_back(QVariant(it.first));
  }

  TulipSettings::instance().beginGroup("viewLatestColorScale");
  TulipSettings::instance().setValue("colors", colors);
  TulipSettings::instance().setValue("stops", stops);
  TulipSettings::instance().setValue("gradient?", cs.isGradient());
  TulipSettings::instance().endGroup();
}

ColorScale ColorScalesManager::getLatestColorScale() {
  TulipSettings::instance().beginGroup("viewLatestColorScale");

  if (TulipSettings::instance().contains("colors")) {
    QList<QVariant> colorsListv = TulipSettings::instance().value("colors").toList();
    QList<QVariant> stopsListv = TulipSettings::instance().value("stops").toList();
    bool gradient = TulipSettings::instance().value("gradient?").toBool();

    map<float, Color> cm;

    for (int i = 0; i < colorsListv.size(); ++i) {
      QColor color = colorsListv.at(i).value<QColor>();
      float stop = stopsListv.at(i).value<float>();
      cm[stop] = QColorToColor(color);
    }

    TulipSettings::instance().endGroup();
    return ColorScale(cm, gradient);
  }

  TulipSettings::instance().endGroup();
  return ColorScale();
}
