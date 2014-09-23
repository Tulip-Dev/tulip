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

#ifndef PARALLELCOORDINATESINTERACTORS_H_
#define PARALLELCOORDINATESINTERACTORS_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>

namespace tlp {

/** \brief Interactor abstract class for ParallelCoordinates
 *
 */
class ParallelCoordinatesInteractor  : public NodeLinkDiagramComponentInteractor {

public :

  ParallelCoordinatesInteractor(const QString &iconPath, const QString &text);
  /**
   * return if this interactor is compatible with given View
   */
  bool isCompatible(const std::string &viewName) const;

};

/** \brief Parallel coordinates interactor to select elements
  *
  */
class InteractorParallelCoordsSelection : public ParallelCoordinatesInteractor {

public:

  PLUGININFORMATION("InteractorParallelCoordsSelection", "Tulip Team", "02/04/2009", "Selection Interactor", "1.0", "Selection")

  /**
   * Default constructor
   */
  InteractorParallelCoordsSelection(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

/** \brief Parallel coordinates interactor to highlite elements
 *
 */
class InteractorHighLighter  : public ParallelCoordinatesInteractor {

public:

  PLUGININFORMATION("InteractorHighLiter", "Tulip Team", "02/04/2009", "Element Highliter Interactor", "1.0", "Information")

  /**
   * Default constructor
   */
  InteractorHighLighter(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

/** \brief Parallel coordinates interactor to swap axis
 *
 */
class InteractorAxisSwapper  : public ParallelCoordinatesInteractor {

public:

  PLUGININFORMATION("InteractorAxisSwapper", "Tulip Team", "02/04/2009", "Axis Swap Interactor", "1.0", "Modification")

  /**
   * Default constructor
   */
  InteractorAxisSwapper(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

/** \brief Parallel coordinates interactor to have sliders on axis
 *
 */
class InteractorAxisSliders : public ParallelCoordinatesInteractor {

public:

  PLUGININFORMATION("InteractorAxisSliders", "Tulip Team", "02/04/2009", "Axis Sliders Interactor", "1.0" ,"Modification")

  /**
   * Default constructor
   */
  InteractorAxisSliders(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

/** \brief Parallel coordinates interactor to have boxplot
 *
 */
class InteractorBoxPlot  : public ParallelCoordinatesInteractor {

public:

  PLUGININFORMATION("InteractorBoxPlot", "Tulip Team", "02/04/2009", "Box Plot Interactor", "1.0", "Information")

  /**
   * Default constructor
   */
  InteractorBoxPlot(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

/** \brief Parallel coordinates interactor to show element properties when clicking on it
   *
   */
class InteractorShowElementInfos : public ParallelCoordinatesInteractor {

public :

  PLUGININFORMATION( "InteractorShowElementInfos", "Tulip Team", "02/04/2009", "Show Element Infos Interactor", "1.0", "Information")

  /**
   * Default constructor
   */
  InteractorShowElementInfos(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

/** \brief Parallel coordinates interactor to modify space between two consecutive axis by using the mouse wheel
   *
   */
class InteractorAxisSpacer : public ParallelCoordinatesInteractor {

public :

  PLUGININFORMATION("InteractorAxisSpacer", "Tulip Team", "02/04/2009", "Axis Spacer Interactor", "1.0", "Modification")

  /**
   * Default constructor
   */
  InteractorAxisSpacer(const tlp::PluginContext *);

  /**
   * Construct chain of responsibility
   */
  void construct();

};

}

#endif /* PARALLELCOORDINATESINTERACTORS_H_ */

