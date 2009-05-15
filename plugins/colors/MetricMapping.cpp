//-*-c++-*-
#include <cmath>

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include <tulip/TulipPlugin.h>
#include <tulip/Color.h>
#include <tulip/Vector.h>

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
    // colormodel
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "int" ) \
    HTML_HELP_DEF( "values", "[0,1]" ) \
    HTML_HELP_DEF( "default", "0" ) \
    HTML_HELP_BODY() \
    "This value defines the type of color interpolation. Following values are valid :" \
    "<ul><li>0: HSV interpolation ;</li><li>1: RGB interpolation</li></ul>" \
    HTML_HELP_CLOSE(),
    // color1
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Color" ) \
    HTML_HELP_DEF( "values", "[0,255]^4" ) \
    HTML_HELP_DEF( "default", "red" ) \
    HTML_HELP_BODY() \
    "This is the start color used in the interpolation process." \
    HTML_HELP_CLOSE(),
    // color2
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Color" ) \
    HTML_HELP_DEF( "values", "[0,255]^4" ) \
    HTML_HELP_DEF( "default", "green" ) \
    HTML_HELP_BODY() \
    "This is the end color used in the interpolation process." \
    HTML_HELP_CLOSE(),
    // Mapping type
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Boolean" ) \
    HTML_HELP_DEF( "values", "true / false" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This value defines the type of mapping. Following values are valid :" \
    "<ul><li>true : linear mapping</li><li>false: uniform quantification</li></ul>" \
    HTML_HELP_CLOSE(),
  };
}

