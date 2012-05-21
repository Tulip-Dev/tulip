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
#ifndef _Tulip_VIEWINTERFACE_H
#define _Tulip_VIEWINTERFACE_H

#include <tulip/ViewWidget.h>

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>


#include <tulip/Plugin.h>
#include <tulip/PluginLister.h>
#include <tulip/ObservableGraph.h>
#include <tulip/Vector.h>
#include <tulip/Plugin.h>

class QMenuBar;
class QVBoxLayout;

class QMenuBar;
class QVBoxLayout;

namespace tlp {

class Interactor;

class TLP_QT_SCOPE View3 : public ViewWidget {
  Q_OBJECT

  bool _dataInitialized;

public:
  View3();
  virtual ~View3() {}

  tlp::DataSet state() const;
  void setState(const tlp::DataSet& data);

public slots:
  void draw(tlp::PluginProgress*);

protected:
  void currentInteractorChanged(tlp::Interactor *i);
  void graphChanged(tlp::Graph *g);
  void interactorsInstalled(const QList<tlp::Interactor *> &lst);
  void setupWidget();
  /*
    Methods from Tulip 3
    */
public:
  virtual QWidget *construct(QWidget *parent) = 0;
  virtual QWidget *getWidget() =0;
  virtual void setData(Graph *graph,DataSet dataSet) = 0;
  virtual void getData(Graph **graph,DataSet *dataSet) = 0;
  virtual Graph *getGraph() = 0;
  virtual std::list<std::pair<QWidget *,std::string> > getConfigurationWidget() {
    return std::list<std::pair<QWidget *,std::string> >();
  }
  virtual void setInteractors(const std::list<Interactor *> &interactors) = 0;
  virtual std::list<Interactor *> getInteractors() = 0;
  virtual void setActiveInteractor(Interactor *interactor) = 0;
  virtual Interactor *getActiveInteractor() = 0;
  virtual bool _DEPRECATED createPicture(const std::string& pictureName, int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);
  virtual bool savePicture(const std::string& pictureName, int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);
  virtual QImage createPicture(int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);
  virtual std::string getRealViewName() const {
    return "";
  }
  virtual void undoIsDone() {}

public slots:
  /**
    @note This method has been renamed from setGraph to changeGraph to prevent name conflicts with new View system
    */
  virtual void changeGraph(Graph *graph) = 0;
  virtual void draw() = 0;
  virtual void refresh() = 0;
  virtual void init() = 0;

signals:
  void elementSelected(unsigned int eltId, bool isNode);
  void requestChangeGraph(tlp::View3* view,tlp::Graph* graph);
};

}

#endif
