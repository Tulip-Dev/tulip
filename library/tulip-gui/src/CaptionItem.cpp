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
#include "tulip/CaptionItem.h"

#include <tulip/DoubleProperty.h>
#include <tulip/ColorProperty.h>

using namespace std;

namespace tlp {

CaptionItem::CaptionItem(View *view):view(view),_graph(NULL),_metricProperty(NULL),_colorProperty(NULL) {
  _captionGraphicsItem=new CaptionGraphicsItem(view);
  connect(_captionGraphicsItem,SIGNAL(filterChanged(float,float)),this,SLOT(applyNewFilter(float,float)));
  connect(_captionGraphicsItem,SIGNAL(selectedPropertyChanged(std::string)),this,SLOT(selectedPropertyChanged(std::string)));
}

CaptionItem::~CaptionItem() {
}

void CaptionItem::create() {
  initCaption();
  generateCaption();
}

void CaptionItem::initCaption() {
  _captionGraphicsItem->loadConfiguration();
}

void CaptionItem::generateCaption() {

  if(_graph!=view->graph()) {
    if(_graph)
      _graph->removeGraphObserver(this);

    _graph=view->graph();
    _graph->addGraphObserver(this);
  }

  if(_metricProperty!=view->graph()->getProperty<DoubleProperty>(_captionGraphicsItem->usedProperty())) {
    if(_metricProperty)
      _metricProperty->removePropertyObserver(this);

    _metricProperty=view->graph()->getProperty<DoubleProperty>(_captionGraphicsItem->usedProperty());
    _metricProperty->addPropertyObserver(this);
  }

  if(_colorProperty!=view->graph()->getProperty<ColorProperty>("viewColor")) {
    if(_colorProperty)
      _colorProperty->removePropertyObserver(this);

    _colorProperty=view->graph()->getProperty<ColorProperty>("viewColor");
    _colorProperty->addPropertyObserver(this);
  }

  map<double,Color> metricToColorMap;
  vector<pair <double,Color> > metricToColorFiltered;
  Iterator<node> *itN=view->graph()->getNodes();

  for (; itN->hasNext();) {
    node nit=itN->next();
    metricToColorMap[_metricProperty->getNodeValue(nit)]=_colorProperty->getNodeValue(nit);
  }

  double minProp=_metricProperty->getNodeMin();
  double maxProp=_metricProperty->getNodeMax();

  double intervale=(maxProp-minProp)/50.;
  double nextValue=minProp;

  for(map<double,Color>::const_iterator it=metricToColorMap.begin(); it!=metricToColorMap.end(); ++it) {
    if((*it).first>=nextValue) {
      metricToColorFiltered.push_back(*it);
      nextValue+=intervale;
    }
  }

  QLinearGradient activeGradient(QPointF(0,0),QPointF(0,160.));
  QLinearGradient hideGradient(QPointF(0,0),QPointF(0,160.));

  generateGradients(metricToColorFiltered,activeGradient,hideGradient);
  _captionGraphicsItem->generateCaption(activeGradient,hideGradient,_captionGraphicsItem->usedProperty(),minProp,maxProp);
}

void CaptionItem::generateGradients(const vector<pair <double,Color> > &metricToColorFiltered, QGradient &activeGradient, QGradient &hideGradient) {
  double minProp=(*metricToColorFiltered.begin()).first;
  double maxProp=(*(--metricToColorFiltered.end())).first;

  Color color;

  for(vector<pair<double,Color> >::const_iterator it=metricToColorFiltered.begin(); it!=metricToColorFiltered.end(); ++it) {
    float position=((*it).first-minProp)/(maxProp-minProp);
    color=(*it).second;
    activeGradient.setColorAt(position,QColor(color[0],color[1],color[2],255));
    hideGradient.setColorAt(position,QColor(color[0],color[1],color[2],100));
  }
}

QGraphicsItem *CaptionItem::captionGraphicsItem() {
  return _captionGraphicsItem->getCaptionItem();
}

void CaptionItem::applyNewFilter(float begin, float end) {

  DoubleProperty *metricProperty=view->graph()->getProperty<DoubleProperty>(_captionGraphicsItem->usedProperty());
  ColorProperty *colorProperty=view->graph()->getProperty<ColorProperty>("viewColor");

  double minProp=metricProperty->getNodeMin();
  double maxProp=metricProperty->getNodeMax();

  double beginMetric=minProp+(begin*(maxProp-minProp));
  double endMetric=minProp+(end*(maxProp-minProp));

  metricProperty->removePropertyObserver(this);
  colorProperty->removePropertyObserver(this);

  Observer::holdObservers();

  Iterator<node> *itN=view->graph()->getNodes();

  for (; itN->hasNext();) {
    node nit=itN->next();
    Color tmp(colorProperty->getNodeValue(nit));

    if(metricProperty->getNodeValue(nit)<beginMetric || metricProperty->getNodeValue(nit)>endMetric) {
      tmp[3]=25;
      colorProperty->setNodeValue(nit,tmp);
    }
    else {
      tmp[3]=255;
      colorProperty->setNodeValue(nit,tmp);
    }

  }

  delete itN;

  Observer::unholdObservers();

  metricProperty->addPropertyObserver(this);
  colorProperty->addPropertyObserver(this);
}

void CaptionItem::treatEvent(const Event &ev) {
  if(typeid(ev) == typeid(PropertyEvent)) {
    generateCaption();
  }

  if(typeid(ev) == typeid(GraphEvent)) {
    create();
  }
}

void CaptionItem::selectedPropertyChanged(string propertyName) {
  generateCaption();
}

}
