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
#include <tulip/SizeProperty.h>

using namespace std;

namespace tlp {

CaptionItem::CaptionItem(View *view):view(view),_graph(NULL),_metricProperty(NULL),_colorProperty(NULL),_sizeProperty(NULL) {
  _captionGraphicsItem=new CaptionGraphicsItem(view);
  connect(_captionGraphicsItem,SIGNAL(filterChanged(float,float)),this,SLOT(applyNewFilter(float,float)));
  connect(_captionGraphicsItem,SIGNAL(selectedPropertyChanged(std::string)),this,SLOT(selectedPropertyChanged(std::string)));
  connect(_captionGraphicsItem,SIGNAL(selectedTypeChanged(std::string)),this,SLOT(selectedTypeChanged(std::string)));
}

CaptionItem::~CaptionItem() {
}

void CaptionItem::create(CaptionType captionType) {
  _captionType=captionType;
  initCaption();

  if(captionType==ColorCaption)
    generateColorCaption();
  else
    generateSizeCaption();
}

void CaptionItem::initCaption() {
  _captionGraphicsItem->loadConfiguration();

  if(_metricProperty)
    _metricProperty->removePropertyObserver(this);

  _metricProperty=NULL;

  if(_colorProperty)
    _colorProperty->removePropertyObserver(this);

  _colorProperty=NULL;

  if(_sizeProperty)
    _sizeProperty->removePropertyObserver(this);

  _sizeProperty=NULL;


}

void CaptionItem::clearObservers() {
  if(_graph!=view->graph()) {
    if(_graph)
      _graph->removeGraphObserver(this);

    _graph=view->graph();
    _graph->addGraphObserver(this);
  }

  if(_metricProperty)
    _metricProperty->removePropertyObserver(this);

  _metricProperty=view->graph()->getProperty<DoubleProperty>(_captionGraphicsItem->usedProperty());
  _metricProperty->addPropertyObserver(this);

  if(_captionType==ColorCaption) {
    if(_colorProperty)
      _colorProperty->removePropertyObserver(this);
  }else{
    if(_sizeProperty)
      _sizeProperty->removePropertyObserver(this);

    _sizeProperty=view->graph()->getProperty<SizeProperty>("viewSize");
    _sizeProperty->addPropertyObserver(this);
  }

  _colorProperty=view->graph()->getProperty<ColorProperty>("viewColor");

  if(_captionType==ColorCaption) {
    _colorProperty->addPropertyObserver(this);
  }
}

void CaptionItem::generateColorCaption() {

  clearObservers();

  map<double,Color> metricToColorMap;
  vector<pair <double,Color> > metricToColorFiltered;
  Iterator<node> *itN=view->graph()->getNodes();

  for (; itN->hasNext();) {
    node nit=itN->next();
    metricToColorMap[_metricProperty->getNodeValue(nit)]=_colorProperty->getNodeValue(nit);
  }

  delete itN;

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
  _captionGraphicsItem->generateColorCaption(activeGradient,hideGradient,_captionGraphicsItem->usedProperty(),minProp,maxProp);
}

void CaptionItem::generateSizeCaption() {

  clearObservers();

  double minProp=_metricProperty->getNodeMin();
  double maxProp=_metricProperty->getNodeMax();
  float maxSize=0;

  map<double,float> metricToSizeMap;
  vector<pair <double,float> > metricToSizeFiltered;
  Iterator<node> *itN=view->graph()->getNodes();

  for (; itN->hasNext();) {
    node nit=itN->next();
    metricToSizeMap[_metricProperty->getNodeValue(nit)]=_sizeProperty->getNodeValue(nit)[0];

    if(maxSize<_sizeProperty->getNodeValue(nit)[0])
      maxSize=_sizeProperty->getNodeValue(nit)[0];
  }

  delete itN;

  double intervale=(maxProp-minProp)/50.;
  double nextValue=minProp;

  for(map<double,float>::const_iterator it=metricToSizeMap.begin(); it!=metricToSizeMap.end(); ++it) {
    if((*it).first>=nextValue) {
      metricToSizeFiltered.push_back(pair<double,float>((*it).first,(*it).second/maxSize));
      nextValue+=intervale;
    }
  }

  if(metricToSizeFiltered.size()==1) {
    metricToSizeFiltered.push_back(metricToSizeFiltered[0]);
  }

  _captionGraphicsItem->generateSizeCaption(metricToSizeFiltered,_captionGraphicsItem->usedProperty(),minProp,maxProp);
}

void CaptionItem::generateGradients(const vector<pair <double,Color> > &metricToColorFiltered, QGradient &activeGradient, QGradient &hideGradient) {
  double minProp=(*metricToColorFiltered.begin()).first;
  double maxProp=(*(--metricToColorFiltered.end())).first;

  Color color;

  for(vector<pair<double,Color> >::const_iterator it=metricToColorFiltered.begin(); it!=metricToColorFiltered.end(); ++it) {
    float position=1.-((*it).first-minProp)/(maxProp-minProp);
    color=(*it).second;
    activeGradient.setColorAt(position,QColor(color[0],color[1],color[2],255));
    hideGradient.setColorAt(position,QColor(color[0],color[1],color[2],100));
  }
}

CaptionGraphicsBackgroundItem *CaptionItem::captionGraphicsItem() {
  return _captionGraphicsItem->getCaptionItem();
}

void CaptionItem::removeObservation(bool remove){
  if(!remove){
    _graph->addGraphObserver(this);
    _metricProperty->addPropertyObserver(this);
    if(_captionType==ColorCaption) {
      _colorProperty->addPropertyObserver(this);
    }else{
      _sizeProperty->addPropertyObserver(this);
    }
  }else{
    _graph->removeGraphObserver(this);
    _metricProperty->removePropertyObserver(this);
    if(_captionType==ColorCaption) {
      _colorProperty->removePropertyObserver(this);
    }else{
      _sizeProperty->removePropertyObserver(this);
    }
  }
}

void CaptionItem::applyNewFilter(float begin, float end) {
  emit filtering(true);
  _graph->removeGraphObserver(this);
  _metricProperty->removePropertyObserver(this);
  if(_captionType==ColorCaption) {
    _colorProperty->removePropertyObserver(this);
  }else{
    _sizeProperty->removePropertyObserver(this);
  }

  ColorProperty *borderColorProperty=_graph->getProperty<ColorProperty>("viewBorderColor");

  double minProp=_metricProperty->getNodeMin();
  double maxProp=_metricProperty->getNodeMax();

  double beginMetric=minProp+(begin*(maxProp-minProp));
  double endMetric=minProp+(end*(maxProp-minProp));

  Observer::holdObservers();

  Iterator<node> *itN=view->graph()->getNodes();

  Color tmp;
  Color borderTmp;

  for (; itN->hasNext();) {
    node nit=itN->next();

    tmp=Color(_colorProperty->getNodeValue(nit));
    borderTmp=Color(borderColorProperty->getNodeValue(nit));

    if(_metricProperty->getNodeValue(nit)<beginMetric || _metricProperty->getNodeValue(nit)>endMetric) {
      tmp[3]=25;
      borderTmp[3]=25;
      _colorProperty->setNodeValue(nit,tmp);
      borderColorProperty->setNodeValue(nit,borderTmp);
    }
    else {
      tmp[3]=255;
      borderTmp[3]=255;
      _colorProperty->setNodeValue(nit,tmp);
      borderColorProperty->setNodeValue(nit,borderTmp);
    }

  }

  delete itN;

  Observer::unholdObservers();

  _graph->addGraphObserver(this);
  _metricProperty->addPropertyObserver(this);
  if(_captionType==ColorCaption) {
    _colorProperty->addPropertyObserver(this);
  }else{
    _sizeProperty->addPropertyObserver(this);
  }

  emit filtering(false);
}

void CaptionItem::treatEvent(const Event &ev) {
  if(typeid(ev) == typeid(PropertyEvent)) {
    if(_captionType==ColorCaption)
      generateColorCaption();
    else
      generateSizeCaption();
  }

  if(typeid(ev) == typeid(GraphEvent)) {
    create(_captionType);
  }
}

void CaptionItem::selectedPropertyChanged(string propertyName) {
  if(_captionType==ColorCaption)
    generateColorCaption();
  else
    generateSizeCaption();
}

void CaptionItem::selectedTypeChanged(string typeName) {
  if(typeName=="Color") {
    generateColorCaption();
    _captionType=ColorCaption;
  }
  else {
    generateSizeCaption();
    _captionType=SizeCaption;
  }
}

}
