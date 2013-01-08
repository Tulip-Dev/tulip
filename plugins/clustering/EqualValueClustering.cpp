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

#include "EqualValueClustering.h"

//================================================================================
using namespace std;
using namespace tlp;

PLUGIN(EqualValueClustering)

namespace {
const char * paramHelp[] = {
  // selectedNodes
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "PropertyInterface*" ) \
  HTML_HELP_BODY() \
  "Property used to partition the graph" \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "String Collection" ) \
  HTML_HELP_DEF("values", "nodes <BR> edges") \
  HTML_HELP_DEF( "default", "nodes" )  \
  HTML_HELP_BODY() \
  "Graph elements to partition"  \
  HTML_HELP_CLOSE(),
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "values", "[true, false]" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, the resulting subgraphs are guaranted to be connected." \
  HTML_HELP_CLOSE()
};
}
#define ELT_TYPE "Type"
#define ELT_TYPES "nodes;edges;"
#define NODE_ELT 0
#define EDGE_ELT 1
//================================================================================
EqualValueClustering::EqualValueClustering(tlp::PluginContext* context):Algorithm(context) {
  addInParameter<PropertyInterface*>("Property", paramHelp[0], "viewMetric");
  addInParameter<StringCollection>(ELT_TYPE, paramHelp[1], ELT_TYPES);
  addInParameter<bool>("Connected", paramHelp[2], "false");
}
//===============================================================================
bool EqualValueClustering::run() {
  PropertyInterface *property=NULL;
  StringCollection eltTypes(ELT_TYPES);
  bool connected = false;
  eltTypes.setCurrent(0);

  if (dataSet!=NULL) {
    dataSet->get("Property", property);
    dataSet->get(ELT_TYPE, eltTypes);
    dataSet->get("Connected", connected);
  }

  if (property == NULL)
    property = graph->getProperty("viewMetric");

  const bool onNodes = eltTypes.getCurrent() == NODE_ELT;

  StableIterator<node> itN(graph->getNodes());
  StableIterator<edge> itE(graph->getEdges());
  int step = 0, maxSteps = graph->numberOfNodes();

  // property name is used when naming created sub graphs
  const string pName = property->getName();

  if (maxSteps < 100)
    maxSteps = 100;

  if (pluginProgress)
    pluginProgress->setComment(onNodes ? "Partitioning nodes..." : "Partitioning edges");

  vector<Graph *> subGraphs;

  // try to work with double value if it's a DoubleProperty
  if (typeid(*property) == typeid(DoubleProperty)) {
    TLP_HASH_MAP<double, Graph *> partitions;
    DoubleProperty *metric = (DoubleProperty *) property;

    if (onNodes) {
      while (itN.hasNext()) {
        Graph *sg;
        node n = itN.next();
        const double& tmp = metric->getNodeValue(n);

        if (partitions.find(tmp) == partitions.end()) {
          sg = graph->addSubGraph();
          stringstream sstr;
          sstr << pName << ": " << tmp;
          sg->setAttribute("name", sstr.str());
          partitions[tmp]=sg;
          subGraphs.push_back(sg);
        }
        else
          sg = partitions[tmp];

        sg->addNode(n);

        if (pluginProgress && ((++step % (maxSteps/100)) == 0)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }
      }

      step = 0;
      maxSteps = graph->numberOfEdges();

      if (maxSteps < 100)
        maxSteps = 100;

      if (pluginProgress)
        pluginProgress->setComment("Partitioning edges...");

      while(itE.hasNext()) {
        edge ite = itE.next();
        pair<node, node> eEnds = graph->ends(ite);
        const double& tmp = metric->getNodeValue(eEnds.first);

        if (tmp == metric->getNodeValue(eEnds.second)) {
          partitions[tmp]->addEdge(ite);
        }

        if (pluginProgress && ((++step % (maxSteps/100)) == 0)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }
      }
    }
    else {
      while (itE.hasNext()) {
        Graph *sg;
        edge e = itE.next();
        const double& tmp = metric->getEdgeValue(e);

        if (partitions.find(tmp) == partitions.end()) {
          sg = graph->addSubGraph();
          stringstream sstr;
          sstr << pName << ": " << tmp;
          sg->setAttribute("name", sstr.str());
          partitions[tmp]=sg;
          subGraphs.push_back(sg);
        }
        else
          sg = partitions[tmp];

        pair<node, node> eEnds = graph->ends(e);
        sg->addNode(eEnds.first);
        sg->addNode(eEnds.second);
        sg->addEdge(e);

        if (pluginProgress && ((++step % (maxSteps/100)) == 0)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }
      }
    }
  }
  else {
    TLP_HASH_MAP<string, Graph *> partitions;

    if (onNodes) {
      while (itN.hasNext()) {
        Graph *sg;
        node n = itN.next();
        string tmp=property->getNodeStringValue(n);

        if (partitions.find(tmp)==partitions.end()) {
          sg = graph->addSubGraph();
          stringstream sstr;
          sstr << pName << ": " << tmp;
          sg->setAttribute("name", sstr.str());
          partitions[tmp]=sg;
          subGraphs.push_back(sg);
        }
        else
          sg = partitions[tmp];

        sg->addNode(n);

        if (pluginProgress && ((++step % (maxSteps/100)) == 0)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }
      }

      step = 0;
      maxSteps = graph->numberOfEdges();

      if (maxSteps < 100)
        maxSteps = 100;

      if (pluginProgress)
        pluginProgress->setComment("Partitioning edges...");

      while(itE.hasNext()) {
        edge ite = itE.next();
        pair<node, node> eEnds = graph->ends(ite);
        string tmp = property->getNodeStringValue(eEnds.first);

        if (tmp == property->getNodeStringValue(eEnds.second)) {
          partitions[tmp]->addEdge(ite);
        }

        if (pluginProgress && ((++step % (maxSteps/100)) == 0)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }
      }
    }
    else {
      while (itE.hasNext()) {
        Graph *sg;
        edge e = itE.next();
        string tmp = property->getEdgeStringValue(e);

        if (partitions.find(tmp) == partitions.end()) {
          sg = graph->addSubGraph();
          stringstream sstr;
          sstr << pName << ": " << tmp;
          sg->setAttribute("name", sstr.str());
          partitions[tmp]=sg;
          subGraphs.push_back(sg);
        }
        else
          sg = partitions[tmp];

        pair<node, node> eEnds = graph->ends(e);
        sg->addNode(eEnds.first);
        sg->addNode(eEnds.second);
        sg->addEdge(e);

        if (pluginProgress && ((++step % (maxSteps/100)) == 0)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }
      }
    }
  }

  if (connected) {
    // loop on subgraphs to only have connected components
    for (unsigned int i = 0; i < subGraphs.size(); ++i) {
      Graph* sg = subGraphs[i];
      std::vector<std::set<node> > components;
      // compute the connected components's subgraph
      ConnectedTest::computeConnectedComponents(sg, components);

      if (components.size() > 1) {
        string name;
        sg->getAttribute("name", name);
        // remove the orginal subgraph
        graph->delSubGraph(sg);

        // create new subgraphs with same name
        for (unsigned int i = 0; i < components.size(); ++i) {
          sg = graph->inducedSubGraph(components[i]);
          stringstream sstr;
          sstr << name << " [" << i << ']';
          sg->setAttribute("name", sstr.str());
        }
      }
    }
  }

  return true;
}

//================================================================================
