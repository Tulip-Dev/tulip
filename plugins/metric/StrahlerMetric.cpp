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

#include <cmath>
#include <cstdio>

#include <tulip/StringProperty.h>
#include <tulip/StringCollection.h>

#include "StrahlerMetric.h"

PLUGIN(StrahlerMetric)

using namespace std;
using namespace tlp;

namespace std {
struct couple {
  int p,r;
  bool operator ==(const couple&d) {
    if ((p==d.p) && (r==d.r)) return true;

    return false;
  }
};

template<>
struct equal_to<couple> {
  bool operator()(const couple &c,const couple &d) {
    if ((c.r==d.r) && (c.p==d.p)) return true;

    return false;
  }
};

template<>
struct less<couple> {
  bool operator()(const couple &c,const couple &d) {
    return (c.r < d.r) ||
           ((c.r == d.r) && (c.p < d.p));
  }
};
}

struct StackEval {
  StackEval(int f,int u):freeS(f),usedS(u) {}
  int freeS,usedS;
};

struct GreaterStackEval {
  bool operator()(const StackEval& e1,const StackEval& e2) {
    return (e1.freeS>e2.freeS);
  }
};

Strahler StrahlerMetric::topSortStrahler(tlp::node n, int &curPref,
    TLP_HASH_MAP<node,int> &tofree,
    TLP_HASH_MAP<node,int> &prefix,
    TLP_HASH_MAP<node,bool> &visited,
    TLP_HASH_MAP<node,bool> &finished,
    TLP_HASH_MAP<node,Strahler> &cachedValues) {
  visited[n]=true;
  Strahler result;
  prefix[n]=curPref;
  curPref++;

  if (graph->outdeg(n)==0) {
    finished[n]=true;
    return(result);
  }

  list<int> strahlerResult;
  list<StackEval> tmpEval;
  //Construction des ensembles pour evaluer le strahler
  Iterator<node> *itN=graph->getOutNodes(n);

  for (; itN->hasNext();) {
    node tmpN=itN->next();

    if (!visited[tmpN]) {
      //Arc Normal
      tofree[n]=0;
      Strahler tmpValue=topSortStrahler(tmpN,curPref,tofree,prefix,visited,finished,cachedValues);
      //Data for strahler evaluation on the spanning Dag.
      strahlerResult.push_front(tmpValue.strahler);
      //Counting current used stacks.
      tmpEval.push_back(StackEval(tmpValue.stacks-tmpValue.usedStack+tofree[n],tmpValue.usedStack-tofree[n]));
      //Looking if we need more stacks to evaluate this node.
      //old freeStacks=max(freeStacks,tmpValue.stacks-tmpValue.usedStack+tofree[n]);
    }
    else {
      if (finished[tmpN]) {
        if(prefix[tmpN]<prefix[n]) {
          //Cross Edge
          Strahler tmpValue=cachedValues[tmpN];
          //Data for strahler evaluation on the spanning Dag.
          strahlerResult.push_front(tmpValue.strahler);
          //Looking if we need more stacks to evaluate this node.
          tmpEval.push_back(StackEval(tmpValue.stacks,0));
        }
        else {
          //Arc descent
          Strahler tmpValue=cachedValues[tmpN];
          //Data for strahler evaluation on the spanning Dag.
          strahlerResult.push_front(tmpValue.strahler);
        }
      }
      else {
        if(tmpN==n) {
          tmpEval.push_back(StackEval(1,0));
        }
        else {
          //New nested cycle.
          tofree[tmpN]++;
          tmpEval.push_back(StackEval(0,1));
          //result.usedStack++;
        }

        //Return edge
        //Register needed tp store the result of the recursive call
        strahlerResult.push_front(1);
      }
    }
  }

  delete itN;

  //compute the minimal nested cycles
  GreaterStackEval gSE;
  tmpEval.sort(gSE);
  result.stacks=0;
  result.usedStack=0;

  for (list<StackEval>::iterator it=tmpEval.begin(); it!=tmpEval.end(); ++it) {
    result.usedStack+=it->usedS;
    result.stacks=std::max(result.stacks,it->freeS+it->usedS);
    result.stacks-=it->usedS;
  }

  result.stacks=result.stacks+result.usedStack;
  //evaluation of tree strahler
  int additional=0;
  int available=0;
  strahlerResult.sort();

  while (!strahlerResult.empty()) {
    int tmpDbl=strahlerResult.back();
    strahlerResult.pop_back();

    if (tmpDbl>available) {
      additional+=tmpDbl - available;
      available=tmpDbl - 1;
    }
    else
      available -=1;
  }

  result.strahler=additional;
  strahlerResult.clear();
  finished [n]=true;
  cachedValues[n]=result;
  return result;
}

