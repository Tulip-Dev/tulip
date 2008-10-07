//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef PARALLEL_COORDINATES_WIDGET_H
#define PARALLEL_COORDINATES_WIDGET_H

#include <tulip/GlMainWidget.h>
#include <tulip/MouseInteractors.h>
#include <QtGui/QWidget>

#include <tulip/View.h>

#include "ParallelCoordinatesView.h"
#include "ParallelCoordinatesGraphProxy.h"
#include "ViewConfigDialog.h"

namespace tlp {

  class ParallelCoordinatesWidget : public GlMainWidget, Observer, GraphObserver {

  public :
    
    ParallelCoordinatesWidget(View *parent);
    ~ParallelCoordinatesWidget();
    
    void setData(Graph *graph,DataSet dataSet);
    void getData(Graph **graph,DataSet *dataSet);

    Graph *getGraph();
    QWidget* getWidget() {return this;}
    
    void swapAxis(string axisName1,string axisName2);
    
    void setViewType(viewType vType);
    
    bool eventFilter(QObject *obj, QEvent *e);
    
    void showConfigDialog();

    void addNode(Graph *,const node ){}
    void addEdge(Graph *,const edge ){}
    void delNode(Graph *,const node );
    void delEdge(Graph *,const edge );
    void reverseEdge(Graph *,const edge ){}
    void destroy(Graph *){}
    
    void update(set<Observable *>::iterator begin ,set<Observable *>::iterator end);
    void observableDestroyed(Observable *) {}
    void redrawView();

 private :

    void initObservers();
    void clearObservers();
    
    void createView(viewType vType);
    
    ViewConfigDialog *vcd;
    ParallelCoordinatesGraphProxy *graphProxy;
    GlGraphComposite *glGraphComposite;
    GlLayer *mainLayer;
    GlLayer *viewLayer;
    DataSet *dataSet;
    MouseNKeysNavigator *navigator;
    ParallelCoordinatesView *parallelCoordsView;
    Graph *graph;
    View *view;
    
  };

}

#endif // PARALLEL_COORDINATES_WIDGET_H
