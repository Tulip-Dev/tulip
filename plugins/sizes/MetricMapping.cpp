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

#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

namespace {
const char * paramHelp[] = {
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "DoubleProperty" ) \
  HTML_HELP_DEF( "default", "\"viewMetric\"" ) \
  HTML_HELP_BODY() \
  "Metric to map to size." \
  HTML_HELP_CLOSE(),
  // input
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "SizeProperty" ) \
  HTML_HELP_DEF( "default", "\"viewSize\"" ) \
  HTML_HELP_BODY() \
  "If not all dimensions (width, height, depth) are checked below, the dimensions not computed are copied from this property." \
  HTML_HELP_CLOSE(),
  // width, height, depth
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "boolean" ) \
  HTML_HELP_DEF( "values", "true/false" ) \
  HTML_HELP_DEF( "default", "true" ) \
  HTML_HELP_BODY() \
  "Each checked dimension is adjusted to represent property, each unchecked dimension is copied from input." \
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
  "Type of mapping." \
  "<ul><li>true: linear mapping (min value of property is mapped to min size, max to max size, and a linear interpolation is used inbetween.)</li>" \
  "<li>false: uniform quantification (the values of property are sorted, and the same size increment is used between consecutive values).</li></ul>"                          \
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

  //proportional mapping
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "string" ) \
  HTML_HELP_DEF( "default", "Area Proportional" ) \
  HTML_HELP_BODY() \
  "The mapping can either be area/volume proportional, or square/cubic;" \
  "i.e. the areas/volumes will be proportional, or the dimensions (width, height and depth) will be." \
  HTML_HELP_CLOSE(),
};
}

