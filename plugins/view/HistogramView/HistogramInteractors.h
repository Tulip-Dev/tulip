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

#ifndef HISTOGRAMINTERACTORS_H_
#define HISTOGRAMINTERACTORS_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>

using namespace std;

namespace tlp {

class HistogramStatistics;
class HistoStatsConfigWidget;

class HistogramInteractor : public NodeLinkDiagramComponentInteractor {

public :

  HistogramInteractor(const QString &iconPath, const QString &text);

  bool isCompatible(const std::string &viewName) const;

};

class HistogramInteractorNavigation : public HistogramInteractor {

public :

  PLUGININFORMATION("HistogramInteractorNavigation", "Tulip Team", "02/04/2009", "Histogram Navigation Interactor", "1.0","Navigation")

  HistogramInteractorNavigation(const PluginContext *);

  void construct();

};

class HistogramInteractorMetricMapping : public HistogramInteractor {

public :

  PLUGININFORMATION("HistogramInteractorColorMapping", "Tulip Team", "02/04/2009", "Histogram Color Mapping Interactor", "1.0", "Information")

  HistogramInteractorMetricMapping(const PluginContext *);

  void construct();

};

class HistogramInteractorStatistics : public HistogramInteractor {

public :

  PLUGININFORMATION("HistogramInteractorStatistics", "Tulip Team", "02/04/2009", "Histogram Statistics Interactor", "1.0", "Information")

  HistogramInteractorStatistics(const PluginContext *);
  ~HistogramInteractorStatistics();

  void construct();
  QWidget* configurationWidget() const;
  void install(QObject *target);

private :

  HistoStatsConfigWidget *histoStatsConfigWidget;
  HistogramStatistics *histoStatistics;
};

}


#endif /* HISTOGRAMINTERACTORS_H_ */
