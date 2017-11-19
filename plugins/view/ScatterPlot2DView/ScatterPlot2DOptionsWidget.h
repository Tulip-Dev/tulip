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

#ifndef SCATTERPLOT2DOPTIONSWIDGET_H_
#define SCATTERPLOT2DOPTIONSWIDGET_H_

#include <QWidget>

#include <tulip/Color.h>
#include <tulip/Size.h>

namespace Ui {
class ScatterPlot2DOptionsWidgetData;
}

class QPushButton;

namespace tlp {

class ColorButton;

class ScatterPlot2DOptionsWidget : public QWidget {

  Q_OBJECT

public:
  ScatterPlot2DOptionsWidget(QWidget *parent = nullptr);
  ~ScatterPlot2DOptionsWidget() override;

  void setWidgetEnabled(const bool enabled);
  bool uniformBackground() const;
  Color getBackgroundColor() const;
  Color getMinusOneColor() const;
  Color getZeroColor() const;
  Color getOneColor() const;
  void setBackgroundColor(const Color &color);
  Size getMinSizeMapping() const;
  Size getMaxSizeMapping() const;
  void setMinSizeMapping(const float minSize);
  void setMaxSizeMapping(const float maxSize);

  bool useCustomXAxisScale() const;
  void useCustomXAxisScale(const bool value);

  bool useCustomYAxisScale() const;
  void useCustomYAxisScale(const bool value);

  std::pair<double, double> getXAxisScale() const;
  void setXAxisScale(const std::pair<double, double> &value);

  std::pair<double, double> getYAxisScale() const;
  void setYAxisScale(const std::pair<double, double> &value);

  void setInitXAxisScale(const std::pair<double, double> &value);
  void setInitYAxisScale(const std::pair<double, double> &value);

  void resetAxisScale();

  bool displayGraphEdges() const;
  void setDisplayGraphEdges(const bool showEdges);

  bool displayNodeLabels() const;
  void setDisplayNodeLabels(const bool showLabels);

  bool displayScaleLabels() const;
  void setDisplayScaleLabels(const bool scaleLabels);

  bool configurationChanged();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void setButtonColor(tlp::ColorButton *button, const Color &color);
  void minSizeSpinBoxValueChanged(int);
  void maxSizeSpinBoxValueChanged(int);
  void pressXScaleCheckBox(bool checked);
  void pressYScaleCheckBox(bool checked);
  void updateColorScale();

private:
  bool oldValuesInitialized;
  bool oldUniformBackground;
  Color oldUniformBackgroundColor;
  Color oldMinusOneColor;
  Color oldZeroColor;
  Color oldOneColor;
  Size oldMinSizeMapping;
  Size oldMaxSizeMapping;
  bool oldDisplayGraphEdges;
  bool oldDisplayNodeLabels;
  bool oldlabelscaled;
  bool oldUseCustomXAxisScale;
  bool oldUseCustomYAxisScale;
  std::pair<double, double> oldXAxisScale, oldYAxisScale; //<min, max>
  std::pair<double, double> initXAxisScale, initYAxisScale;
  Ui::ScatterPlot2DOptionsWidgetData *_ui;
};
}

#endif /* SCATTERPLOT2DOPTIONSWIDGET_H_ */
