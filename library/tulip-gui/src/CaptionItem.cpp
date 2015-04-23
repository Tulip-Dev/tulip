/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <tulip/CaptionGraphicsItem.h>
#include <tulip/View.h>

#include <QtGui/QLinearGradient>

using namespace std;

namespace tlp {

CaptionItem::CaptionItem(View *view):view(view),_graph(NULL),_metricProperty(NULL),_colorProperty(NULL),_sizeProperty(NULL),_backupColorProperty(NULL),_backupBorderColorProperty(NULL) {
  _captionGraphicsItem=new CaptionGraphicsItem(view);
  connect(_captionGraphicsItem,SIGNAL(filterChanged(float,float)),this,SLOT(applyNewFilter(float,float)));
  connect(_captionGraphicsItem,SIGNAL(selectedPropertyChanged(std::string)),this,SLOT(selectedPropertyChanged(std::string)));
}

CaptionItem::~CaptionItem() {
  clearObservers();
}

void CaptionItem::create(CaptionType captionType) {
  _captionType=captionType;
  _captionGraphicsItem->setType(captionType);
  initCaption();

  if(captionType==NodesColorCaption || captionType==EdgesColorCaption)
    generateColorCaption(captionType);
  else
    generateSizeCaption(captionType);

  if(_backupColorProperty) {
    delete _backupColorProperty;
    delete _backupBorderColorProperty;
  }

  _backupColorProperty=new ColorProperty(_graph);
  *_backupColorProperty=*_colorProperty;
  _backupBorderColorProperty=NULL;
}

void CaptionItem::initCaption() {
  _captionGraphicsItem->loadConfiguration();

  if(_metricProperty)
    _metricProperty->removeObserver(this);

  _metricProperty=NULL;

  if(_colorProperty)
    _colorProperty->removeObserver(this);

  _colorProperty=NULL;

  if(_sizeProperty)
    _sizeProperty->removeObserver(this);

  _sizeProperty=NULL;


}

void CaptionItem::clearObservers() {

  if(_graph!=view->graph())
    if(_graph)
      _graph->removeObserver(this);

  _graph=view->graph();

  if(_graph==NULL) {
    _metricProperty=NULL;
    _colorProperty=NULL;
    _sizeProperty=NULL;
    return;
  }

  if(_metricProperty)
    _metricProperty->removeObserver(this);

  if(_captionGraphicsItem->usedProperty()!="") {
    _metricProperty=view->graph()->getProperty<DoubleProperty>(_captionGraphicsItem->usedProperty());
    _metricProperty->addObserver(this);
  }
  else {
    _metricProperty=NULL;
  }

  if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption) {
    if(_colorProperty)
      _colorProperty->removeObserver(this);
  }
  else {
    if(_sizeProperty)
      _sizeProperty->removeObserver(this);

    _sizeProperty=view->graph()->getProperty<SizeProperty>("viewSize");
    _sizeProperty->addObserver(this);
  }

  _colorProperty=view->graph()->getProperty<ColorProperty>("viewColor");

  if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption) {
    _colorProperty->addObserver(this);
  }

  if(_graph) {
    _graph->removeObserver(this);
    _graph->addObserver(this);
  }
}

void CaptionItem::generateColorCaption(CaptionType captionType) {

  clearObservers();

  vector<pair <double,Color> > metricToColorFiltered;
  double minProp=0;
  double maxProp=1;
  QLinearGradient activeGradient(QPointF(0,0),QPointF(0,160.));
  QLinearGradient hideGradient(QPointF(0,0),QPointF(0,160.));

  string propertyName="empty";

  if(_metricProperty==NULL) {
    metricToColorFiltered.push_back(pair<double,Color>(0.,Color(255,255,255,255)));
    metricToColorFiltered.push_back(pair<double,Color>(1.,Color(255,255,255,255)));
  }
  else {

    map<double,Color> metricToColorMap;

    if(captionType==NodesColorCaption) {
      minProp=_metricProperty->getNodeMin();
      maxProp=_metricProperty->getNodeMax();
      Iterator<node> *itN=view->graph()->getNodes();

      for (; itN->hasNext();) {
        node nit=itN->next();
        metricToColorMap[_metricProperty->getNodeValue(nit)]=_colorProperty->getNodeValue(nit);
      }

      delete itN;
    }
    else {
      minProp=_metricProperty->getEdgeMin();
      maxProp=_metricProperty->getEdgeMax();
      Iterator<edge> *itE=view->graph()->getEdges();

      for (; itE->hasNext();) {
        edge eit=itE->next();
        metricToColorMap[_metricProperty->getEdgeValue(eit)]=_colorProperty->getEdgeValue(eit);
      }

      delete itE;
    }

    double intervale=(maxProp-minProp)/50.;
    double nextValue=minProp;

    for(map<double,Color>::const_iterator it=metricToColorMap.begin(); it!=metricToColorMap.end(); ++it) {
      if((*it).first>=nextValue) {
        metricToColorFiltered.push_back(*it);
        nextValue+=intervale;
      }
    }

    propertyName=_captionGraphicsItem->usedProperty();
  }

  if(metricToColorFiltered.size()<2) {
    metricToColorFiltered.push_back(pair<double,Color>(0.,Color(255,255,255,255)));
    metricToColorFiltered.push_back(pair<double,Color>(1.,Color(255,255,255,255)));
  }

  generateGradients(metricToColorFiltered,activeGradient,hideGradient);
  _captionGraphicsItem->generateColorCaption(activeGradient,hideGradient,propertyName,minProp,maxProp);
}

