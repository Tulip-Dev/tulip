//-*-c++-*-
/**
 Author: Antoine Lambert Morgan Mathiaut
 Last modification : 08/12/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef _Tulip_BASEGRAPHICSVIEWCOMPONENT_H
#define _Tulip_BASEGRAPHICSVIEWCOMPONENT_H

#include "tulip/AbstractView.h"

class QFrame;

namespace tlp {

  class GlMainWidgetGraphicsView;
  class GWOverviewWidget;

class TLP_QT_SCOPE BaseGraphicsViewComponent : public AbstractView{

  Q_OBJECT

public :

  BaseGraphicsViewComponent(const std::string &realViewName);
  virtual ~BaseGraphicsViewComponent();

  virtual QWidget *construct(QWidget *parent);

  virtual void setData(Graph *graph,DataSet dataSet) {baseView->setData(graph,dataSet);}

  virtual void getData(Graph **graph,DataSet *dataSet) {baseView->getData(graph,dataSet);}

  virtual Graph *getGraph() {return baseView->getGraph();}

  virtual void setInteractors(const std::list<Interactor *> &interactors) {baseView->setInteractors(interactors);}

  virtual std::list<Interactor *> getInteractors() {return baseView->getInteractors();}

  virtual void setActiveInteractor(Interactor *interactor) {baseView->setActiveInteractor(interactor);}

  virtual void createPicture(const std::string &pictureName,int width=0, int height=0) {baseView->createPicture(pictureName,width,height);}

  virtual std::string getRealViewName();
public slots:

  virtual void setGraph(Graph *graph) {baseView->setGraph(graph);}

  virtual void draw();

  virtual void refresh();

  virtual void init();

  virtual void elementSelectedSlot(unsigned int,bool);

protected :

  GWOverviewWidget *overviewWidget;
  QFrame *overviewFrame;
  std::string realViewName;
  GlMainWidgetGraphicsView *graphicsView;
  View *baseView;

};

}

#endif






