//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 04/09

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef PARALLELCOORDINATESINTERACTORS_H_
#define PARALLELCOORDINATESINTERACTORS_H_

#include <tulip/InteractorChainOfResponsibility.h>

namespace tlp {

  /** \brief Interactor abstract class for ParallelCoordinates
   *
   */
  class ParallelCoordinatesInteractor  : public InteractorChainOfResponsibility {

  public :

    ParallelCoordinatesInteractor(const QString &iconPath, const QString &text);
    /**
     * return if this interactor is compatible with given View
     */
    bool isCompatible(const std::string &viewName);

  };

  /** \brief Parallel coordinates interactor to select elements
    *
    */
  class InteractorParallelCoordsSelection : public ParallelCoordinatesInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorParallelCoordsSelection();

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

    /**
     * Default constructor
     */
    InteractorHighLighter();

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

    /**
     * Default constructor
     */
    InteractorAxisSwapper();

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

    /**
     * Default constructor
     */
    InteractorAxisSliders();

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

    /**
     * Default constructor
     */
    InteractorBoxPlot();

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

	  /**
	   * Default constructor
	   */
	  InteractorShowElementInfos();

	  /**
	   * Construct chain of responsibility
	   */
	  void construct();

  };

}

#endif /* PARALLELCOORDINATESINTERACTORS_H_ */

