/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/GraphProperty.h>
#include <tulip/StringCollection.h>
#include <tulip/Algorithm.h>
#include <tulip/IntegerProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

//==============================================================================
static const char *paramHelp[] = {
    // directed
    "If true, the graph is considered directed.",

    // recursive
    "If true, the algorithm is applied along the entire hierarchy of subgraphs.",

    // node aggregation function
    "Function used to compute a measure for a meta-node based on the values of its underlying "
    "nodes. If 'none', no value is computed.",

    // edge aggregation function
    "Function used to compute a measure for a meta-edge based on the values of its underlying "
    "edges. If 'none', no value is computed.",

    // meta-node label
    "Property used to label meta-nodes. An arbitrary underlying node is chosen and its associated "
    "value for the given property becomes the meta-node label.",

    // use name of subgraphs
    "If true, the meta-node label is the same as the name of the subgraph it represents.",

    // edge cardinality
    "If true, the property edgeCardinality is created for each meta-edge of the quotient graph "
    "(and store the number of edges it represents).",

    // layout quotient graph
    "If true, a force directed layout is computed for each quotient graph.",

    // layout clusters graphs
    "If true, a force directed layout is computed for each cluster graph."};

//===============================================================================
// define a specific calculator for viewLabel
class QuotientLabelCalculator : public AbstractStringProperty::MetaValueCalculator {
  StringProperty *sgLabel;
  bool useSubGraphName;

public:
  QuotientLabelCalculator(StringProperty *label, bool useSgName)
      : AbstractStringProperty::MetaValueCalculator(), sgLabel(label), useSubGraphName(useSgName) {}

  void computeMetaValue(AbstractStringProperty *label, node mN, Graph *sg, Graph *) override {
    if (sgLabel)
      label->setNodeValue(mN, sgLabel->getNodeValue(sg->getOneNode()));
    else if (useSubGraphName) {
      string name;
      sg->getAttribute("name", name);
      label->setNodeValue(mN, name);
    }
  }
};

// define a specific calculator for edgeCardinality
class EdgeCardinalityCalculator : public IntegerMinMaxProperty::MetaValueCalculator {
public:
  void computeMetaValue(AbstractProperty<IntegerType, IntegerType, NumericProperty> *card, edge mE,
                        Iterator<edge> *itE, Graph *) override {
    unsigned int nbEdges = 0;

    while (itE->hasNext()) {
      itE->next();
      ++nbEdges;
    }

    card->setEdgeValue(mE, nbEdges);
  }
};

#define AGGREGATION_FUNCTIONS "none;average;sum;max;min"
class QuotientClustering : public tlp::Algorithm {
public:
  PLUGININFORMATION("Quotient Clustering", "David Auber", "13/06/2001",
                    "Computes a quotient subgraph (meta-nodes pointing on subgraphs) using an "
                    "already existing subgraphs hierarchy.",
                    "1.6", "Clustering")
  //================================================================================
  QuotientClustering(PluginContext *context) : Algorithm(context) {
    addDependency("FM^3 (OGDF)", "1.4");
    addDependency("Fast Overlap Removal", "1.4");
    addInParameter<bool>("directed", paramHelp[0], "true");
    addInParameter<StringCollection>("node function", paramHelp[2], AGGREGATION_FUNCTIONS, true,
                                     "none <br> average <br> sum <br> max <br> min");
    addInParameter<StringCollection>("edge function", paramHelp[3], AGGREGATION_FUNCTIONS, true,
                                     "none <br> average <br> sum <br> max <br> min");
    addInParameter<StringProperty>("meta-node label", paramHelp[4], "", false);
    addInParameter<bool>("use name of subgraph", paramHelp[5], "false");
    addInParameter<bool>("recursive", paramHelp[1], "false");
    addInParameter<bool>("layout quotient graph(s)", paramHelp[7], "false");
    addInParameter<bool>("layout clusters", paramHelp[8], "false");
    addInParameter<bool>("edge cardinality", paramHelp[6], "false");
  }

  bool check(string &errmsg) override {
    // do nothing if there is no subgraph
    if (graph->numberOfSubGraphs() == 0) {
      errmsg = "No subgraph found. Exiting because there is nothing to do.";
      return false;
    }
    return true;
  }

