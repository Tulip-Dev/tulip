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

/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 14/05/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_MAINCONTROLLER_H
#define Tulip_MAINCONTROLLER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/Reflect.h>
#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>

#include "tulip/ControllerViewsManager.h"

class QAction;
class QMenu;
class QTabWidget;

namespace tlp {

  class Graph;
  class View;
  class SGHierarchyWidget;
  class PropertyDialog;
  class ElementPropertiesWidget;
  class Morphing;
  class GraphState;

  /** \brief Default controller of Tulip
   *
   * This Controller create a GUI like the old Tulip
   * On left : Property, and hierarchical view
   * In view menu you have access to all view plugins
   */
  class TLP_QT_SCOPE MainController :  public ControllerViewsManager, public Observable, public GraphObserver {

    Q_OBJECT;

  public:
    /**
     * Default constructor
     */
    MainController();
    /**
     * Default destructor
     */
    virtual ~MainController();

    /**
     * Attach the QMainWindow to the controller
     */
    void attachMainWindow(MainWindowFacade facade);

    /**
     * Set graph and dataSet to the controller
     */
    void setData(Graph *graph=0,DataSet dataSet=DataSet());
    /**
     * Return the graph and the dataSet of the controller
     */
    void getData(Graph **graph,DataSet *dataSet);
    

  protected :
    /**
     * Set the default initialization of the graph
     */
    void initializeGraph(Graph *graph);
    /**
     * Load the GUI
     */
    void loadGUI();
    /**
     * Construct main menu
     */
    void buildMenu();
    /**
     * Create and init the main view (NodeLinkDiagramComponent)
     */
    View* initMainView(DataSet dataSet);
    /**
     * Create a view with name : name
     */
    virtual View* createView(const std::string &name,Graph *graph,DataSet dataSet,bool forceWidgetSize=true,const QRect &rect=QRect(0,0,0,0),bool maximized=false);

    typedef std::set< tlp::Observable * >::iterator ObserverIterator;

    /**
     * Draw/Init all views
     */
    virtual void drawViews(bool init=false);
    /**
     * This function is call when an observable is destroyed
     */
    void observableDestroyed(Observable *);
    /**
     * This function is call when an observable is update
     */
    void update ( ObserverIterator begin, ObserverIterator end);
    /**
     * Init all observers (graph and properties)
     */
    void initObservers();
    /**
     * Clear all observers
     */
    void clearObservers();
    /**
     * Call when a subgraph is add
     */
    void addSubGraph(Graph*, Graph *);
    /**
     * Call when a subgraph is remove
     */
    void delSubGraph(Graph*, Graph*);
    /**
     * Call when a local property is add
     */
    void addLocalProperty(Graph*, const std::string&);
    /**
     * Call when a local property is delete
     */
    void delLocalProperty(Graph*, const std::string&);
    /**
     * Call when an attribute is set on a graph
     */
    void afterSetAttribute(Graph*, const std::string&);
    /**
     * Call to update number of nodes/edges
     */
    void updateCurrentGraphInfos();
    
    /**
     * Activate undo/redo button, reload propertyWidget and redrawViews after we have change a property
     */
    void afterChangeProperty();
    
    /**
     * Store current view in a GraphState to use it with applyMorphing function
     */
    GraphState *constructGraphState();
    
    /**
     * Do a morphing between current state and the given GraphState
     */
    void applyMorphing(GraphState *graphState);

    std::map<View *, int> lastConfigTabIndexOnView;
    unsigned int currentGraphNbNodes;
    unsigned int currentGraphNbEdges;
    Graph *graphToReload;
    bool blockUpdate;
    bool inAlgorithm;

    Morphing *morph;

    QDockWidget *tabWidgetDock;
    QDockWidget *configWidgetDock;
    QTabWidget *configWidgetTab;
    SGHierarchyWidget *clusterTreeWidget;
    PropertyDialog *propertiesWidget;
    ElementPropertiesWidget *eltProperties;

    QMenu *editMenu;
    QMenu *algorithmMenu;
    QMenu *viewMenu;
    QMenu *optionsMenu;
    QMenu *graphMenu;

    QAction *forceRatioAction;
    QAction *mapMetricAction;
    QAction *morphingAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *snapshotAction;
    QAction *editUndoAction;
    QAction *editRedoAction;

    QMenu *intMenu;
    QMenu *stringMenu;
    QMenu *sizesMenu;
    QMenu *colorsMenu;
    QMenu *layoutMenu;
    QMenu *metricMenu;
    QMenu *selectMenu;
    QMenu *generalMenu;

  protected slots:
    /**
     * Change the graph and load it in left part of the GUI
     */
    virtual bool changeGraph(Graph *graph);
    /**
     * Clear observers when a graph is about to be remove
     */
    void graphAboutToBeRemove(Graph *graph);
    /**
     * This function is call when a view is activated
     * Return true if view can be activated (ie this view exist)
     */
    virtual bool windowActivated(QWidget *w);
    /**
     * Load the interactor (referenced by the given QAction) in current View
    */
    virtual bool changeInteractor(QAction* action);
    /**
     * This slot is call when a view emit elementSelected
     * Show the element in left part of the GUI
     */
    void showElementProperties(unsigned int eltId, bool isNode);
    /**
     * This slot is call to ensure the Graph Editor is visible
     */
    void showGraphEditor();
    /**
     * This slot is call to ensure the View Editor is visible
     */
    void showViewEditor();
    /**
     * This slot is call when a view want to change its graph
     */
    void viewRequestChangeGraph(tlp::View *view,tlp::Graph *graph);
    /**
     * This slot is call went a view will be closed
     */
    virtual void widgetWillBeClosed(QObject *object);

    void isAcyclic();
    void isSimple();
    void isConnected();
    void isBiconnected();
    void isTriconnected();
    void isTree();
    void isFreeTree();
    void isPlanar();
    void isOuterPlanar();
    void makeAcyclic();
    void makeBiconnected();
    void makeSimple();
    void makeConnected();
    void makeDirected();
    void reverseSelectedEdgeDirection();

    void changeMetric();
    void changeString();
    void changeSizes();
    void changeColors();
    void changeInt();
    void changeSelection();
    void changeLayout();
    void applyAlgorithm();

    void updateUndoRedoInfos();
    void undo();
    void redo();
    void snapshot();

  public slots :
      void editCut();
      void editCopy();
      void editPaste();
      void editFind();
      void editCreateGroup();
      void editCreateSubgraph();
      void editDelSelection();
      void editReverseSelection();
      void editSelectAll();
      void editDeselectAll();

  };

}

#endif
