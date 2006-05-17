//-*-c++-*-
#include <cmath>
#include <tulip/TulipPlugin.h>
#include <tulip/ForEach.h>

using namespace std;

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "MetricProxy" ) \
    HTML_HELP_BODY() \
    "This metricProxy is used to affect scalar values to superGraph items." \
    HTML_HELP_CLOSE(),
    // input
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "SizesProxy" ) \
    HTML_HELP_BODY() \
    "This sizesProxy is used to affect values to unselected dimensions (width, height, depth)." \
    HTML_HELP_CLOSE(),
    // colormodel
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Boolean" ) \
    HTML_HELP_DEF( "values", "true/false" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "This value defines if this parameter will be computed or taken from the input sizesProxy" \
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
    // Mapping type
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Boolean" ) \
    HTML_HELP_DEF( "values", "true / false" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "If true the plug-in will compute node sizes else it will compute edge sizes :" \
    "<ul><li>true : node size</li><li>false: edge size</li></ul>" \
    HTML_HELP_CLOSE(),
  };
}

// error msg for invalid range value
static const char* rangeErrorMsg = "max size must be greater than min size";

/** \addtogroup size */
/*@{*/
/// MetricMapping.h - Compute size of elements according to a metric.
/** This plugin enables to set the size of the grapĥ&#146;s elements
 *  according to a metric.
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class MetricMapping:public Sizes {
public:
  MetricMapping(const PropertyContext &context):Sizes(context) {
    addParameter<MetricProxy>("property", paramHelp[0]);
    addParameter<SizesProxy>("input", paramHelp[1]);
    addParameter<bool>("width", paramHelp[2],"true");
    addParameter<bool>("height", paramHelp[2],"true");
    addParameter<bool>("depth", paramHelp[2],"true");
    addParameter<double>("min size","","1");
    addParameter<double>("max size","","10");
    addParameter<bool>("type", paramHelp[3],"true");
    addParameter<bool>("node/edge", paramHelp[3],"true");
  }
  
  ~MetricMapping(){}

  void computeNodeSize() {
    Iterator<node> *itN=superGraph->getNodes();
    while(itN->hasNext()) {
      node itn=itN->next();
      double sizos=min+(entryMetric->getNodeValue(itn)-shift)*(max-min)/range;
      Size result=entrySize->getNodeValue(itn);
      if (xaxis) result[0]=sizos;
      if (yaxis) result[1]=sizos;
      if (zaxis) result[2]=sizos;
      sizesProxy->setNodeValue(itn, result);
    } delete itN;
  }
  
  void computeEdgeSize() {
    Iterator<edge> *itE=superGraph->getEdges();
    while(itE->hasNext()) {
      edge ite=itE->next();
      double sizos = min+(entryMetric->getEdgeValue(ite)-shift)*(max-min)/range;
      Size result  = entrySize->getEdgeValue(ite);
      result[0] = sizos;
      result[1] = sizos;
      sizesProxy->setEdgeValue(ite, result);
    } delete itE;
  }

  bool check(std::string &errorMsg) {
    xaxis=yaxis=zaxis=true;
    min=1;
    max=10;
    nodeoredge = true;
    entryMetric=superGraph->getProperty<MetricProxy>("viewMetric");
    entrySize=superGraph->getProperty<SizesProxy>("viewSize");
    mappingType = true;
    if ( dataSet!=0 ) {
      dataSet->get("property",entryMetric);
      dataSet->get("input",entrySize);
      dataSet->get("width",xaxis);
      dataSet->get("height",yaxis);
      dataSet->get("depth",zaxis);
      dataSet->get("min size",min);
      dataSet->get("max size",max);
      dataSet->get("type",mappingType);
      dataSet->get("node/edge",nodeoredge);
    }
    if (min >= max) {
      /*cerr << rangeErrorMsg << endl;
	pluginProgress->setError(rangeErrorMsg); */
      errorMsg = std::string(rangeErrorMsg);
      return false;
    }
  }

  bool run() {
    string metricS="viewMetric";
    string sizeS="viewSize";

    MetricProxy *tmp = 0;
    if (!mappingType) {
      tmp = new MetricProxy(superGraph);
      *tmp = *entryMetric;
      tmp->uniformQuantification(300);
      entryMetric = tmp;
    }
    if(nodeoredge) {
      range = entryMetric->getNodeMax(superGraph) - entryMetric->getNodeMin(superGraph);
      shift = entryMetric->getNodeMin(superGraph);
      computeNodeSize();
      edge e;
      forEach(e, superGraph->getEdges())
	sizesProxy->setEdgeValue(e, entrySize->getEdgeValue(e));
    }
    else {
      range = entryMetric->getEdgeMax(superGraph) - entryMetric->getEdgeMin(superGraph);
      shift = entryMetric->getEdgeMin(superGraph);
      computeEdgeSize();
      node n;
      forEach(n, superGraph->getNodes())
	sizesProxy->setNodeValue(n, entrySize->getNodeValue(n));
    }
    if (!mappingType) delete tmp;
    return true;
  }

private:
  MetricProxy *entryMetric;
  SizesProxy *entrySize;
  bool xaxis,yaxis,zaxis,mappingType;
  double min,max;
  double range;
  double shift;
  bool nodeoredge;
};
/*@}*/
SIZESPLUGIN(MetricMapping,"Metric Mapping","Auber","08/08/2003","0","0","1");
