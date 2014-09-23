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

#ifndef HISTOSTATSCONFIGWIDGET_H_
#define HISTOSTATSCONFIGWIDGET_H_

#include <QWidget>

namespace Ui {
class HistoStatsConfigWidget;
}

namespace tlp {

class HistoStatsConfigWidget : public QWidget  {

  Q_OBJECT

  Ui::HistoStatsConfigWidget *_ui;

public :

  HistoStatsConfigWidget(QWidget *parent = 0);
  ~HistoStatsConfigWidget();

  void setMinMaxMeanAndSd(double min, double max, double mean, double standardDeviation);
  bool densityEstimation() const;
  double getSampleStep() const;
  double getBandwidth() const;
  QString getKernelFunctionName() const;
  bool displayMeanAndStandardDeviation() const;
  bool nodesSelection() const;
  double getSelectionLowerBound() const;
  double getSelectionUpperBound() const;

private :

  double getBoundFromString(const QString &bound) const;

  double min, max;
  double mean, standardDeviation;

signals:
  void computeAndDrawInteractor();

};


}

#endif /* HISTOSTATSCONFIGWIDGET_H_ */
