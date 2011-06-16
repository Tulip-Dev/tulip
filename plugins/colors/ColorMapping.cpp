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
#include <cmath>

#include <tulip/TulipPlugin.h>
#include <tulip/Color.h>
#include <tulip/Vector.h>
#include <tulip/ColorScale.h>
#include "DoubleStringsListRelationDialog.h"

#include <tulip/GlComposite.h>
#include <tulip/GlRect.h>
#include <tulip/GlLabel.h>

using namespace std;
using namespace tlp;

void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v ) {
  float theMin, theMax, delta;
  theMin = std::min(std::min(r, g), b); //MIN( MIN( r, g) , b );
  theMax = std::max(std::max(r, g), b); //MAX( MAX( r, g) , b );
  *v = theMax;                               // v
  delta = theMax - theMin;
  if( theMax != 0 )
    *s = delta / theMax;               // s
  else {
    // r = g = b = 0                // s = 0, v is undefined
    *s = 0;
    *h = -1;
    return;
  }
  if( r == theMax )
    *h = ( g - b ) / delta;         // between yellow & magenta
  else if( g == theMax )
    *h = 2 + ( b - r ) / delta;     // between cyan & yellow
  else
    *h = 4 + ( r - g ) / delta;     // between magenta & cyan
  *h *= 60;                               // degreesxk
  if( *h < 0 )
    *h += 360;
}

void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ) {
        int i;
        float f, p, q, t;
        if( s == 0 ) { // achromatic (grey)
	  *r = *g = *b = v;
	  return;
        }
        h /= 60;               // sector 0 to 5
        i = (int)floor( h );
        f = h - (float)i;      // factorial part of h
        p = v * ( 1 - s );
        q = v * ( 1 - s * f );
        t = v * ( 1 - s * ( 1 - f ) );
        switch( i ) {
	case 0:
	  *r = v;*g = t;*b = p;
	  break;
	case 1:
	  *r = q;*g = v;*b = p;
	  break;
	case 2:
	  *r = p;*g = v;*b = t;
	  break;
	case 3:
	  *r = p;*g = q;*b = v;
	  break;
	case 4:
	  *r = t;*g = p;*b = v;
	  break;
	default:                // case 5:
	  *r = v;*g = p;*b = q;
	  break;
        }
}

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "DoubleProperty" ) \
    HTML_HELP_BODY() \
    "This metric is used to affect scalar values to graph items." \
    "The meaning of theses values depends of the choosen color model." \
    HTML_HELP_CLOSE(),
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "PropertyInterface*" ) \
    HTML_HELP_BODY() \
    "This metric is used to affect scalar values to graph items." \
    "The meaning of theses values depends of the choosen color model." \
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF("values", "linear <BR> uniform <BR> enumerated") \
    HTML_HELP_DEF( "default", "linear" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the type of color mapping"	\
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF("values", "nodes <BR> edges") \
    HTML_HELP_DEF( "default", "nodes" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the target of the color mapping"	\
    HTML_HELP_CLOSE(),
    // color1
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "ColorScale" ) \
    HTML_HELP_BODY() \
    "This is the color scale used." \
    HTML_HELP_CLOSE(),

  };
}

#define ELT_TYPE "type"
#define ELT_TYPES "linear;uniform;enumerated"
#define LINEAR_ELT 0
#define UNIFORM_ELT 1
#define ENUMERATED_ELT 2

#define TARGET_TYPE "target"
#define TARGET_TYPES "nodes;edges"
#define NODES_TARGET 0
#define EDGES_TARGET 1

class ColorMapping: public ColorAlgorithm {
private:
  DoubleProperty *entryMetric;
  StringCollection eltTypes;
  StringCollection targetType;
  ColorScale colorScale;
  Vector<float,4> deltaRGBA;

public:
  ColorMapping(const PropertyContext &context):ColorAlgorithm(context),eltTypes(ELT_TYPES){
    addParameter<DoubleProperty>("linear/uniform\nproperty",paramHelp[0],"viewMetric");
    addParameter<PropertyInterface*>("enumerated\nproperty",paramHelp[1],"viewMetric");
    addParameter<StringCollection>(ELT_TYPE, paramHelp[2], ELT_TYPES);
    addParameter<StringCollection>(TARGET_TYPE, paramHelp[3], TARGET_TYPES);
    addParameter<ColorScale>("colorScale",paramHelp[4],"((229, 40, 0, 200), (255, 170, 0, 200), (255, 255, 127, 200), (156, 161, 255, 200), (75, 75, 255, 200))");
  }

