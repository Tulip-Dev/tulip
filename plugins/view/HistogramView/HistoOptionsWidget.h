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

#ifndef HISTOOPTIONSWIDGET_H_
#define HISTOOPTIONSWIDGET_H_

#include <QWidget>

#include <tulip/Color.h>

namespace Ui {
class HistoOptionsWidgetData;
}

namespace tlp {

class HistoOptionsWidget : public QWidget {

  Q_OBJECT

public:
  HistoOptionsWidget(QWidget *parent = 0);
  ~HistoOptionsWidget();

  void setWidgetEnabled(const bool enabled);

  void setNbOfHistogramBins(const unsigned int nbOfHistogramBins);
  unsigned int getNbOfHistogramBins();

  void setNbXGraduations(const unsigned int nbXGraduations);
  unsigned int getNbXGraduations();

  void setYAxisIncrementStep(const unsigned int yAxisIncrementStep);
  unsigned int getYAxisIncrementStep();

  void setCumulativeFrequenciesHistogram(const bool cumulHisto);
  bool cumulativeFrequenciesHisto();

  void setUniformQuantification(const bool uniformQuantification);
  bool uniformQuantification();

  void setXAxisLogScale(const bool xAxisLogScale);
  bool xAxisLogScaleSet() const;

  void setYAxisLogScale(const bool yAxisLogScale);
  bool yAxisLogScaleSet() const;

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

  void setBinWidth(const double binWidth);

  Color getBackgroundColor() const;
  void setBackgroundColor(const Color &color);

  bool showGraphEdges() const;
  void setShowGraphEdges(const bool showGraphEdges);

  void enableShowGraphEdgesCB(const bool enable);

  bool configurationChanged();

private slots:

  void enableOrDisableNbXGraduationsSP(int uniQuantState);
  void pressXScaleCheckBox(bool checked);
  void pressYScaleCheckBox(bool checked);

private:
  bool oldValueInitialized;
  unsigned int oldNbOfHistogramBins;
  unsigned int oldNbXGraduations;
  unsigned int oldYAxisIncrementStep;
  bool oldCumulativeFrequenciesHistogram;
  bool oldUniformQuantification;
  bool oldXAxisLogScale;
  bool oldYAxisLogScale;
  Color oldBackgroundColor;
  bool oldShowGraphEdges;
  bool oldUseCustomXAxisScale;
  bool oldUseCustomYAxisScale;
  std::pair<double, double> oldXAxisScale, oldYAxisScale; //<min, max>
  std::pair<double, double> initXAxisScale, initYAxisScale;
  Ui::HistoOptionsWidgetData *_ui;
};
}

#endif /* HISTOOPTIONSWIDGET_H_ */
