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

#include "tulip/AbstractView.h"

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
  class GridOptionsWidget;

  class TLP_QT_SIMPLE_SCOPE GlMainView : public AbstractView, public GlSceneObserver {

    Q_OBJECT;
  
  public:
    GlMainView(const std::string &pluginName,QWidget *parent=NULL, const char *name=NULL);
    ~GlMainView();

    void setData(Graph *graph,DataSet dataSet);
    void getData(Graph **graph,DataSet *dataSet);
    Graph *getGraph();
    GlMainWidget *getGlMainWidget();

    void constructInteractorsMap();
    void getInteractorsActionList(std::list<QAction*> &);
    void installInteractor(QAction *);

    void specificEventFilter(QObject *object,QEvent *event);
    void buildContextMenu(QObject *object,QMouseEvent *event,QMenu *contextMenu);
    void computeContextMenuAction(QAction *action);

  protected :

    // GlSceneObserver interface
    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);

    GlMainWidget *mainWidget;
    GridOptionsWidget *gridOptionsWidget;

    QDockWidget *overviewDock;
    GWOverviewWidget *overviewWidget;
    QFrame *overviewFrame;
    QMenu *viewMenu;
    QMenu *dialogMenu;
    QMenu *optionsMenu;
    QMenu *exportImageMenu;
    QAction *actionTooltips;
    QAction *actionsGridOptions;

    QAction *renderingParametersDialogAction;
    RenderingParametersDialog *renderingParametersDialog;
    LayerManagerWidget *layerWidget;

    QAction* overviewAction;
    QAction* selectAction;
    QAction* deleteAction;
    QAction* goAction;	
    QAction* ungroupAction;
    QAction* propAction;
    bool isNode;
    int itemId;
			 

  protected slots:
    void exportImage(QAction*);
    void centerView();
    void showDialog(QAction*);
    void gridOptions();
    void hideOverview(bool);	     

  public slots:
    void draw();
    void refresh();
    void init();

    void setGraph(Graph *graph);

    /*signals:
      void showElementPropertiesSignal(unsigned int eltId, bool isNode);*/

  };

}

#endif
