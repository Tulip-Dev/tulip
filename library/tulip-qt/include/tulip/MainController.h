//-*-c++-*-

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

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/Reflect.h>
#include <tulip/Observable.h>
#include <tulip/Graph.h>

#include "tulip/Controller.h"

class QAction;
class QMenu;

namespace tlp {

  class Graph;
  class View;
  class SGHierarchyWidget;
  class PropertyDialog;
  class ElementPropertiesWidget;
#ifdef STATS_UI
  class TulipStat;
#endif
  class Morphing;

  /** \brief Default controller of Tulip
   *
   * This Controller create a GUI like the old Tulip
   * On left : Property, and hierarchical view
   * In view menu you have access to all view plugins
   */
  class TLP_QT_SCOPE MainController : public Controller, public Observer, public GraphObserver {

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
    /**
     * Return the Graph visualized by the controller
     */
    Graph *getGraph();

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
    View* createView(const std::string &name,Graph *graph,DataSet dataSet,const QRect &rect=QRect(0,0,0,0));
    /**
     * Put interactors of the given view in graphToolBar
     */
    void installInteractors(View *view);

    typedef std::set< tlp::Observable * >::iterator ObserverIterator;

    /**
     * Redraw all views
     */
    void redrawViews(bool init=false);
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
     * Change a property
     */
    template<typename PROPERTY> bool changeProperty(std::string, std::string, bool = true, bool = false , bool = true);


    Graph *currentGraph;
    View *currentView;
    Graph * copyCutPasteGraph;
    std::map<View *,std::string> viewNames;
    std::map<QWidget *,View*> viewWidget;
    std::map<View *, QAction *> lastInteractorOnView;
    std::map<View *,Graph* > viewGraph;
    QWidget *lastWidget;

    Morphing *morph;

    QDockWidget *tabWidgetDock;
    SGHierarchyWidget *clusterTreeWidget;
    PropertyDialog *propertiesWidget;
    ElementPropertiesWidget *eltProperties;
#ifdef STATS_UI
    TulipStat *statsWidget;
#endif

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

    QMenu *intMenu;
    QMenu *stringMenu;
    QMenu *sizesMenu;
    QMenu *colorsMenu;
    QMenu *layoutMenu;
    QMenu *metricMenu;
    QMenu *selectMenu;
    QMenu *generalMenu;

  protected slots:
    /*
     * Change the graph and load it in left part of the GUI
     */
    void changeGraph(Graph *graph);
    /*
     * Clear observers when a graph is about to be remove
     */
    void graphAboutToBeRemove(Graph *graph);
    /**
     * Change GUI when a QWidget is activated
     */
    void windowActivated(QWidget *w);
    /**
     * Load the interactor (referenced by the given QAction) in current View
     */
    void changeInteractor(QAction* action);
    /**
     * Add a new view to the controller
     */
    void addView(QAction *action);
    /**
     * This slot is call when a view emit elementSelected
     * Show the element in left part of the GUI
     */
    void showElementProperties(unsigned int eltId, bool isNode);

    /**
     * This slot is call went a view will be closed
     */
    void widgetWillBeClosed(QObject *object);

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

    void changeMetric(QAction*);
    void changeString(QAction*);
    void changeSizes(QAction*);
    void changeColors(QAction*);
    void changeInt(QAction*);
    void changeSelection(QAction*);
    void changeLayout(QAction*);
    void applyAlgorithm(QAction*);

    void updateUndoRedoInfos();
    void undo();
    void redo();

  public :

    View *getView(QWidget *);

  };

}

#endif