void CaptionItem::generateSizeCaption(CaptionType captionType) {

  clearObservers();

  if(!_metricProperty) {
    vector<pair <double,float> > metricToSizeFiltered;
    metricToSizeFiltered.push_back(pair<double,float>(0.,1.));
    metricToSizeFiltered.push_back(pair<double,float>(1.,1.));
    _captionGraphicsItem->generateSizeCaption(metricToSizeFiltered,"empty",0.,1.);
    return;
  }

  double minProp=_metricProperty->getNodeMin();
  double maxProp=_metricProperty->getNodeMax();
  float maxSize=0;

  map<double,float> metricToSizeMap;
  vector<pair <double,float> > metricToSizeFiltered;

  if(captionType==NodesSizeCaption) {
    Iterator<node> *itN=view->graph()->getNodes();

    for (; itN->hasNext();) {
      node nit=itN->next();
      metricToSizeMap[_metricProperty->getNodeValue(nit)]=_sizeProperty->getNodeValue(nit)[0];

      if(maxSize<_sizeProperty->getNodeValue(nit)[0])
        maxSize=_sizeProperty->getNodeValue(nit)[0];
    }

    delete itN;
  }
  else {
    Iterator<edge> *itE=view->graph()->getEdges();

    for (; itE->hasNext();) {
      edge eit=itE->next();
      metricToSizeMap[_metricProperty->getEdgeValue(eit)]=_sizeProperty->getEdgeValue(eit)[0];

      if(maxSize<_sizeProperty->getEdgeValue(eit)[0])
        maxSize=_sizeProperty->getEdgeValue(eit)[0];
    }

    delete itE;
  }

  double intervale=(maxProp-minProp)/50.;
  double nextValue=minProp;

  for(map<double,float>::const_iterator it=metricToSizeMap.begin(); it!=metricToSizeMap.end(); ++it) {
    if((*it).first>=nextValue) {
      metricToSizeFiltered.push_back(pair<double,float>((*it).first,(*it).second/maxSize));
      nextValue+=intervale;
    }
  }

  if(metricToSizeFiltered.empty()) {
    metricToSizeFiltered.push_back(pair<double,float>(minProp,0));
    metricToSizeFiltered.push_back(pair<double,float>(maxProp,0));
  }

  if(metricToSizeFiltered.size()==1) {
    metricToSizeFiltered.push_back(metricToSizeFiltered[0]);
  }

  if(metricToSizeFiltered.size()<2) {
    metricToSizeFiltered.clear();
    metricToSizeFiltered.push_back(pair<double,float>(0.,1.));
    metricToSizeFiltered.push_back(pair<double,float>(1.,1.));
    _captionGraphicsItem->generateSizeCaption(metricToSizeFiltered,"empty",0.,1.);
    return;
  }

  _captionGraphicsItem->generateSizeCaption(metricToSizeFiltered,_captionGraphicsItem->usedProperty(),minProp,maxProp);
}

void CaptionItem::generateGradients(const vector<pair <double,Color> > &metricToColorFiltered, QGradient &activeGradient, QGradient &hideGradient) {
  double minProp=(*metricToColorFiltered.begin()).first;
  double maxProp=(*(--metricToColorFiltered.end())).first;

  Color color;

  for(vector<pair<double,Color> >::const_iterator it=metricToColorFiltered.begin(); it!=metricToColorFiltered.end(); ++it) {
    float position= (maxProp-minProp) ?
                    (1.-((*it).first-minProp)/(maxProp-minProp)) :
                    0.0;
    color=(*it).second;
    activeGradient.setColorAt(position,QColor(color[0],color[1],color[2],255));
    hideGradient.setColorAt(position,QColor(color[0],color[1],color[2],100));
  }
}

CaptionGraphicsBackgroundItem *CaptionItem::captionGraphicsItem() {
  return _captionGraphicsItem->getCaptionItem();
}

