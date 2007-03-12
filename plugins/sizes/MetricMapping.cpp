//-*-c++-*-
#include <cmath>
#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

namespace {
  const char * paramHelp[] = {
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "DoubleProperty" ) \
    HTML_HELP_BODY() \
    "This metric is used to affect scalar values to graph items." \
    HTML_HELP_CLOSE(),
    // input
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "SizeProperty" ) \
    HTML_HELP_BODY() \
    "This size property is used to affect values to unselected dimensions (width, height, depth)." \
    HTML_HELP_CLOSE(),
    // width, height, depth
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "boolean" ) \
    HTML_HELP_DEF( "values", "true/false" ) \
    HTML_HELP_DEF( "default", "true" ) \
    HTML_HELP_BODY() \
    "Indicates if this parameter will be computed(box is checked) or kept(box is unchecked) from the value of input size property" \
    HTML_HELP_CLOSE(),
    // min
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "double" ) \
    HTML_HELP_BODY() \
    "Gives the minimum value of the range of computed sizes." \
    HTML_HELP_CLOSE(),
    // max
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "double" ) \
    HTML_HELP_BODY() \
    "Gives the maximum value of the range of computed sizes." \
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
    "If true the algorithm will compute the size of nodes else it will compute the size of edges :" \
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
class MetricMapping:public SizeAlgorithm {
public:
  MetricMapping(const PropertyContext &context):SizeAlgorithm(context) {
    addParameter<DoubleProperty>("property", paramHelp[0]);
    addParameter<SizeProperty>("input", paramHelp[1]);
    addParameter<bool>("width", paramHelp[2],"true");
    addParameter<bool>("height", paramHelp[2],"true");
    addParameter<bool>("depth", paramHelp[2],"true");
    addParameter<double>("min size",paramHelp[3],"1");
    addParameter<double>("max size",paramHelp[4],"10");
    addParameter<bool>("type", paramHelp[5],"true");
    addParameter<bool>("node/edge", paramHelp[6],"true");
  }
  
  ~MetricMapping(){}

  void computeNodeSize() {
    Iterator<node> *itN=graph->getNodes();
    while(itN->hasNext()) {
      node itn=itN->next();
      double sizos=min+(entryMetric->getNodeValue(itn)-shift)*(max-min)/range;
      Size result=entrySize->getNodeValue(itn);
      if (xaxis) result[0]=sizos;
      if (yaxis) result[1]=sizos;
      if (zaxis) result[2]=sizos;
      sizeResult->setNodeValue(itn, result);
    } delete itN;
  }
  
  void computeEdgeSize() {
    Iterator<edge> *itE=graph->getEdges();
    while(itE->hasNext()) {
      edge ite=itE->next();
      double sizos = min+(entryMetric->getEdgeValue(ite)-shift)*(max-min)/range;
      Size result  = entrySize->getEdgeValue(ite);
      result[0] = sizos;
      result[1] = sizos;
      sizeResult->setEdgeValue(ite, result);
    } delete itE;
  }

  bool check(std::string &errorMsg) {
    xaxis=yaxis=zaxis=true;
    min=1;
    max=10;
    nodeoredge = true;
    entryMetric=graph->getProperty<DoubleProperty>("viewMetric");
    entrySize=graph->getProperty<SizeProperty>("viewSize");
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
    return true;
  }

  bool run() {
    string metricS="viewMetric";
    string sizeS="viewSize";

    DoubleProperty *tmp = 0;
    if (!mappingType) {
      tmp = new DoubleProperty(graph);
      *tmp = *entryMetric;
      tmp->uniformQuantification(300);
      entryMetric = tmp;
    }
    if(nodeoredge) {
      range = entryMetric->getNodeMax(graph) - entryMetric->getNodeMin(graph);
      shift = entryMetric->getNodeMin(graph);
      computeNodeSize();
      edge e;
      forEach(e, graph->getEdges())
	sizeResult->setEdgeValue(e, entrySize->getEdgeValue(e));
    }
    else {
      range = entryMetric->getEdgeMax(graph) - entryMetric->getEdgeMin(graph);
      shift = entryMetric->getEdgeMin(graph);
      computeEdgeSize();
      node n;
      forEach(n, graph->getNodes())
	sizeResult->setNodeValue(n, entrySize->getNodeValue(n));
    }
    if (!mappingType) delete tmp;
    return true;
  }

private:
  DoubleProperty *entryMetric;
  SizeProperty *entrySize;
  bool xaxis,yaxis,zaxis,mappingType;
  double min,max;
  double range;
  double shift;
  bool nodeoredge;
};
/*@}*/
SIZEPLUGIN(MetricMapping,"Metric Mapping","Auber","08/08/2003","","1.0");
