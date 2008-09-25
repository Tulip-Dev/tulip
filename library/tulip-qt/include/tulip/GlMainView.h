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

#ifndef Tulip_GLMAINVIEW_H
#define Tulip_GLMAINVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/View.h"

#include <QtGui/QMainWindow>

#include <QtGui/QDockWidget>

#include "tulip/GlMainWidget.h"

class QFrame;

namespace tlp {

  class GWOverviewWidget;
  class PropertyDialog;
  class SGHierarchyWidget;
  class ElementPropertiesWidget;
  class RenderingParametersDialog;
  class LayerManagerWidget;
#ifdef STATS_UI
  class TulipStats;
#endif

  class TLP_QT_SIMPLE_SCOPE GlMainViewCreator : public ViewCreator {

  public :
  
    View* create(const std::string &name,QWidget *parent);
  };

  class TLP_QT_SIMPLE_SCOPE GlMainView : public View, public GlSceneObserver {

    Q_OBJECT;
  
  public:
    GlMainView(const std::string &pluginName,QWidget *parent=NULL, const char *name=NULL);
    ~GlMainView();

    void setData(Graph *graph,DataSet dataSet);
    DataSet getData();
    Graph *getGraph();
    GlMainWidget *getGlMainWidget();

    void constructInteractorsMap();
    void getInteractorsActionList(std::list<QAction*> &);
    Iterator<Interactor *> *installInteractor(const std::string &);

    void addNode(Graph *,const node );
    void addEdge(Graph *,const edge );
    void delNode(Graph *,const node );
    void delEdge(Graph *,const edge );

    //bool eventFilter(QObject *, QEvent *);

    bool cutIsEnable() {return true;}
    bool copyIsEnable() {return true;}
    bool pasteIsEnable() {return true;}
    bool findIsEnable() {return true;}
    bool selectAllIsEnable() {return true;}
    bool delSelectionIsEnable() {return true;}
    bool deselectAllIsEnable() {return true;}
    bool invertSelectionIsEnable() {return true;}
    bool createGroupIsEnable() {return true;}
    bool createSubgraphIsEnable() {return true;}

    bool doProgressUpdate() {return true;}
    void progressUpdate();

    void specificEventFilter(QObject *object,QEvent *event);
    void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu);
    void computeContextMenuAction(QAction *action);

  protected :

    std::string newName();
    // GlSceneObserver interface
    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);

    GlMainWidget *mainWidget;

    QDockWidget *overviewDock;
    GWOverviewWidget *overviewWidget;
    QFrame *overviewFrame;
    QMenu *viewMenu;
    QMenu *dialogMenu;
    QMenu *optionsMenu;
    QAction *actionTooltips;

    QAction *renderingParametersDialogAction;
    RenderingParametersDialog *renderingParametersDialog;
    LayerManagerWidget *layerWidget;

    tlp::Graph * copyCutPasteGraph;

    QAction* overviewAction;
    QAction* selectAction;
    QAction* deleteAction;
    QAction* goAction;	
    QAction* ungroupAction;
    QAction* propAction;
    bool isNode;
    int itemId;
			 

  protected slots:
    void centerView();
    void showDialog(QAction*);

  public slots:
    void redrawView();
    void hideOverview(bool);
    void cut();
    void copy();
    void paste();
    void find();
    void selectAll();
    void deselectAll();
    void invertSelection();
    void delSelection();
    void createSubgraph();
    void reverseSelectedEdgeDirection();
    void createGroup();  
    void changeGraph(Graph *);

    /*signals:
      void showElementPropertiesSignal(unsigned int eltId, bool isNode);*/

  };

}

#endif
