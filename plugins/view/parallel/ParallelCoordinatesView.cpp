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

#include <tulip/ForEach.h>
#include <tulip/GlQuad.h>
#include <tulip/GlSphere.h>
#include <tulip/GlCircle.h>
#include <tulip/GlCurve.h>

#include "ParallelCoordinatesView.h"
#include "NominalParallelAxis.h"
#include "QuantitativeParallelAxis.h"
#include "GlCylinder.h"
#include "GlNodeGlyph.h"
#include "ParallelTools.h"

#include <sstream>

namespace tlp {

ParallelCoordinatesView::ParallelCoordinatesView(ParallelCoordinatesGraphProxy *graphProxy, Coord begin,
						 unsigned int width, unsigned int height, viewType vType) :
  _begin(begin), _width(width), 
  
  _height(height) , _graphProxy(graphProxy) , _viewType(vType) {
  
  _data = new GlGraphInputData(_graphProxy, &_parameters);

  createAxis();
  plotAllData();
}

ParallelCoordinatesView::~ParallelCoordinatesView() {
  erase();
}


void ParallelCoordinatesView::createAxis(bool willChangeAxisOrder)
{
  string propertyName;
  unsigned int pos = 0;
  list<string> l = _graphProxy->getSelectedProperties();
  list<string>::iterator it;
  
  _nbAxis = _graphProxy->getNumberOfSelectedProperties();
  
  for (it = l.begin(); it != l.end() ; ++it) {
    
    ParallelAxis *axis = NULL;

    Coord coord;
    if(_nbAxis!=1){
      if(!willChangeAxisOrder)
	coord=Coord(_begin.getX() + getPos(*it) * (_width/(_nbAxis - 1)), _begin.getY());
      else
	coord=Coord(_begin.getX() + pos * (_width/(_nbAxis - 1)), _begin.getY());
    }
    else
      coord=Coord(_begin.getX() + (_width/2), _begin.getY());
    
    string typeName = (_graphProxy->getProperty(*it))->getTypename();

    unsigned int vSize = _graphProxy->getDataMaxId();

    if (typeName == "string") {
      vector<string> vs(vSize);
      _graphProxy->fillVectorWithData((void *) &vs, *it);
      axis = new NominalParallelAxis(coord, _height, *it, &vs, _viewType);

    } else if (typeName == "int") {
      vector<int> vi(vSize);
      _graphProxy->fillVectorWithData((void *) &vi, *it);
      axis = new QuantitativeParallelAxis<int>(coord, _height, *it, &vi, true, _viewType);
      
    } else if (typeName == "double") {
      vector<double> vd(vSize);
      _graphProxy->fillVectorWithData((void *) &vd, *it);
      axis = new QuantitativeParallelAxis<double>(coord, _height, *it, &vd, true, _viewType);
    }

    if (axis != NULL) {
      addGlEntity(axis, *it);

      if(willChangeAxisOrder)
	_axisOrder.push_back(*it);

      _axis[*it] = axis;
      ++pos;
    } 
  }
}

void ParallelCoordinatesView::plotAllData() {
  
  Color c;
  if (_graphProxy->getDataLocation() == NODES) {
    node n;
    forEach(n, _graphProxy->getNodes()) {
      if (_graphProxy->isDataSelected(n.id)) {
	c = colorSelect2;
      } else {
	c = _graphProxy->getDataColor(n.id);
      }
      if (_viewType == VIEW_3D) {
	plotData(n.id, Color(c[0], c[1], c[2], 200));
      } else {
	plotData(n.id, c);
      }
    }
  } else {
    edge e;
    forEach(e, _graphProxy->getEdges()) {
      if (_graphProxy->isDataSelected(e.id)) {
	c = colorSelect2;
      } else {
	c = _graphProxy->getDataColor(e.id);
      }
      if (_viewType == VIEW_3D) {
	plotData(e.id, Color(c[0], c[1], c[2], 200));
      } else {
	plotData(e.id, c);
      }
    }
  }
  
}

void ParallelCoordinatesView::plotData(unsigned int dataIdx, const Color &color) {
  
  Size size = _graphProxy->getDataViewSize(dataIdx);
  float f = (size[0] + size[1] + size[2]) / 3.;
  if (f < 1.) {
    f = 1;
  }
  
  
  for (unsigned int j = 0 ; j < _axisOrder.size() ; j++) {
    
    Coord pointCoord = _axis[_axisOrder[j]]->getPointCoordOnAxisForData(dataIdx);
    ostringstream oss;
    oss << "data " << dataIdx << " var " << _axisOrder[j];

    
    GlSimpleEntity *point;
    
    if (_graphProxy->getDataLocation() == NODES) {
      point = new GlNodeGlyph(pointCoord, _data, node(dataIdx));
    } else if (_viewType == VIEW_3D) {
      point = new GlSphere(pointCoord, 2 * f, color);
    } else {
      point = new GlCircle(pointCoord, 2 * f, color, color, true, true);
    }
    
    addGlEntity(point, oss.str());
    _dataPlotElts[dataIdx].push_back(point);
    
    if (j != (_axisOrder.size() - 1)) {
      
      Coord endLineCoord = _axis[_axisOrder[j + 1]]->getPointCoordOnAxisForData(dataIdx);
      ostringstream oss2;
      oss2 << "data " << dataIdx <<" line " << _axisOrder[j] << " -> " << _axisOrder[j+1];
      
      GlSimpleEntity *line;
      
      if (_viewType == VIEW_3D) {
	
	double cylRadius = 0.3;
	if (_graphProxy->getDataTexture(dataIdx) == "") {
	  line = new GlCylinder(pointCoord, endLineCoord, f * cylRadius, f * cylRadius, color);
	} else {
	  line = new GlCylinder(pointCoord, endLineCoord, f * cylRadius, f * cylRadius, _graphProxy->getDataTexture(dataIdx), color); 
	}
	
      } else if (_viewType == VIEW_2D_SPLINE) {
	
	vector<Coord> v;
	Coord line1[2], line2[2];
	
	if ( j < (_axisOrder.size() - 2) ) {
	  
	  Coord c1 = _axis[_axisOrder[j + 2]]->getPointCoordOnAxisForData(dataIdx);
	  
	  line1[0] = Coord(pointCoord.getX() + (endLineCoord.getX() - pointCoord.getX()) / 2, 0, 0);
	  line1[1] = Coord(pointCoord.getX() + (endLineCoord.getX() - pointCoord.getX()) / 2, 10, 0);
	  line2[0] = endLineCoord;
	  line2[1] = c1;
	  
	  
	}  else {
	  
	  line1[0] = Coord(0, endLineCoord.getY(), 0);
	  line1[1] = Coord(10, endLineCoord.getY(), 0);
	  line2[0] = Coord(pointCoord.getX() + ((endLineCoord.getX() - pointCoord.getX()) / 2), 0, 0);
	  line2[1] = Coord(pointCoord.getX() + ((endLineCoord.getX() - pointCoord.getX()) / 2), 0, 0);
	  
	}
	
	Coord *ctrlPoint = computeStraightLineIntersection(line1, line2);
	
	if (ctrlPoint->getY() > (_begin.getY() + _height)) {
	  ctrlPoint->setY(_begin.getY() + _height);
	} else if (ctrlPoint->getY() < _begin.getY()) {
	  ctrlPoint->setY(_begin.getY());
	}

	v.push_back(pointCoord);
	v.push_back(*ctrlPoint);
	v.push_back(endLineCoord);

	double curveWidth = 0.2 * f;
	line = new GlCurve(v, color, color, curveWidth, curveWidth);

	if (_graphProxy->getDataTexture(dataIdx) != "") {
	  ((GlCurve *)line)->setTexture(_graphProxy->getDataTexture(dataIdx));
	}
	
      } else {
	
	double quadHalfWidth = 0.5 * f;
	Coord coordQuad[] = {pointCoord + Coord(0, -quadHalfWidth), endLineCoord + Coord(0, -quadHalfWidth), 
			     endLineCoord + Coord(0, quadHalfWidth), pointCoord + Coord(0, quadHalfWidth)};
	line = new GlQuad(coordQuad, color);
	if (_graphProxy->getDataTexture(dataIdx) != "") {
	  ((GlQuad *)line)->setTextureName(_graphProxy->getDataTexture(dataIdx));
	}
      }
      
      addGlEntity(line, oss2.str());
      _dataPlotElts[dataIdx].push_back(line);
      
    }
  }
}

unsigned int 
ParallelCoordinatesView::nbParallelAxis() {
    return _nbAxis;
}


void
ParallelCoordinatesView::swapAxis(string axisName1, string axisName2) {
  
  vector<string>::iterator it;
  int pi,pj;
  int pos=0;


  
  for(it=_axisOrder.begin();it!=_axisOrder.end();++it,++pos){
    if(*it==axisName1)
      pi=pos;

    if(*it==axisName2)
      pj=pos;
  }

  string tmp=_axisOrder[pi];
  _axisOrder[pi]=_axisOrder[pj];
  _axisOrder[pj]=tmp;

  

  Coord ci = _axis[axisName1]->getBaseCoord();
  Coord cj = _axis[axisName2]->getBaseCoord();
   _axis[axisName1]->translate(cj - ci);
   _axis[axisName2]->translate(ci - cj);
  eraseData();

  plotAllData();


}

void ParallelCoordinatesView::deleteDataRepresentation(const unsigned int dataId) {
  vector<GlEntity *> dataRep = _dataPlotElts[dataId];
  vector<GlEntity *>::iterator it;
  for (it = dataRep.begin() ; it != dataRep.end() ; ++it) {
    deleteGlEntity((GlSimpleEntity *)*it);
    delete *it;
  }
  _dataPlotElts[dataId].clear();
}

ParallelAxis *
ParallelCoordinatesView::getAxisByIndex(unsigned int i) {
  if (i < _axisOrder.size()) {
    return (ParallelAxis *)findGlEntity(_axisOrder[i]);
  }
  return NULL;
}

int ParallelCoordinatesView::getDataIdFromGlEntity(GlEntity *glEntity) {
  
  int ret = -1;
  
  map<unsigned int, vector<GlEntity *> >::iterator it;
  for (it = _dataPlotElts.begin() ; it != _dataPlotElts.end() ; ++it) {
    vector<GlEntity *>::iterator it2;
    for (it2 = (it->second.begin()) ; it2 != (it->second.end()) ; ++it2) {
      if (*it2 == glEntity) {
	ret = it->first;
	break;
      }
    }
    if (ret != -1)
      break;
  }

  return ret;
  
}

void ParallelCoordinatesView::selectData(const unsigned int dataId) {
  deleteDataRepresentation(dataId);
  _graphProxy->setDataSelected(dataId, true);
  plotData(dataId, colorSelect2);
}

void ParallelCoordinatesView::unselectData(const unsigned int dataId) {
  
  deleteDataRepresentation(dataId);
  _graphProxy->setDataSelected(dataId, false);
  Color c = _graphProxy->getDataColor(dataId);
  plotData(dataId, Color(c[0], c[1], c[2], 200));
  
}


void ParallelCoordinatesView::redrawView() {
  erase();
  createAxis(false);
  plotAllData();
}


void ParallelCoordinatesView::eraseData(){
 map<unsigned int, vector<GlEntity *> >::iterator it2;
  for (it2 = _dataPlotElts.begin() ; it2 != _dataPlotElts.end() ; ++it2) {
    vector<GlEntity *>::iterator it3;
    for (it3 = (it2->second).begin() ; it3 != (it2->second).end() ; ++it3) {
      deleteGlEntity((GlSimpleEntity *)*it3);
      delete *it3;
    }      
  }
  _dataPlotElts.clear();
 
}

void ParallelCoordinatesView::erase(){

  map<string, ParallelAxis *>::iterator it;
  for (it = _axis.begin() ; it != _axis.end() ; ++it) {
    deleteGlEntity(it->second);
    delete it->second;
  }
  
  _axis.clear();

  eraseData();
}

int ParallelCoordinatesView::getPos(string name){
  int pos=0;
  vector<string>::iterator it; 
 for(it=_axisOrder.begin();it!=_axisOrder.end();++it,++pos){
    if(*it==name)
      return pos;
  }
  return -1;
}


void ParallelCoordinatesView::setViewType(viewType vType) {  
  _viewType = vType;
}

const vector<string> ParallelCoordinatesView::getAxisNames() {
  return _axisOrder;
}

}

