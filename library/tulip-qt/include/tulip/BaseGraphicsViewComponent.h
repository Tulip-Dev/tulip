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

#ifndef _Tulip_BASEGRAPHICSVIEWCOMPONENT_H
#define _Tulip_BASEGRAPHICSVIEWCOMPONENT_H

#include <tulip/AbstractView.h>
#include <tulip/TabWidgetHidableMenuGraphicsProxy.h>

class QFrame;
class QGraphicsView;

namespace tlp {

class GlMainWidgetGraphicsView;
class GlMainWidgetItem;
class GWOverviewWidget;


class TLP_QT_SCOPE BaseGraphicsViewComponent : public AbstractView {

  Q_OBJECT

public :

  BaseGraphicsViewComponent(const std::string &realViewName);

  virtual ~BaseGraphicsViewComponent();

  virtual QWidget *construct(QWidget *parent);

  virtual void setData(Graph *graph,DataSet dataSet) {
    baseView->setData(graph,dataSet);
  }

  virtual void getData(Graph **graph,DataSet *dataSet) {
    baseView->getData(graph,dataSet);
  }

  virtual Graph *getGraph() {
    return baseView->getGraph();
  }

  virtual void setInteractors(const std::list<Interactor *> &interactors) {
    baseView->setInteractors(interactors);
  }

  virtual std::list<Interactor *> getInteractors() {
    return baseView->getInteractors();
  }

  virtual void setActiveInteractor(Interactor *interactor) {
    baseView->setActiveInteractor(interactor);
  }

  virtual void createPicture(const std::string &pictureName,int width=0, int height=0) {
    baseView->savePicture(pictureName, width, height, false);
  }

  virtual bool savePicture(const std::string &pictureName,int width, int height, bool center, int zoom=1, int xOffset=0, int yOffset = 0) {
    return baseView->savePicture(pictureName, width, height, center, zoom, xOffset, yOffset);
  }

  virtual tlp::View *getRealView() const {
    return baseView;
  }

  virtual std::string getRealViewName() const;

  TabWidgetHidableMenuGraphicsProxy *getOptionsTabWiget() const {
    return tabWidgetProxy;
  }

public slots:

  virtual void setGraph(Graph *graph) {
    baseView->setGraph(graph);
  }

  virtual void draw();

  virtual void refresh();

  virtual void init();

  virtual void elementSelectedSlot(unsigned int,bool);

  void setOptionsTabWidgetVisible(const bool visible);

protected slots :

  void hideOverview(bool);
  void setVisibleOverview(bool);

protected :

  TabWidgetHidableMenuGraphicsProxy *tabWidgetProxy;
  GlMainWidgetItem *overviewItem;
  QFrame *overviewFrame;
  std::string realViewName;
  QGraphicsView *graphicsView;
  View *baseView;
  QAction *showHideOptionWidgets;

};

}

#endif






