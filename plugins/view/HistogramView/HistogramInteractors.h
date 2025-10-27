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

#ifndef HISTOGRAMINTERACTORS_H_
#define HISTOGRAMINTERACTORS_H_

#include <tulip/InteractorViewExplorer.h>
#include "../../utils/PluginNames.h"

namespace tlp {

class HistogramStatistics;
class HistoStatsConfigWidget;

class HistogramInteractor : public NodeLinkDiagramComponentInteractor {

public:
  HistogramInteractor(const QString &iconPath, const QString &text,
                      const unsigned int priority = 0);

  bool isCompatible(const std::string &viewName) const override;
};

class HistogramInteractorMetricMapping : public HistogramInteractor {

public:
  PLUGININFORMATION("HistogramInteractorColorMapping", "Tulip Team", "02/04/2009",
                    "Histogram Color Mapping Interactor", "1.0", "Information")

  HistogramInteractorMetricMapping(const PluginContext *);

  void construct() override;
};

class HistogramInteractorStatistics : public HistogramInteractor {

public:
  PLUGININFORMATION("HistogramInteractorStatistics", "Tulip Team", "02/04/2009",
                    "Histogram Statistics Interactor", "1.0", "Information")

  HistogramInteractorStatistics(const PluginContext *);
  ~HistogramInteractorStatistics() override;

  void construct() override;
  QWidget *configurationOptionsWidget() const override;
  void install(QObject *target) override;

private:
  HistoStatsConfigWidget *histoStatsConfigWidget;
  HistogramStatistics *histoStatistics;
};

/**
 *  \brief interactor to get information about an element of the graph
 */
class HistogramInteractorGetInformation : public InteractorViewExplorer {
public:
  PLUGININFORMATION("HistogramInteractorGetInformation", "Tulip Team", "18/06/2015",
                    "Explore current view", "2.0", "Information")
  /**
   * Default constructor
   */
  HistogramInteractorGetInformation(const tlp::PluginContext *);

  bool isCompatible(const std::string &viewName) const override;
};
} // namespace tlp

#endif /* HISTOGRAMINTERACTORS_H_ */