// error msg for invalid range value
static const string rangeSizeErrorMsg = "max size must be greater than min size";
static const string rangeMetricErrorMsg = "All values are the same";
static const string AREA_PROPORTIONAL = "Area Proportional";
/** \addtogroup size */
/*@{*/
/// Metric Mapping - Compute size of elements according to a metric.
/** This plugin enables to set the size of the graph's elements
 *  according to a metric.
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class MetricSizeMapping:public SizeAlgorithm {
public:
  MetricSizeMapping(const PropertyContext &context):SizeAlgorithm(context) {
    addParameter<DoubleProperty>("property", paramHelp[0], "viewMetric");
    addParameter<SizeProperty>("input", paramHelp[1], "viewSize");
    addParameter<bool>("width", paramHelp[2],"true");
    addParameter<bool>("height", paramHelp[2],"true");
    addParameter<bool>("depth", paramHelp[2],"false");
    addParameter<double>("min size",paramHelp[3],"1");
    addParameter<double>("max size",paramHelp[4],"10");
    addParameter<bool>("type", paramHelp[5],"true");
    addParameter<bool>("node/edge", paramHelp[6],"true");
    addParameter<StringCollection>("area proportional", paramHelp[7], "Area Proportional;Quadratic/Cubic");
  }

  bool check(std::string &errorMsg) {
    xaxis=yaxis=zaxis=true;
    min=1;
    max=10;
    nodeoredge = true;
    proportional = "Area Proportional";
    entryMetric = NULL;
    entrySize = NULL;
    mappingType = true;
    StringCollection proportionalType;

    if ( dataSet!=NULL) {
      dataSet->get("property",entryMetric);
      dataSet->get("input",entrySize);
      dataSet->get("width",xaxis);
      dataSet->get("height",yaxis);
      dataSet->get("depth",zaxis);
      dataSet->get("min size",min);
      dataSet->get("max size",max);
      dataSet->get("type",mappingType);
      dataSet->get("node/edge",nodeoredge);
      dataSet->get("area proportional",proportionalType);
      proportional = proportionalType.getCurrentString();
    }

    if (!entryMetric)
      entryMetric=graph->getProperty<DoubleProperty>("viewMetric");

    if (!entrySize)
      entrySize=graph->getProperty<SizeProperty>("viewSize");

    if (min >= max) {
      errorMsg = rangeSizeErrorMsg;
      return false;
    }

    if (nodeoredge)
      range = entryMetric->getNodeMax(graph) - entryMetric->getNodeMin(graph);
    else
      range = entryMetric->getEdgeMax(graph) - entryMetric->getEdgeMin(graph);

    if (!range) {
      errorMsg = rangeMetricErrorMsg;
      return false;
    }

    if(!xaxis && !yaxis && !zaxis) {
      errorMsg = "You need at least one axis to map on.";
      return false;
    }

    if(proportional == AREA_PROPORTIONAL) {
      max = max*max;
    }

    return true;
  }

  bool run() {
    DoubleProperty *tmp = NULL;

    if (!mappingType) {
      tmp = new DoubleProperty(graph);
      *tmp = *entryMetric;
      tmp->uniformQuantification(300);
      entryMetric = tmp;
    }

    unsigned int maxIter = graph->numberOfNodes() + graph->numberOfEdges();
    unsigned int iter = 0;

    pluginProgress->showPreview(false);

    if(nodeoredge) {
      shift = entryMetric->getNodeMin(graph);

      // compute size of nodes
      Iterator<node> *itN=graph->getNodes();

      while(itN->hasNext()) {
        node itn=itN->next();
        double sizos = 0;

        if (proportional == AREA_PROPORTIONAL) {
          const double power = 1.0 / (float(xaxis) + float(yaxis) + float(zaxis));
          sizos = min + pow((entryMetric->getNodeValue(itn)-shift)*(max-min)/range, power);
        }
        else {
          sizos = min + (entryMetric->getNodeValue(itn)-shift)*(max-min)/range;
        }

        Size result=entrySize->getNodeValue(itn);

        if (xaxis) result[0]=static_cast<float>(sizos);

        if (yaxis) result[1]=static_cast<float>(sizos);

        if (zaxis) result[2]=static_cast<float>(sizos);

        sizeResult->setNodeValue(itn, result);

        if ((iter % 100 == 0) &&
            (pluginProgress->progress(iter, maxIter)!=TLP_CONTINUE)) {
          delete itN;

          if (!mappingType) delete tmp;

          return false;
        }

        ++iter;
      }

      delete itN;

      edge e;
      forEach(e, graph->getEdges()) {
        sizeResult->setEdgeValue(e, entrySize->getEdgeValue(e));

        if ((iter % 100 == 0) &&
            (pluginProgress->progress(iter, maxIter)!=TLP_CONTINUE)) {
          if (!mappingType) delete tmp;

          return false;
        }

        ++iter;
      }
    }
    else {
      shift = entryMetric->getEdgeMin(graph);
      // compute size of edges
      Iterator<edge> *itE=graph->getEdges();

      while(itE->hasNext()) {
        edge ite=itE->next();
        double sizos =
          min+(entryMetric->getEdgeValue(ite)-shift)*(max-min)/range;
        Size result  = entrySize->getEdgeValue(ite);
        result[0] = static_cast<float>(sizos);
        result[1] = static_cast<float>(sizos);
        sizeResult->setEdgeValue(ite, result);

        if ((iter % 100 == 0) &&
            (pluginProgress->progress(iter, maxIter)!=TLP_CONTINUE)) {
          delete itE;

          if (!mappingType) delete tmp;

          return pluginProgress->state()!=TLP_CANCEL;
        }

        ++iter;
      }

      delete itE;

      node n;
      forEach(n, graph->getNodes()) {
        sizeResult->setNodeValue(n, entrySize->getNodeValue(n));

        if ((iter % 100 == 0) &&
            (pluginProgress->progress(iter, maxIter)!=TLP_CONTINUE)) {
          if (!mappingType) delete tmp;

          return pluginProgress->state()!=TLP_CANCEL;
        }

        ++iter;
      }
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
  std::string proportional;
};
/*@}*/
SIZEPLUGIN(MetricSizeMapping,"Metric Mapping","Auber","08/08/2003","","2.0")