void CaptionItem::removeObservation(bool remove) {
  if(!remove) {
    _graph->addObserver(this);
    _metricProperty->addObserver(this);

    if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption) {
      _colorProperty->addObserver(this);
    }
    else {
      _sizeProperty->addObserver(this);
    }
  }
  else {
    _graph->removeObserver(this);

    if(_metricProperty)
      _metricProperty->removeObserver(this);

    if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption) {
      _colorProperty->removeObserver(this);
    }
    else {
      _sizeProperty->removeObserver(this);
    }
  }
}

void CaptionItem::applyNewFilter(float begin, float end) {
  if(_metricProperty==NULL)
    return;

  emit filtering(true);
  _graph->removeObserver(this);
  _metricProperty->removeObserver(this);

  if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption) {
    _colorProperty->removeObserver(this);
  }
  else {
    _sizeProperty->removeObserver(this);
  }

  Observable::holdObservers();

  ColorProperty *borderColorProperty=_graph->getProperty<ColorProperty>("viewBorderColor");

  if(!_backupBorderColorProperty) {
    _backupBorderColorProperty=new ColorProperty(_graph);
    *_backupBorderColorProperty=*borderColorProperty;
  }
  else {
    *borderColorProperty=*_backupBorderColorProperty;
  }

  *_colorProperty=*_backupColorProperty;




  Color tmp;
  Color borderTmp;

  if(_captionType==NodesColorCaption || _captionType==NodesSizeCaption) {
    double minProp=_metricProperty->getNodeMin();
    double maxProp=_metricProperty->getNodeMax();

    double beginMetric=minProp+(begin*(maxProp-minProp));
    double endMetric=minProp+(end*(maxProp-minProp));

    Iterator<node> *itN=view->graph()->getNodes();

    for (; itN->hasNext();) {
      node nit=itN->next();

      tmp=Color(_backupColorProperty->getNodeValue(nit));
      borderTmp=Color(_backupBorderColorProperty->getNodeValue(nit));

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
  }
  else {
    double minProp=_metricProperty->getEdgeMin();
    double maxProp=_metricProperty->getEdgeMax();

    double beginMetric=minProp+(begin*(maxProp-minProp));
    double endMetric=minProp+(end*(maxProp-minProp));

    Iterator<edge> *itE=view->graph()->getEdges();

    for (; itE->hasNext();) {
      edge eit=itE->next();

      tmp=Color(_backupColorProperty->getEdgeValue(eit));
      borderTmp=Color(_backupBorderColorProperty->getEdgeValue(eit));

      if(_metricProperty->getEdgeValue(eit)<beginMetric || _metricProperty->getEdgeValue(eit)>endMetric) {
        tmp[3]=25;
        borderTmp[3]=25;
        _colorProperty->setEdgeValue(eit,tmp);
        borderColorProperty->setEdgeValue(eit,borderTmp);
      }
      else {
        tmp[3]=255;
        borderTmp[3]=255;
        _colorProperty->setEdgeValue(eit,tmp);
        borderColorProperty->setEdgeValue(eit,borderTmp);
      }
    }

    delete itE;
  }

  Observable::unholdObservers();

  _graph->addObserver(this);
  _metricProperty->addObserver(this);

  if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption) {
    _colorProperty->addObserver(this);
  }
  else {
    _sizeProperty->addObserver(this);
  }

  emit filtering(false);
}

void CaptionItem::treatEvents(const vector<Event> &ev) {

  bool deleteEvent=false;
  bool propertyEvent=false;
  bool graphEvent=false;

  for(vector<Event>::const_iterator it=ev.begin(); it!=ev.end(); ++it) {

    PropertyInterface *prop=dynamic_cast<PropertyInterface*>((*it).sender());

    if(typeid(*it)==typeid(Event))
      if((*it).type()==Event::TLP_DELETE)
        deleteEvent=true;

    if(prop)
      propertyEvent=true;

    if(typeid(*it)==typeid(GraphEvent))
      graphEvent=true;
  }

  if(deleteEvent)
    create(_captionType);

  if(propertyEvent) {
    if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption)
      generateColorCaption(_captionType);
    else
      generateSizeCaption(_captionType);

    if(_backupColorProperty)
      delete _backupColorProperty;

    _backupColorProperty=new ColorProperty(_graph);
    *_backupColorProperty=*_colorProperty;
  }



  if(graphEvent) {
    create(_captionType);
  }
}

void CaptionItem::selectedPropertyChanged(string /*propertyName*/) {
  if(_captionType==NodesColorCaption || _captionType==EdgesColorCaption)
    generateColorCaption(_captionType);
  else
    generateSizeCaption(_captionType);

  if(_backupColorProperty)
    delete _backupColorProperty;

  _backupColorProperty=new ColorProperty(_graph);
  *_backupColorProperty=*_colorProperty;
}

}
