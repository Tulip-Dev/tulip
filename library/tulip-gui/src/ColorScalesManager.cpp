#include <tulip/ColorScalesManager.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TlpTools.h>

#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QImage>
#include <QVariantMap>
#include <QColor>

using namespace std;
using namespace tlp;

list<string> ColorScalesManager::getColorScalesList() {

  list<string> ret;

  QFileInfo colorscaleDirectory(tlpStringToQString(TulipBitmapDir)+QString("colorscales"));

  if(colorscaleDirectory.exists() && colorscaleDirectory.isDir()) {
    QDir dir(colorscaleDirectory.absoluteFilePath());
    dir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      QString colorScaleName = fileInfo.fileName();
      colorScaleName.replace(".png", "");
      colorScaleName.replace("_from_ColorBrewer.org", "");
      ret.push_back(QStringToTlpString(colorScaleName));
    }
  }

  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("ColorScales");
  QStringList savedColorScalesIdList = settings.childKeys();

  for (int i = 0 ; i < savedColorScalesIdList.size() ; ++i) {
    if (!savedColorScalesIdList.at(i).contains("_gradient?"))
      ret.push_back(QStringToTlpString(savedColorScalesIdList.at(i)));
  }

  settings.endGroup();

  settings.beginGroup("ColorScalesNoRegular");
  savedColorScalesIdList = settings.childKeys();

  for (int i = 0 ; i < savedColorScalesIdList.size() ; ++i) {
    if (!savedColorScalesIdList.at(i).contains("_gradient?"))
      ret.push_back(QStringToTlpString(savedColorScalesIdList.at(i)));
  }

  settings.endGroup();

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
    colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(pixelValue), qAlpha(pixelValue)));
  }

  if (imageHeight % step != 0) {
    QRgb pixelValue = gradientImage.pixel(0, imageHeight - 1);
    colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(pixelValue), qAlpha(pixelValue)));
  }

  reverse(colors.begin(), colors.end());
  return ColorScale(colors);
}

ColorScale ColorScalesManager::getColorScale(const string &colorScaleName) {
  QString colorscaleDirectory(tlpStringToQString(TulipBitmapDir)+QString("colorscales"));

  QFileInfo colorScaleFile(colorscaleDirectory + "/" + colorScaleName.c_str() + ".png");

  if (colorScaleFile.exists()) {
    return getColorScaleFromImageFile(colorScaleFile.absoluteFilePath());
  }

  colorScaleFile = QFileInfo(colorscaleDirectory + "/" + colorScaleName.c_str() + "_from_ColorBrewer.org.png");

  if (colorScaleFile.exists()) {
    return getColorScaleFromImageFile(colorScaleFile.absoluteFilePath());
  }

  map<float, Color> colorsMap;
  bool gradient = true;

  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("ColorScales");

  if (settings.contains(colorScaleName.c_str())) {
    QList<QVariant> colorsListv = settings.value(colorScaleName.c_str()).toList();
    reverse(colorsListv.begin(), colorsListv.end());

    for (int i = 0 ; i < colorsListv.size() ; ++i) {
      QColor color = colorsListv.at(i).value<QColor>();
      float stop = i / static_cast<float>(colorsListv.size()-1);
      colorsMap[stop] = QColorToColor(color);
    }
  }

  QString gradientScaleId = tlpStringToQString(colorScaleName) + "_gradient?";
  gradient = settings.value(gradientScaleId).toBool();
  settings.endGroup();

  settings.beginGroup("ColorScalesNoRegular");

  if (settings.contains(colorScaleName.c_str())) {
    QVariantMap colorsMapv = settings.value(colorScaleName.c_str()).toMap();

    for(QVariantMap::iterator it = colorsMapv.begin() ; it != colorsMapv.end() ; ++it) {
      colorsMap[(it.key()).toDouble()] = QColorToColor(it.value().value<QColor>());
    }

    gradient = settings.value(gradientScaleId).toBool();
  }

  return ColorScale(colorsMap, gradient);
}

void ColorScalesManager::registerColorScale(const string &colorScaleName, const ColorScale &colorScale) {
  list<string> colorScalesList = ColorScalesManager::getColorScalesList();

  if (find(colorScalesList.begin(), colorScalesList.end(), colorScaleName) != colorScalesList.end()) {
    if (colorScale.hasRegularStops()) {
      QList<QVariant> colorsVector;

      for (unsigned int i = 0; i < const_cast<ColorScale&>(colorScale).getStopsCount() ; ++i) {
        float stop = i / static_cast<float>(const_cast<ColorScale&>(colorScale).getStopsCount()-1);
        Color color = colorScale.getColorAtPos(stop);
        colorsVector.push_back(QVariant(colorToQColor(color)));
      }

      reverse(colorsVector.begin(), colorsVector.end());
      QSettings settings("TulipSoftware","Tulip");
      settings.beginGroup("ColorScales");
      settings.setValue(tlpStringToQString(colorScaleName), colorsVector);
      QString gradientId = tlpStringToQString(colorScaleName) + "_gradient?";
      settings.setValue(gradientId, colorScale.isGradient());
      settings.endGroup();
    }
    else {
      QVariantMap colorsMap;
      map<float, Color> colorsMapTlp = colorScale.getColorMap();

      for (map<float, Color>::iterator it = colorsMapTlp.begin() ; it != colorsMapTlp.end() ; ++it) {
        colorsMap[QString::number(it->first)] = colorToQColor(it->second);
      }

      QSettings settings("TulipSoftware","Tulip");
      settings.beginGroup("ColorScalesNoRegular");
      settings.setValue(tlpStringToQString(colorScaleName), colorsMap);
      QString gradientId = tlpStringToQString(colorScaleName) + "_gradient?";
      settings.setValue(gradientId, colorScale.isGradient());
      settings.endGroup();
    }
  }
}

void ColorScalesManager::removeColorScale(const string &colorScaleName) {
  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("ColorScales");
  QString savedColorScaleId = tlpStringToQString(colorScaleName);

  if (settings.contains(savedColorScaleId)) {
    settings.remove(savedColorScaleId);
    settings.remove(savedColorScaleId+"_gradient?");
  }

  settings.endGroup();
  settings.beginGroup("ColorScalesNoRegular");
  savedColorScaleId = tlpStringToQString(colorScaleName);

  if (settings.contains(savedColorScaleId)) {
    settings.remove(savedColorScaleId);
    settings.remove(savedColorScaleId+"_gradient?");
  }

  settings.endGroup();
}