namespace {
const char * paramHelp[] = {
  // property
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "values", "true, false" ) \
  HTML_HELP_DEF( "default", "false" ) \
  HTML_HELP_BODY() \
  "If true, for each node the Strahler number is computed from a spanning tree having that node as root: complexity o(n^2). If false the Strahler number is computed from a spanning tree having the heuristicly estimated graph center as root." \
  HTML_HELP_CLOSE(),
  //Orientation
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "String Collection" ) \
  HTML_HELP_DEF("Values", "all <BR> ramification<BR> nested cycles") \
  HTML_HELP_DEF( "default", "all" )  \
  HTML_HELP_BODY() \
  "Type of computation"  \
  HTML_HELP_CLOSE()
};
}
#define COMPUTATION_TYPE "Type"
#define COMPUTATION_TYPES "all;ramification;nested cycles;"
#define ALL 0
#define REGISTERS 1
#define STACKS 2
//==============================================================================
StrahlerMetric::StrahlerMetric(const tlp::PluginContext* context):DoubleAlgorithm(context), allNodes(false) {
  addInParameter<bool>("All nodes", paramHelp[0], "false");
  addInParameter<StringCollection>(COMPUTATION_TYPE, paramHelp[1], COMPUTATION_TYPES);
}
//==============================================================================
bool StrahlerMetric::run() {
  allNodes = false;
  StringCollection computationTypes(COMPUTATION_TYPES);
  computationTypes.setCurrent(0);

  if (dataSet!=NULL) {
    dataSet->get("All nodes", allNodes);
    dataSet->get(COMPUTATION_TYPE, computationTypes);
  }

  TLP_HASH_MAP<node,bool> visited;
  TLP_HASH_MAP<node,bool> finished;
  TLP_HASH_MAP<node,int> prefix;
  TLP_HASH_MAP<node,int> tofree;
  TLP_HASH_MAP<node,Strahler> cachedValues;
  int curPref=0;
  /*
    Selection *parameter=graph->getProperty<BooleanProperty>("viewSelection");
    Iterator<node> *it=graph->getNodes();
    for (;it->hasNext();) {
    node curNode=it->next();
    if ((!visited[curNode]) && (parameter->getNodeValue(curNode)) ) {
    tofree[curNode]=0;
    topSortStrahler(curNode,curPref,tofree,prefix,visited,finished,cachedValues);
    }
    } delete it;
  */
  Iterator<node> *itN=graph->getNodes();
  unsigned int i = 0;

  while (itN->hasNext()) {
    node itn = itN->next();
    tofree[itn] = 0;

    if (!finished[itn]) {
      topSortStrahler(itn,curPref,tofree,prefix,visited,finished,cachedValues);
    }

    if (allNodes) {
      if (pluginProgress->progress(i++, graph->numberOfNodes())!=TLP_CONTINUE) break;

      switch(computationTypes.getCurrent()) {
      case ALL:
        result->setNodeValue(itn,sqrt((double)cachedValues[itn].strahler*(double)cachedValues[itn].strahler
                                      +(double)cachedValues[itn].stacks*(double)cachedValues[itn].stacks));
        break;

      case REGISTERS:
        result->setNodeValue(itn, (double)cachedValues[itn].strahler);
        break;

      case STACKS:
        result->setNodeValue(itn, (double)cachedValues[itn].stacks);
      }

      visited.clear();
      finished.clear();
      prefix.clear();
      tofree.clear();
      cachedValues.clear();
      curPref=0;
    }
  }

  delete itN;

  if (!allNodes) {
    itN = graph->getNodes();

    while (itN->hasNext()) {
      node itn=itN->next();

      switch(computationTypes.getCurrent()) {
      case ALL:
        result->setNodeValue(itn,sqrt((double)cachedValues[itn].strahler*(double)cachedValues[itn].strahler
                                      +(double)cachedValues[itn].stacks*(double)cachedValues[itn].stacks));
        break;

      case REGISTERS:
        result->setNodeValue(itn, (double)cachedValues[itn].strahler);
        break;

      case STACKS:
        result->setNodeValue(itn, (double)cachedValues[itn].stacks);
      }
    }

    delete itN;
  }

  return pluginProgress->state()!=TLP_CANCEL;
}
