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
    virtual bool isCompatible(const std::string &viewName) {
      if(viewName=="Parallel Coordinates view")
        return true;
      return false;
    }

  };
  /*
    interactorsMap["Navigate in graph"].push_back(InteractorManager::getInst().getInteractor("MouseNKeysNavigator"));
    interactorsMap["Zoom on rectangle"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
    interactorsMap["Zoom on rectangle"].push_back(InteractorManager::getInst().getInteractor("MouseBoxZoomer"));
    interactorsMap["Get information on nodes/edges"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementShowInfos"));
    interactorsMap["Get information on nodes/edges"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
    interactorsMap["Select nodes/edges in a rectangle"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementsSelector"));
    interactorsMap["Select nodes/edges in a rectangle"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
    interactorsMap["Delete nodes or edges"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementDeleter"));
    interactorsMap["Delete nodes or edges"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));

    interactorsMap["Highlight elements"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsElementHighlighter"));
    interactorsMap["Highlight elements"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
    interactorsMap["Axis Swapper"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsAxisSwapper"));
    interactorsMap["Axis Swapper"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
    interactorsMap["Axis Sliders"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsAxisSliders"));
    interactorsMap["Axis Sliders"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
    interactorsMap["Axis Box Plot"].push_back(InteractorManager::getInst().getInteractor("ParallelCoordsAxisBoxPlot"));
    interactorsMap["Axis Box Plot"].push_back(InteractorManager::getInst().getInteractor("MousePanNZoomNavigator"));
  }
  //==================================================
  void ParallelCoordinatesView::constructInteractorsActionList() {
    interactorsActionList.push_back(new QAction(QIcon(":/i_navigation.png"), "Navigate in graph", this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_zoom.png"), "Zoom on rectangle", this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_select.png"), "Get information on nodes/edges", this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_selection.png"), "Select nodes/edges in a rectangle", this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_del.png"), "Delete nodes or edges", this));

    interactorsActionList.push_back(new QAction(QIcon(":/i_element_highlighter.png"), "Highlight elements",this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_axis_swapper.png"),"Axis Swapper",this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_axis_sliders.png"),"Axis Sliders",this));
    interactorsActionList.push_back(new QAction(QIcon(":/i_axis_boxplot.png"),"Axis Box Plot",this));
  }
   */

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
  class InteractorHighLiter  : public ParallelCoordinatesInteractor {

  public:

    /**
     * Default constructor
     */
    InteractorHighLiter();

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

}

#endif /* PARALLELCOORDINATESINTERACTORS_H_ */

