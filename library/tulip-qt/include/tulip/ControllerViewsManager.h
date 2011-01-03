/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef Tulip_CONTROLLERVIEWSMANAGER_H
#define Tulip_CONTROLLERVIEWSMANAGER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <map>
#include <string>

#include "tulip/Controller.h"

class QWidget;
class QAction;

namespace tlp {

  class Graph;
  class View;
  
  /** \brief Views manager for Controller 
   *
   * This class manage views for Controllers 
   */
  class TLP_QT_SCOPE ControllerViewsManager : public Controller {

    Q_OBJECT;
    
    public :
      
      ControllerViewsManager();
      virtual ~ControllerViewsManager() {}
      
      /**
       * Attach the QMainWindow to the controller
       */
      void attachMainWindow(MainWindowFacade facade);
      
      /**
       * Return the Graph visualized by the controller
       * Equivalent to getCurrentGraph()
       */
      virtual Graph *getGraph();
      
      /**
       * Return the Graph visualized by the current view in the controller
       * Equivalent to getGraph()
       */
      virtual Graph *getCurrentGraph();
      
      /**
       * Set the graph visualized by the current view in the controller
       */
      virtual void setCurrentGraph(Graph *graph);
      
      /**
       * Return the current view in the controller
       */
      virtual View *getCurrentView();
      
      /**
       * Return the number of opened view
       */
      virtual unsigned int getViewsNumber();
      
      /**
       * Get the graph associated with the given view
       */
      virtual Graph *getGraphOfView(View *view);
      
      /**
       * return the views for the given graph.
       */
      virtual std::vector<View*> getViewsOfGraph(Graph *graph);

      /**
       * Set the graph associated with the given view
       */
      virtual void setGraphOfView(View *view,Graph *graph);
      /**
        * Set the data of the given view.
        */
       virtual void setDataOfView(tlp::View* view, tlp::Graph* graph, const tlp::DataSet& dataSet);
      /**
       * Get the view associated with the given widget
       */
      virtual View *getViewOfWidget(QWidget *widget);
      
      /**
       * Set the view associated with the given widget
       */
      virtual void setViewOfWidget(QWidget *widget,View *view);
      
      /**
       * Get views
       */
      virtual void getViews(std::vector<View*> &views);

      /**
       * Get the widget associated with the given view
       */
      virtual QWidget *getWidgetOfView(View *view);
      
      /**
       * Get the name of the given view
       */
      virtual std::string getNameOfView(View *view);
      
      /**
       * Set the name of the given view
       */
      virtual void setNameOfView(View *view,const std::string &name);

      /**
       * Get the configuration widget of interactor activated for this view
       */
      virtual QWidget *getInteractorConfigurationWidgetOfView(View *view);

      /**
       * \brief Change the title of views of graph : graph
       * Through all opened views and change window title of view that visualize the graph
       * Window title is : view name + visualized graph name
       */
      virtual void changeWindowTitle(Graph *graph);
      
      /**
       * Close all the views.
       */
      virtual void closeAllViews();

      /**
       * Close the given view.
       */
      virtual void closeView(View *view);

      /**
       * Close all the views for the given graph.
       */
      virtual void closeViewsRelatedToGraph(Graph* graph);

      //*************************************************************************
      
      /**
       * Create a view with given name/graph/dataSet, set position and size of this view at rect and maximize it if maximized is at true
       * if forceWidgetSize is false : use rect size only if created widget have a size smaller than (10,10)
       * Return new created view
       */
      virtual View* createView(const std::string &name,Graph *graph,DataSet dataSet,bool forceWidgetSize,const QRect &rect,bool maximized);

      /**
       * Initialize an already created view and set it into the workspace with the specified position, size, etc
       * See ControllerViewsManager::createView() for details.
       */
      void addView(View *view, Graph *graph, DataSet dataSet, bool forceWidgetSize, const QRect &rect, bool maximized, const std::string &viewName, QWidget *viewWidget);
      
      /**
       * Put interactors of the given view in graphToolBar
       */
      virtual void installInteractors(View *view);
      
      /**
       * Update view that use given graph (call setGraph on these views)
       */
      virtual void updateViewsOfGraph(Graph *graph);
      
      /**
       * Update view that use sub graph of given graph (call setGraph on these views)
       */
      virtual void updateViewsOfSubGraphs(Graph *graph);
      
      /**
       * Change graph of view that use oldGraph (call setGraph on these views)
       */
      virtual void changeGraphOfViews(Graph *oldGraph,Graph *newGraph);
      
      /**
       * Call draw/init on views, if call init() if init==true and draw() else 
       */
      virtual void drawViews(bool init=false);
      
      /**
       * Store hierarchy of graphs for all views 
       */
      virtual void saveViewsGraphsHierarchies();
      
      /**
       * Check hierarchy of graphs for all views
       */
      virtual void checkViewsGraphsHierarchy();
      
    protected slots:
   
      /**
       * Create an empty view with action->text() name and currentGraph 
       */
      virtual void createView(QAction *action);
      /**
       * This function is call when a view is activated
       * Return true if view can be activated (ie this view exist)
       */
      virtual bool windowActivated(QWidget *widget);
      /**
       * Load the interactor (in this function we catch the QAction who send this signal) in current View
       */
      virtual bool changeInteractor();
      /**
       * Load the interactor (referenced by the given QAction) in current View
       */
      virtual bool changeInteractor(QAction* action);
      /**
       * Load the interactor (referenced by the given QAction) in current View
       * ConfigurationWidget of this interactor is store in configurationWidget;
       */
      virtual bool changeInteractor(QAction* action,QWidget **configurationWidget);
      /**
       * Change the graph on the currentView 
       */
      virtual bool changeGraph(Graph *graph);
      /**
       * This slot is call went a view will be closed
       */
      virtual void widgetWillBeClosed(QObject *object);

    protected:
      View *currentView;
      std::map<View *, QAction *> lastInteractorOnView;
    private :
      
      Graph *currentGraph;
      
      std::map<View *,std::string> viewNames;
      std::map<QWidget *,View*> viewWidget;
      std::map<View *,Graph* > viewGraph;
      
      std::map<View *, QWidget *> lastInteractorConfigurationWidgetOnView;
      std::map<View *,std::list<unsigned int> > viewsGraphsHierarchy;
      
  };

}

#endif
