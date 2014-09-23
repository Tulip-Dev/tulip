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

#include "EqualValueClustering.h"

#include <tulip/StringCollection.h>

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

  // try to work with NumericProperty
  if (dynamic_cast<NumericProperty*>(property))
    return computeClusters((NumericProperty *) property,
                           onNodes, connected);

  return computeClusters(property, onNodes, connected);
}

bool EqualValueClustering::computeClusters(NumericProperty* prop,
    bool onNodes, bool connected) {
  unsigned int step = 0;
  unsigned int maxSteps;

  TLP_HASH_MAP<double, Graph *> clusters;
  TLP_HASH_MAP<std::string, unsigned int> valuesCount;
  MutableContainer<bool> visited;
  visited.setAll(false);

  if (onNodes) {
    maxSteps = graph->numberOfNodes();

    if (pluginProgress)
      pluginProgress->setComment("Partitioning nodes...");

    // do a bfs traversal for each node
    StableIterator<node> itN(graph->getNodes());

    while (itN.hasNext()) {
      node curNode = itN.next();

      // check if curNode has been already visited
      if (!visited.get(curNode.id)) {
        // get the value of the node
        double curValue = prop->getNodeDoubleValue(curNode);
        Graph* sg;

        if (connected || (clusters.find(curValue) == clusters.end())) {
          // add a new cluster
          sg = graph->addSubGraph();
          // set its name
          string strVal = prop->getNodeStringValue(curNode);
          stringstream sstr;
          sstr << prop->getName().c_str() << ": " << strVal.c_str();

          if (connected) {
            TLP_HASH_MAP<std::string, unsigned int>::iterator itv =
              valuesCount.find(strVal);

            if (itv != valuesCount.end()) {
              itv->second += 1;
              sstr << " [" << itv->second << ']';
            }
            else
              valuesCount[strVal] = 0;
          }
          else
            clusters[curValue] = sg;

          sg->setName(sstr.str());
        }
        else
          sg = clusters[curValue];

        // add curNode in the cluster
        sg->addNode(curNode);

        if (pluginProgress && (++step % 50 == 1)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }

        // do a bfs traversal for this node
        list<node> nodesToVisit;
        visited.set(curNode.id, true);
        nodesToVisit.push_front(curNode);

        while(!nodesToVisit.empty()) {
          curNode = nodesToVisit.front();
          nodesToVisit.pop_front();
          edge curEdge;
          forEach(curEdge, graph->getInOutEdges(curNode)) {
            node neighbour = graph->opposite(curEdge, curNode);

            if (neighbour == curNode) {
              // add loop
              sg->addEdge(curEdge);
              continue;
            }

            //
            // check if neighbour has the same value
            if (curValue == prop->getNodeDoubleValue(neighbour)) {
              // check if neighbour has not been visited
              if (!visited.get(neighbour.id)) {
                // add neighbour and edge in cluster
                sg->addNode(neighbour);
                sg->addEdge(curEdge);
                // push it for further deeper exploration
                visited.set(neighbour.id, true);
                nodesToVisit.push_back(neighbour);

                if (pluginProgress && (++step % 50 == 1)) {
                  pluginProgress->progress(step, maxSteps);

                  if (pluginProgress->state() !=TLP_CONTINUE)
                    return pluginProgress->state()!= TLP_CANCEL;
                }
              }
              else {
                // check if curEdge already exist in cluster
                if (!sg->isElement(curEdge))
                  sg->addEdge(curEdge);
              }
            }
          }
        }
      }
    }
  }
  else {
    maxSteps = graph->numberOfEdges();

    if (pluginProgress)
      pluginProgress->setComment("Partitioning edges...");

    // do a bfs traversal for each edge
    StableIterator<edge> itE(graph->getEdges());

    while (itE.hasNext()) {
      edge curEdge = itE.next();

      // check if curEdge has been already visited
      if (!visited.get(curEdge.id)) {
        // get the value of the edge
        double curValue = prop->getEdgeDoubleValue(curEdge);
        Graph* sg;

        if (connected || (clusters.find(curValue) == clusters.end())) {
          // add a new cluster
          sg = graph->addSubGraph();
          // set its name
          string strVal = prop->getEdgeStringValue(curEdge);
          stringstream sstr;
          sstr << prop->getName().c_str() << ": " << strVal.c_str();

          if (connected) {
            TLP_HASH_MAP<std::string, unsigned int>::iterator itv =
              valuesCount.find(strVal);

            if (itv != valuesCount.end()) {
              itv->second += 1;
              sstr << " [" << itv->second << ']';
            }
            else
              valuesCount[strVal] = 0;
          }
          else
            clusters[curValue] = sg;

          sg->setName(sstr.str());
        }
        else
          sg = clusters[curValue];

        // add curEdge in cluster
        std::pair<node, node> ends = graph->ends(curEdge);
        sg->addNode(ends.first);
        sg->addNode(ends.second);
        sg->addEdge(curEdge);

        if (pluginProgress && (++step % 50 == 1)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }

        // do a bfs traversal for this edge
        list<node> nodesToVisit;
        nodesToVisit.push_front(ends.first);
        nodesToVisit.push_front(ends.second);
        visited.set(curEdge.id, true);

        while(!nodesToVisit.empty()) {
          node curNode = nodesToVisit.front();
          nodesToVisit.pop_front();
          forEach(curEdge, graph->getInOutEdges(curNode)) {
            // check if the edge has not been visited AND
            // if it has the same value
            if (!visited.get(curEdge.id) &&
                curValue == prop->getEdgeDoubleValue(curEdge)) {
              node neighbour = graph->opposite(curEdge, curNode);

              if (neighbour != curNode) {
                // add neighbour in cluster
                sg->addNode(neighbour);
                // and push it for further deeper exploration
                nodesToVisit.push_back(neighbour);
              }

              // add edge in cluster
              sg->addEdge(curEdge);
              visited.set(curEdge.id, true);

              if (pluginProgress && (++step % 50 == 1)) {
                pluginProgress->progress(step, maxSteps);

                if (pluginProgress->state() !=TLP_CONTINUE)
                  return pluginProgress->state()!= TLP_CANCEL;
              }
            }
          }
        }
      }
    }
  }

  return true;
}

