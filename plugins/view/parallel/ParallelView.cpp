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

#define SPACE_BETWEEN_AXIS 100
#include "ParallelView.h"

using namespace std;

namespace tlp {

  VIEWPLUGIN(ParallelCoordinatesViewCreator, "ParallelCoordinatesView", "Tulip Team", "16/04/2008", "parallel coordinates view", "1.0", 2);

  View* ParallelCoordinatesViewCreator::create(QWidget *parent){
    return new ParallelView(parent);
  }

  ParallelView::ParallelView(QWidget *parent) : View(parent){
    setupUi(this);

    mainWidget=new ParallelCoordinatesWidget(this);
    setCentralWidget(mainWidget);

    dialogMenu->addAction("Config");
    connect(dialogMenu, SIGNAL(triggered(QAction*)), SLOT(showDialog(QAction*)));
  }

  ParallelView::~ParallelView() {
    delete mainWidget;
  }

  void ParallelView::setData(Graph *graph,string *in) {
    mainWidget->setData(graph,in);
  }

  Graph* ParallelView::getGraph() {
    return mainWidget->getGraph();
  }

  void ParallelView::showDialog(QAction *action) {
    mainWidget->showConfigDialog();
  }

}