class MetricColorMapping: public ColorAlgorithm {
private:
  DoubleProperty *entryMetric;
  int colorModel;
  Color color1;
  Color color2;
  Vector<float,4> deltaRGBA;

public:
  MetricColorMapping(const PropertyContext &context):ColorAlgorithm(context){
    addParameter<DoubleProperty>("property",paramHelp[0],"viewMetric");
    addParameter<int>("colormodel",paramHelp[1],"1");
    addParameter<bool>("type",paramHelp[4],"true");
    addParameter<Color>("color1",paramHelp[2],"(255,255,0,128)");
    addParameter<Color>("color2",paramHelp[3],"(0,0,255,228)");
  }
  //=========================================================
  ~MetricColorMapping(){}
  //=========================================================
  Color getColor(double value, double range) {
    if (range==0) range = 1;
    switch(colorModel) {
    case 0: //linear mapping on hue
      float h,s,v;
      float r,g,b;
      v=1;
      s=1;
      h=55.0+value*300.0/range;
      HSVtoRGB(&r,&g,&b,h,s,v);
      return Color((int)(r*255.0f),
		   (int)(g*255.0f),
		   (int)(b*255.0f),
		   (int)(color1[3] + deltaRGBA[3]*value));
    case 1: //RGB interpolation
    default:
      Vector<float,4> tmp(deltaRGBA);
      tmp *= value;
      return  Color((int)(tmp[0]+color1[0]),
		    (int)(tmp[1]+color1[1]),
		    (int)(tmp[2]+color1[2]),
		    (int)(tmp[3]+color1[3]));
    }
  }
  //=========================================================
  void computeEdgeColor() {
    double minE,maxE;
    minE = entryMetric->getEdgeMin(graph);
    maxE = entryMetric->getEdgeMax(graph);
    for (int i=0;i<4;++i)
      deltaRGBA[i]=double(color2[i]-color1[i]);
    if (maxE!=minE)
      deltaRGBA/=double(maxE-minE);
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
    for (int i=0;i<4;++i)
      deltaRGBA[i]=double(color2[i]-color1[i]);
    if (maxN!=minN)
      deltaRGBA/=double(maxN-minN);
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
    DoubleProperty* metricS = graph->getProperty<DoubleProperty>("viewMetric");
    colorModel=1;
    color1.set(255,255,0,128);
    color2.set(0,0,255,228);
    bool mappingType = true;
    if ( dataSet!=0 ) {
      dataSet->get("property", metricS);
      dataSet->get("colormodel", colorModel);
      dataSet->get("color1", color1);
      dataSet->get("color2", color2);
      dataSet->get("type", mappingType);
    }
    if (mappingType) {
      entryMetric = metricS;
      /*
	Iterator<node> *itN = graph->getNodes();
	while(itN->hasNext()) {
	node n = itN->next();
	entryMetric->setNodeValue(n, log(1+entryMetric->getNodeValue(n)));
	} delete itN;
      */
    }
    else {
      DoubleProperty *tmp= new DoubleProperty(graph);
      *tmp = *metricS;
      tmp->uniformQuantification(300);
      entryMetric = tmp;
    }
    if(graph->numberOfNodes()!=0)
      computeNodeColor();
    if(graph->numberOfEdges()!=0)
      computeEdgeColor();

    GlComposite *composite=new GlComposite();

    int xMax=0;
    double minN,maxN;

    minN=entryMetric->getNodeMin(graph);
    maxN=entryMetric->getNodeMax(graph);

    stringstream sstr1;
    stringstream sstr2;
    sstr1 << minN;
    sstr2 << maxN;

    GlRect *rect;
    GlRect *rect1;
    GlRect *rect2;
    GlRect *rect3;
    GlRect *rect4;
    GlRect *rect5;

    if(colorModel){
      rect=new GlRect(Coord(15,95,0),Coord(25,15,0),color2,color1);
    }else{
      rect1=new GlRect(Coord(15,31,0),Coord(25,15,0),Color(0,255,0,255),Color(255,255,0,255));
      rect2=new GlRect(Coord(15,47,0),Coord(25,31,0),Color(0,255,255,255),Color(0,255,0,255));
      rect3=new GlRect(Coord(15,63,0),Coord(25,47,0),Color(0,0,255,255),Color(0,255,255,255));
      rect4=new GlRect(Coord(15,79,0),Coord(25,63,0),Color(255,0,255,255),Color(0,0,255,255));
      rect5=new GlRect(Coord(15,95,0),Coord(25,79,0),Color(255,0,0,255),Color(255,0,255,255));
    }

    GlLabel *label1=new GlLabel(Coord(30,15,0),Coord(5+12*sstr1.str().size(),15,0),Color(0,0,0,255),true);
    label1->setText(sstr1.str());
    GlLabel *label2=new GlLabel(Coord(30,95,0),Coord(5+12*sstr2.str().size(),15,0),Color(0,0,0,255),true);
    label2->setText(sstr2.str());

    if(sstr1.str().size()>sstr2.str().size()) {
      xMax=30 + (int) label1->getSize()[0];
    }else{
      xMax=30 + (int) label2->getSize()[0];
    }

    GlRect *backRect=new GlRect(Coord(5,105,0),Coord(xMax+5,5,0),Color(0,0,0,50),Color(0,0,0,50));
    composite->addGlEntity(backRect,"backRect");

    composite->addGlEntity(label1,"min");
    composite->addGlEntity(label2,"max");

    if(colorModel){
      composite->addGlEntity(rect,"rect");
    }else{
      composite->addGlEntity(rect1,"rect1");
      composite->addGlEntity(rect2,"rect2");
      composite->addGlEntity(rect3,"rect3");
      composite->addGlEntity(rect4,"rect4");
      composite->addGlEntity(rect5,"rect5");
    }

    DataSet nodeLinkDiagramComponentDataSet;
    DataSet infoDataSet;
    string infoName="MetricMapping";

    infoDataSet.set<long>("composite",(long)composite);
    infoDataSet.set<string>("layer","Foreground");
    nodeLinkDiagramComponentDataSet.set<DataSet>(infoName,infoDataSet);
    graph->setAttribute("NodeLinkDiagramComponent",nodeLinkDiagramComponentDataSet);

    if (!mappingType) delete entryMetric;
    return true;
  }
  //=========================================================
  bool check(string &err){
    //    cerr << __PRETTY_FUNCTION__ << endl;
    return true;
  }
};

COLORPLUGIN(MetricColorMapping,"Metric Mapping","Auber","04/07/2003","Color metric mapping plugin","1.0");
