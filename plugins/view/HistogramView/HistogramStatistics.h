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

#ifndef HISTOGRAMSTATISTICS_H_
#define HISTOGRAMSTATISTICS_H_

#include <tulip/GLInteractor.h>
#include <tulip/Coord.h>
#include <map>

#include <QString>

namespace tlp {

class HistoStatsConfigWidget;
class HistogramView;

class GlAxis;
class GlQuantitativeAxis;

class KernelFunction : public std::unary_function<double, double> {

public :
  virtual ~KernelFunction() {}

  virtual double operator()(double val) = 0;

};

class HistogramStatistics : public GLInteractorComponent {

  Q_OBJECT

public :

  HistogramStatistics(HistoStatsConfigWidget *ConfigWidget);
  HistogramStatistics(const HistogramStatistics &histoStats);
  ~HistogramStatistics();

  bool eventFilter(QObject *, QEvent *);
  bool draw(GlMainWidget *glMainWidget);

  void viewChanged(View *view);

private slots :

  void computeAndDrawInteractor();

public slots:
  void computeInteractor();

private :

  void cleanupAxis();
  void initKernelFunctionsMap();

protected :

  HistogramView *histoView;
  HistoStatsConfigWidget *histoStatsConfigWidget;
  std::map<unsigned int, double> graphPropertyValueSet;
  double propertyMean;
  double propertyStandardDeviation;
  std::vector<Coord> densityEstimationCurvePoints;
  std::map<QString, KernelFunction *> kernelFunctionsMap;
  GlQuantitativeAxis *densityAxis;
  GlAxis *meanAxis, *standardDeviationPosAxis, *standardDeviationNegAxis;
  GlAxis *standardDeviation2PosAxis, *standardDeviation2NegAxis;
  GlAxis *standardDeviation3PosAxis, *standardDeviation3NegAxis;

};

}

#endif /* HISTOGRAMSTATISTICS_H_ */