bool EqualValueClustering::computeClusters(PropertyInterface* prop,
    bool onNodes, bool connected) {
  unsigned int step = 0;
  unsigned int maxSteps;

  TLP_HASH_MAP<std::string, Graph *> clusters;
  TLP_HASH_MAP<std::string, unsigned int> valuesCount;
  MutableContainer<bool> visited;
  visited.setAll(false);

  if (onNodes) {
    maxSteps = graph->numberOfNodes();

    if (pluginProgress)
      pluginProgress->setComment("Partitioning nodes...");

    // do a bfs traversal for each node
    StableIterator<node> itN(graph->getNodes());

    while (itN.hasNext()) {
      node curNode = itN.next();

      // check if curNode has been already visited
      if (!visited.get(curNode.id)) {
        // get the value of the node
        string curValue = prop->getNodeStringValue(curNode);
        Graph* sg;

        if (connected || (clusters.find(curValue) == clusters.end())) {
          // add a new cluster
          sg = graph->addSubGraph();
          // set its name
          stringstream sstr;
          sstr << prop->getName().c_str() << ": " << curValue.c_str();

          if (connected) {
            TLP_HASH_MAP<std::string, unsigned int>::iterator itv =
              valuesCount.find(curValue);

            if (itv != valuesCount.end()) {
              itv->second += 1;
              sstr << " [" << itv->second << ']';
            }
            else
              valuesCount[curValue] = 0;
          }
          else
            clusters[curValue] = sg;

          sg->setName(sstr.str());
        }
        else
          sg = clusters[curValue];

        // add curNode in cluster
        sg->addNode(curNode);

        if (pluginProgress && (++step % 50 == 1)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }

        // do a bfs traversal for this node
        list<node> nodesToVisit;
        visited.set(curNode.id, true);
        nodesToVisit.push_front(curNode);

        while(!nodesToVisit.empty()) {
          curNode = nodesToVisit.front();
          nodesToVisit.pop_front();
          edge curEdge;
          forEach(curEdge, graph->getInOutEdges(curNode)) {
            node neighbour = graph->opposite(curEdge, curNode);

            if (neighbour == curNode) {
              // add loop in cluster
              sg->addEdge(curEdge);
              continue;
            }

            // check if neighbour has the same value
            if (curValue == prop->getNodeStringValue(neighbour)) {
              // check if neighbour has not been visited
              if (!visited.get(neighbour.id)) {
                // add neighbour and edge in cluster
                sg->addNode(neighbour);
                sg->addEdge(curEdge);
                // push it for further deeper exploration
                visited.set(neighbour.id, true);
                nodesToVisit.push_back(neighbour);

                if (pluginProgress && (++step % 50 == 1)) {
                  pluginProgress->progress(step, maxSteps);

                  if (pluginProgress->state() !=TLP_CONTINUE)
                    return pluginProgress->state()!= TLP_CANCEL;
                }
              }
              else {
                // check if curEdge already exist in cluster
                if (!sg->isElement(curEdge))
                  sg->addEdge(curEdge);
              }
            }
          }
        }
      }
    }
  }
  else {
    maxSteps = graph->numberOfEdges();

    if (pluginProgress)
      pluginProgress->setComment("Partitioning edges...");

    // do a bfs traversal for each edge
    StableIterator<edge> itE(graph->getEdges());

    // do a bfs traversal for each node
    while (itE.hasNext()) {
      edge curEdge = itE.next();

      // check if curEdge has been already visited
      if (!visited.get(curEdge.id)) {
        // get the value of the edge
        string curValue = prop->getEdgeStringValue(curEdge);
        Graph* sg;

        if (connected || (clusters.find(curValue) == clusters.end())) {
          // add a new cluster
          sg = graph->addSubGraph();
          // set its name
          string strVal = prop->getEdgeStringValue(curEdge);
          stringstream sstr;
          sstr << prop->getName().c_str() << ": " << curValue.c_str();

          if (connected) {
            TLP_HASH_MAP<std::string, unsigned int>::iterator itv =
              valuesCount.find(curValue);

            if (itv != valuesCount.end()) {
              itv->second += 1;
              sstr << " [" << itv->second << ']';
            }
            else
              valuesCount[strVal] = 0;
          }
          else
            clusters[curValue] = sg;

          sg->setName(sstr.str());
        }
        else
          sg = clusters[curValue];

        // add curEdge in cluster
        std::pair<node, node> ends = graph->ends(curEdge);
        sg->addNode(ends.first);
        sg->addNode(ends.second);
        sg->addEdge(curEdge);

        if (pluginProgress && (++step % 50 == 1)) {
          pluginProgress->progress(step, maxSteps);

          if (pluginProgress->state() !=TLP_CONTINUE)
            return pluginProgress->state()!= TLP_CANCEL;
        }

        // do a bfs traversal for this edge
        list<node> nodesToVisit;
        nodesToVisit.push_front(ends.first);
        nodesToVisit.push_front(ends.second);
        visited.set(curEdge.id, true);

        while(!nodesToVisit.empty()) {
          node curNode = nodesToVisit.front();
          nodesToVisit.pop_front();
          forEach(curEdge, graph->getInOutEdges(curNode)) {
            // check if the edge has not been visited AND
            // if it has the same value
            if (!visited.get(curEdge.id) &&
                curValue == prop->getEdgeStringValue(curEdge)) {
              node neighbour = graph->opposite(curEdge, curNode);

              if (neighbour != curNode) {
                // add neighbour in cluster
                sg->addNode(neighbour);
                // and push it for further deeper exploration
                nodesToVisit.push_back(neighbour);
              }

              // add edge in cluster
              sg->addEdge(curEdge);
              visited.set(curEdge.id, true);

              if (pluginProgress && (++step % 50 == 1)) {
                pluginProgress->progress(step, maxSteps);

                if (pluginProgress->state() !=TLP_CONTINUE)
                  return pluginProgress->state()!= TLP_CANCEL;
              }
            }
          }
        }
      }
    }
  }

  return true;
}






//================================================================================
