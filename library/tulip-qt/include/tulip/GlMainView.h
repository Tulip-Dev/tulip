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
#include "tulip/GlMainViewData.h"

#include <QtGui/QMainWindow>

#include <QtGui/QDockWidget>

#include "tulip/GlMainWidget.h"

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

  class TLP_QT_SIMPLE_SCOPE GlMainView : public View, public Ui::GlMainViewData, public GlSceneObserver {

    Q_OBJECT;
  
  public:
    GlMainView(const std::string &pluginName,QWidget *parent=NULL, const char *name=NULL);
    ~GlMainView();

    void setData(Graph *graph,DataSet dataSet);
    DataSet getData();
    Graph *getGraph();
    QWidget *getWidget();
    GlMainWidget *getGlMainWidget();

    void constructInteractorsMap();
    void getInteractorsActionList(std::list<QAction*> &);
    Iterator<Interactor *> *installInteractor(const std::string &);

    bool eventFilter(QObject *, QEvent *);

    virtual bool cutIsEnable() {return true;}
    virtual bool copyIsEnable() {return true;}
    virtual bool pasteIsEnable() {return true;}
    virtual bool findIsEnable() {return true;}
    virtual bool selectAllIsEnable() {return true;}
    virtual bool delSelectionIsEnable() {return true;}
    virtual bool deselectAllIsEnable() {return true;}
    virtual bool invertSelectionIsEnable() {return true;}
    virtual bool createGroupIsEnable() {return true;}
    virtual bool createSubgraphIsEnable() {return true;}

    virtual bool doProgressUpdate() {return true;}
    virtual void progressUpdate();

  protected :

    std::string newName();
    // GlSceneObserver interface
    void addLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);
    void modifyLayer(tlp::GlScene*, const std::string&, tlp::GlLayer*);

    GlMainWidget *mainWidget;

    QDockWidget *overviewDock;
    GWOverviewWidget *overviewWidget;

    QAction *renderingParametersDialogAction;
    RenderingParametersDialog *renderingParametersDialog;
    LayerManagerWidget *layerWidget;

    tlp::Graph * copyCutPasteGraph;				    

  protected slots:
    void centerView();
    void showDialog(QAction*);

  public slots:
    void redrawView();
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
