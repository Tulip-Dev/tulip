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
#include "tulip/View3.h"

using namespace std;
using namespace tlp;

View3::View3(): _dataInitialized(false) {
}

void View3::setupWidget() {
  setCentralWidget(getWidget());
}

tlp::DataSet View3::state() const {
  View3* v = const_cast<View3*>(this);
  tlp::Graph* g;
  DataSet d;
  v->getData(&g,&d);
  return d;
}

void View3::setState(const tlp::DataSet& data) {
  setData(graph(),data);
}

void View3::currentInteractorChanged(tlp::Interactor *i) {
  setActiveInteractor(i);
}

void View3::graphChanged(tlp::Graph *g) {
  if (!_dataInitialized) {
    construct(NULL);
    setData(g,DataSet());
    _dataInitialized = true;
  }
  else
    changeGraph(g);
}

void View3::interactorsInstalled(const QList<tlp::Interactor *> &lst) {
  setInteractors(lst.toStdList());
}

/*
  Methods from Tulip 3
  */
bool View3::createPicture(const std::string& pictureName, int width, int height, bool center, int zoom, int xOffset, int yOffset) {
  return savePicture(pictureName,width,height,center,zoom,xOffset,yOffset);
}

bool View3::savePicture(const std::string& , int , int , bool , int , int , int ) {
  return false;
}

QImage View3::createPicture(int, int, bool, int, int, int) {
  return QImage();
}

void View3::draw(tlp::PluginProgress *) {
  draw();
}
