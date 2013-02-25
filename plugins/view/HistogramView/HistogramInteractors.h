/*
      HistogramInteractors.h

   Created on: 7 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef HISTOGRAMINTERACTORS_H_
#define HISTOGRAMINTERACTORS_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>

#include "HistoStatsConfigWidget.h"

using namespace std;

namespace tlp {

class HistogramInteractor : public NodeLinkDiagramComponentInteractor {

public :

    HistogramInteractor(const QString &iconPath, const QString &text);

    bool isCompatible(const std::string &viewName) const;

  unsigned int priority() const {return 0;}

};

class HistogramInteractorNavigation : public HistogramInteractor {

public :

  PLUGININFORMATIONS("HistogramInteractorNavigation", "Tulip Team", "02/04/2009", "Histogram Navigation Interactor", "1.0","Navigation")

  HistogramInteractorNavigation(const PluginContext *);

    void construct();

  unsigned int priority() const {return 5;}

};

class HistogramInteractorMetricMapping : public HistogramInteractor {

public :

  PLUGININFORMATIONS("HistogramInteractorColorMapping", "Tulip Team", "02/04/2009", "Histogram Color Mapping Interactor", "1.0", "Information")

  HistogramInteractorMetricMapping(const PluginContext *);

    void construct();

};

class HistogramInteractorStatistics : public HistogramInteractor {

public :

  PLUGININFORMATIONS("HistogramInteractorStatistics", "Tulip Team", "02/04/2009", "Histogram Statistics Interactor", "1.0", "Information")

  HistogramInteractorStatistics(const PluginContext *);
  ~HistogramInteractorStatistics();

  void construct();
  QWidget* configurationWidget() const;

private :

    HistoStatsConfigWidget *histoStatsConfigWidget;
};

}


#endif /* HISTOGRAMINTERACTORS_H_ */