  //===============================================================================
  bool run() override {
    bool directed = true, edgeCardinality = true, clustersLayout = false;
    bool recursive = false, quotientLayout = true, useSubGraphName = false;
    StringProperty *metaLabel = nullptr;
    StringCollection nodeFunctions(AGGREGATION_FUNCTIONS);
    nodeFunctions.setCurrent(0);
    StringCollection edgeFunctions(AGGREGATION_FUNCTIONS);
    edgeFunctions.setCurrent(0);

    if (dataSet != nullptr) {
      dataSet->get("directed", directed);
      dataSet->get("node function", nodeFunctions);
      dataSet->get("edge function", edgeFunctions);
      dataSet->get("edge cardinality", edgeCardinality);
      dataSet->get("recursive", recursive);
      dataSet->get("meta-node label", metaLabel);
      dataSet->get("use name of subgraph", useSubGraphName);
      dataSet->get("layout quotient graph(s)", quotientLayout);
      dataSet->get("layout clusters", clustersLayout);
    }

    string layoutName = "FM^3 (OGDF)";
    string errMsg;

    DataSet layoutParams;

    if (clustersLayout) {
      for (auto cluster : graph->subGraphs()) {
        SizeProperty *viewSize = cluster->getProperty<SizeProperty>("viewSize");
        Size minSize = viewSize->getMin(cluster);
        Size maxSize = viewSize->getMax(cluster);
        layoutParams.set("unit edge length", std::max(maxSize[0], maxSize[1]) * 5.0);
        cluster->applyPropertyAlgorithm(layoutName,
                                        cluster->getLocalProperty<LayoutProperty>("viewLayout"),
                                        errMsg, &layoutParams);
        double border = std::min(minSize[0], minSize[1]);
        layoutParams.set("x border", border);
        layoutParams.set("y border", border);
        cluster->applyPropertyAlgorithm("Fast Overlap Removal",
                                        cluster->getLocalProperty<LayoutProperty>("viewLayout"),
                                        errMsg, &layoutParams);
      }
    }

    IntegerProperty *opProp = nullptr, *cardProp = nullptr;
    Graph *quotientGraph = graph->getRoot()->addSubGraph();
    string name("quotient of ");
    string graphName = graph->getName();

    if (graphName.empty())
      name += to_string(graph->getId());
    else {
      name += graphName;

      if (graphName == "unnamed")
        name += " " + to_string(graph->getId());
    }
    quotientGraph->setName(name);

    if (!directed) {
      opProp = new IntegerProperty(quotientGraph);
      opProp->setAllEdgeValue(edge().id);
    }

    EdgeCardinalityCalculator cardCalc;

    if (edgeCardinality) {
      cardProp = quotientGraph->getLocalProperty<IntegerProperty>("edgeCardinality");
      cardProp->setMetaValueCalculator(&cardCalc);
    }

    // set specific meta value calculators
    // for most properties
    DoubleProperty::StandardMetaValueCalculator nodeFn =
        static_cast<DoubleProperty::StandardMetaValueCalculator>(nodeFunctions.getCurrent());
    DoubleProperty::StandardMetaValueCalculator edgeFn =
        static_cast<DoubleProperty::StandardMetaValueCalculator>(edgeFunctions.getCurrent());
    QuotientLabelCalculator viewLabelCalc(metaLabel, useSubGraphName);
    tlp_hash_map<PropertyInterface *, PropertyInterface::MetaValueCalculator *> prevCalcs;
    for (const string &pName : quotientGraph->getProperties()) {
      PropertyInterface *prop = quotientGraph->getProperty(pName);

      // do nothing for viewBorderWidth
      if (pName == "viewBorderWidth")
        continue;

      if (dynamic_cast<DoubleProperty *>(prop)) {
        prevCalcs[prop] = prop->getMetaValueCalculator();
        static_cast<DoubleProperty *>(prop)->setMetaValueCalculator(nodeFn, edgeFn);
      }

      if (pName == "viewLabel") {
        prevCalcs[prop] = prop->getMetaValueCalculator();
        static_cast<StringProperty *>(prop)->setMetaValueCalculator(&viewLabelCalc);
      }
    }
    // compute meta nodes, edges and associated meta values
    Iterator<Graph *> *itS = graph->getSubGraphs();
    vector<node> mNodes;
    graph->createMetaNodes(itS, quotientGraph, mNodes, directed);
    delete itS;

    IntegerProperty *viewShape = graph->getProperty<IntegerProperty>("viewShape");

    for (auto mn : mNodes) {
      viewShape->setNodeValue(mn, NodeShape::Square);
    }

    // restore previous calculators
    tlp_hash_map<PropertyInterface *, PropertyInterface::MetaValueCalculator *>::iterator itC =
        prevCalcs.begin();

    while (itC != prevCalcs.end()) {
      if (dynamic_cast<DoubleProperty *>((*itC).first)) {
        delete ((*itC).first)->getMetaValueCalculator();
      }

      ((*itC).first)->setMetaValueCalculator((*itC).second);
      ++itC;
    }

    GraphProperty *metaInfo = graph->getRoot()->getProperty<GraphProperty>("viewMetaGraph");

    // orientation
    if (!directed) {
      // for each edge
      // store opposite edge in opProp
      for (auto mE : quotientGraph->edges()) {
        auto eEnds = quotientGraph->ends(mE);
        edge op = quotientGraph->existEdge(eEnds.second, eEnds.first);

        if (op.isValid()) {
          opProp->setEdgeValue(op, mE.id);
          opProp->setEdgeValue(mE, op.id);
        }
      }
      set<edge> edgesToDel;
      DoubleProperty *viewMetric = quotientGraph->getProperty<DoubleProperty>("viewMetric");

      for (auto mE : quotientGraph->edges()) {
        edge op(opProp->getEdgeValue(mE));

        if (op.isValid() && (edgesToDel.find(mE) == edgesToDel.end()) &&
            (edgesToDel.find(op) == edgesToDel.end())) {
          // if the opposite edge viewMetric associated value is greater
          // than the mE associated value than we will keep it instead of mE
          bool opOK = viewMetric->getEdgeValue(mE) < viewMetric->getEdgeValue(op);

          if (edgeFn != DoubleProperty::NO_CALC) {
            for (auto property : graph->getObjectProperties()) {
              DoubleProperty *metric = dynamic_cast<DoubleProperty *>(property);
              if (metric) {
                auto pName = property->getName();
                // try to avoid view... properties
                if (pName.compare(0, 4, "view") != 0 || pName == "viewMetric") {
                  double value = metric->getEdgeValue(mE);
                  double opValue = metric->getEdgeValue(op);

                  switch (edgeFn) {
                  case DoubleProperty::AVG_CALC:
                    value = (value + opValue) / 2;
                    break;

                  case DoubleProperty::SUM_CALC:
                    value += opValue;
                    break;

                  case DoubleProperty::MAX_CALC:

                    if (value < opValue)
                      value = opValue;

                    break;

                  case DoubleProperty::MIN_CALC:

                    if (value > opValue)
                      value = opValue;

                    break;

                  case DoubleProperty::NO_CALC:
                    break;
                  }

                  if (opOK)
                    metric->setEdgeValue(op, value);
                  else
                    metric->setEdgeValue(mE, value);
                }
              }
            }
          }

          // compute cardinaly if needed
          if (cardProp) {
            unsigned int card = cardProp->getEdgeValue(mE) + cardProp->getEdgeValue(op);

            if (opOK)
              cardProp->setEdgeValue(op, card);
            else
              cardProp->setEdgeValue(mE, card);
          }

          // insert one of the opposite meta edges in edgesToDel
          // and insert its underlying edges in the set of the remaining one
          edge meToKeep(mE.id), meToDel(op.id);

          if (opOK)
            meToKeep = op, meToDel = mE;

          edgesToDel.insert(meToDel);
          set<edge> se = metaInfo->getEdgeValue(meToKeep);
          const set<edge> &nse = metaInfo->getEdgeValue(meToDel);

          for (auto e : nse)
            se.insert(e);

          metaInfo->setEdgeValue(meToKeep, se);
        }
      }

      for (auto e : edgesToDel)
        quotientGraph->delEdge(e);
    }

    delete opProp;

    if (dataSet != nullptr) {
      dataSet->set("quotientGraph", quotientGraph);
    }

    // layouting if needed
    if (quotientLayout) {
      SizeProperty *viewSize = quotientGraph->getProperty<SizeProperty>("viewSize");
      Size minSize = viewSize->getMin(quotientGraph);
      Size maxSize = viewSize->getMax(quotientGraph);
      layoutParams.set("unit edge length", std::max(maxSize[0], maxSize[1]) * 2.0);
      quotientGraph->applyPropertyAlgorithm(
          layoutName, quotientGraph->getLocalProperty<LayoutProperty>("viewLayout"), errMsg,
          &layoutParams);
      double border = std::min(minSize[0], minSize[1]);
      layoutParams.set("x border", border);
      layoutParams.set("y border", border);
      quotientGraph->applyPropertyAlgorithm(
          "Fast Overlap Removal", quotientGraph->getLocalProperty<LayoutProperty>("viewLayout"),
          errMsg, &layoutParams);
    }

    // recursive call if needed
    if (recursive) {
      DataSet dSet;
      dSet.set("directed", directed);
      dSet.set("node function", nodeFunctions);
      dSet.set("edge function", edgeFunctions);
      dSet.set("edge cardinality", edgeCardinality);
      dSet.set("recursive", recursive);
      dSet.set("meta-node label", metaLabel);
      dSet.set("use name of subgraph", useSubGraphName);
      dSet.set("layout quotient graph(s)", quotientLayout);

      for (node mn : mNodes) {
        Graph *sg = quotientGraph->getNodeMetaInfo(mn);
        string eMsg;
        sg->applyAlgorithm("Quotient Clustering", eMsg, &dSet, pluginProgress);

        // if a quotient graph has been computed
        // update metaInfo of current meta node
        if (dSet.getAndFree("quotientGraph", sg))
          metaInfo->setNodeValue(mn, sg);
      }
    }

    return true;
  }
};

PLUGIN(QuotientClustering)
