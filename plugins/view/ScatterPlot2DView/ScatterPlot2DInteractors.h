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

#ifndef SCATTERPLOT2DINTERACTORS_H_
#define SCATTERPLOT2DINTERACTORS_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>

namespace tlp {

class ScatterPlotCorrelCoeffSelectorOptionsWidget;

class ScatterPlot2DInteractor : public NodeLinkDiagramComponentInteractor {

public :

  ScatterPlot2DInteractor(const QString &iconPath, const QString &text);

  bool isCompatible(const std::string &viewName) const;

};

class ScatterPlot2DInteractorNavigation : public ScatterPlot2DInteractor {

public :

  PLUGININFORMATION("ScatterPlot2DInteractorNavigation", "Tulip Team", "02/04/2009", "Scatter Plot 2D Navigation Interactor", "1.0" ,"Navigation")

  ScatterPlot2DInteractorNavigation(const tlp::PluginContext *);

  void construct();

};

class ScatterPlot2DInteractorTrendLine : public ScatterPlot2DInteractor {

public :

  PLUGININFORMATION("ScatterPlot2DInteractorTrendLine", "Tulip Team", "02/04/2009", "Scatter Plot 2D Trend Line Interactor", "1.0", "Information")

  ScatterPlot2DInteractorTrendLine(const tlp::PluginContext *);

  void construct();

};

class ScatterPlot2DInteractorCorrelCoeffSelector : public ScatterPlot2DInteractor {

public :

  PLUGININFORMATION("ScatterPlot2DInteractorCorrelCoeffSelector", "Tulip Team", "02/04/2009", "Scatter Plot 2D Correlation Coefficient Interactor", "1.0", "Information")

  ScatterPlot2DInteractorCorrelCoeffSelector(const tlp::PluginContext *);
  ~ScatterPlot2DInteractorCorrelCoeffSelector();

  void construct();

  QWidget *configurationWidget() const;

private :

  ScatterPlotCorrelCoeffSelectorOptionsWidget *optionsWidget;

};

}

#endif /* SCATTERPLOT2DINTERACTORS_H_ */