  //=========================================================
  ~ColorMapping(){}
  //=========================================================
  Color getColor(double value, double range) {
    if (range==0) range = 1;
    return colorScale.getColorAtPos(value/range);
  }
  //=========================================================
  void computeEdgeColor() {
    double minE,maxE;
    minE = entryMetric->getEdgeMin(graph);
    maxE = entryMetric->getEdgeMax(graph);
    Iterator<edge> *itE=graph->getEdges();
    while(itE->hasNext()) {
      edge ite=itE->next();
      double dd=entryMetric->getEdgeValue(ite)-minE;
      colorResult->setEdgeValue(ite, getColor(dd,maxE-minE));
    } delete itE;
  }
  //=========================================================
  void computeNodeColor() {
    double minN,maxN;
    minN=entryMetric->getNodeMin(graph);
    maxN=entryMetric->getNodeMax(graph);
    Iterator<node> *itN=graph->getNodes();
    while(itN->hasNext()) {
      node itn=itN->next();
      double dd=entryMetric->getNodeValue(itn)-minN;
      colorResult->setNodeValue(itn, getColor(dd,maxN-minN));
    } delete itN;
  }
  //=========================================================
  bool run() {
    //    cerr << __PRETTY_FUNCTION__ << endl;
    eltTypes.setCurrent(LINEAR_ELT);
    targetType.setCurrent(NODES_TARGET);
    DoubleProperty* metricS = graph->getProperty<DoubleProperty>("viewMetric");

    PropertyInterface *metric = graph->getProperty("viewMetric");
    if ( dataSet!=0 ) {
      dataSet->get("linear/uniform\nproperty", metricS);
      dataSet->get("enumerated\nproperty", metric);
      dataSet->get(ELT_TYPE, eltTypes);
      dataSet->get(TARGET_TYPE, targetType);
      dataSet->get("colorScale", colorScale);
    }

    if (eltTypes.getCurrent()==LINEAR_ELT || eltTypes.getCurrent()==UNIFORM_ELT) {
      if (eltTypes.getCurrent()==LINEAR_ELT){
        entryMetric = metricS;
      }else{
        DoubleProperty *tmp= new DoubleProperty(graph);
        *tmp = *metricS;
        tmp->uniformQuantification(300);
        entryMetric = tmp;
      }

      if(targetType.getCurrent()==NODES_TARGET && graph->numberOfNodes()!=0)
        computeNodeColor();
      if(targetType.getCurrent()==EDGES_TARGET && graph->numberOfEdges()!=0)
        computeEdgeColor();

    }else{
      map<string,vector<unsigned int> > mapMetricElements;

      if(targetType.getCurrent()==NODES_TARGET){
        StableIterator<node> itN(graph->getNodes());
        while (itN.hasNext()) {
          node n=itN.next();
          string tmp = metric->getNodeStringValue(n);
          if(mapMetricElements.count(tmp)==0)
            mapMetricElements[tmp]=vector<unsigned int>();
          mapMetricElements[tmp].push_back(n.id);
        }
      }else{
        StableIterator<edge> itE(graph->getEdges());
        while (itE.hasNext()) {
          edge e=itE.next();
          string tmp = metric->getEdgeStringValue(e);
          if(mapMetricElements.count(tmp)==0)
            mapMetricElements[tmp]=vector<unsigned int>();
          mapMetricElements[tmp].push_back(e.id);
        }
      }

      vector<string> enumeratedValues;
      for(map<string, vector<unsigned int> >::iterator it=mapMetricElements.begin();it!=mapMetricElements.end();++it){
        enumeratedValues.push_back((*it).first);
      }

      vector<Color> enumeratedColors;
      for(unsigned int i=0;i<enumeratedValues.size();++i){
        enumeratedColors.push_back(colorScale.getColorAtPos(((float)i)/((float)((enumeratedValues.size()-1)))));
      }

      DoubleStringsListRelationDialog dialog(enumeratedValues,enumeratedColors);
      if(dialog.exec()){
        vector<pair<string,Color> > resultVector;
        dialog.getResult(resultVector);
        for(vector<pair<string,Color> >::iterator it=resultVector.begin();it!=resultVector.end();++it){
          vector<unsigned int> *elements=&mapMetricElements[(*it).first];
          for(vector<unsigned>::iterator itE=elements->begin();itE!=elements->end();++itE){
            if(targetType.getCurrent()==NODES_TARGET)
              colorResult->setNodeValue(node(*itE),(*it).second);
            else
              colorResult->setEdgeValue(edge(*itE),(*it).second);
          }
        }
      }else{
        return false;
      }

    }

    if (eltTypes.getCurrent()==UNIFORM_ELT) delete entryMetric;
    return true;
  }
  //=========================================================
  bool check(string &){
    //    cerr << __PRETTY_FUNCTION__ << endl;
    return true;
  }
};

COLORPLUGIN(ColorMapping,"Color Mapping","Mathiaut","16/09/2010","Color mapping plugin","1.0");
