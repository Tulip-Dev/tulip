/*
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
///@cond DOXYGEN_HIDDEN

#ifndef COLORSCALECONFIGDIALOG_H_
#define COLORSCALECONFIGDIALOG_H_

#include <tulip/ColorScale.h>

#include <QDialog>

namespace Ui {
class ColorScaleDialog;
}

class QTableWidgetItem;
class QListWidgetItem;
class QLabel;

namespace tlp {

class TLP_QT_SCOPE ColorScaleConfigDialog : public QDialog {

  Q_OBJECT

public:
  ColorScaleConfigDialog(const ColorScale &colorScale = ColorScale(), QWidget *parent = nullptr);
  ~ColorScaleConfigDialog() override;
  void setColorScale(const ColorScale &colorScale);
  const ColorScale &getColorScale() const;
  static ColorScale getColorScaleFromImageFile(const std::string &imageFilePath,
                                               bool gradient = true);

protected:
  void resizeEvent(QResizeEvent *event) override;
  void showEvent(QShowEvent *event) override;

private slots:

  void accept() override;
  void pressButtonBrowse();
  void nbColorsValueChanged(int value);
  void colorTableItemDoubleClicked(QTableWidgetItem *item);
  void displaySavedGradientPreview();
  void displayUserGradientPreview();
  void saveCurrentColorScale();
  void deleteSavedColorScale();
  void reeditSaveColorScale(QListWidgetItem *savedColorScaleItem);
  void importColorScaleFromImageFile();
  void importColorScaleFromColorScaleFile();
  void invertEditedColorScale();
  void applyGlobalAlphaToColorScale();

private:
  Ui::ColorScaleDialog *_ui;

  void setColorScaleFromImage(const QString &imageFilePath);

  void loadUserSavedColorScales();
  void displayGradientPreview(const QList<QColor> &colorsVector, bool gradient,
                              QLabel *displayLabel);
  void importColorScaleFromFile(const QString &currentDir);

  ColorScale colorScale;
  ColorScale latestColorScale;
  std::string gradientsImageDirectory;

  static std::map<QString, std::vector<Color>> tulipImageColorScales;
  static void loadTulipImageColorScales();
  static void loadImageColorScalesFromDir(const QString &colorScalesDir);
  static std::vector<Color> getColorScaleFromImageFile(const QString &imageFilePath);
};
} // namespace tlp

#endif /* COLORSCALECONFIGDIALOG_H_ */
///@endcond
