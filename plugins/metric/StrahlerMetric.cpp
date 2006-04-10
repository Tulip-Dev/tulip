//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#include <cmath>
#include <stdio.h>
#include "StrahlerMetric.h"
#include <tulip/StringProxy.h>

METRICPLUGIN(StrahlerMetric,"Strahler","David Auber","06/04/2000","Alpha","0","1");

using namespace std;

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
    bool operator()(const couple &c,const couple &d)
    {
      if ((c.r==d.r) && (c.p==d.p)) return true;
      return false;
    }
  };

  template<>
  struct less<couple> {
    bool operator()(const couple &c,const couple &d) {
      if (c.r<d.r) return true;
      if (c.r>d.r) return false;
      if (c.p<d.p) return true;
      if (c.p>d.p) return false;
      return false;
    }
  };
};

int max(int i1,int i2) {
  if (i1>i2) return i1; else return i2;
}

int min(int i1,int i2) {
  if (i1<i2) return i1; else return i2;
}
struct StackEval {
  StackEval(int f,int u):freeS(f),usedS(u){}
  int freeS,usedS;
};

struct GreaterStackEval {
  bool operator()(const StackEval e1,const StackEval e2) {
    return (e1.freeS>e2.freeS);
  }
};

Strahler StrahlerMetric::topSortStrahler(node n, int &curPref,
					 stdext::hash_map<node,int> &tofree,
					 stdext::hash_map<node,int> &prefix,
					 stdext::hash_map<node,bool> &visited,
					 stdext::hash_map<node,bool> &finished,
					 stdext::hash_map<node,Strahler> &cachedValues)
{
  visited[n]=true;
  Strahler result;
  prefix[n]=curPref;
  curPref++;
  if (superGraph->outdeg(n)==0) {finished[n]=true;return(result);}
  list<int> strahlerResult;
  list<StackEval> tmpEval;
  //Construction des ensembles pour evaluer le strahler
  Iterator<node> *itN=superGraph->getOutNodes(n);
  for (;itN->hasNext();) {
    node tmpN=itN->next();
    if (!visited[tmpN])	{
      //Arc Normal
      tofree[n]=0;
      Strahler tmpValue=topSortStrahler(tmpN,curPref,tofree,prefix,visited,finished,cachedValues);
      //Data for strahler evaluation on the spanning Dag.
      strahlerResult.push_front(tmpValue.strahler);
      //Counting current used stacks.
      tmpEval.push_back(StackEval(tmpValue.stacks-tmpValue.usedStack+tofree[n],tmpValue.usedStack-tofree[n]));
      //Looking if we need more stacks to evaluate this node.
      //old	freeStacks=max(freeStacks,tmpValue.stacks-tmpValue.usedStack+tofree[n]);
    }
    else {
      if (finished[tmpN]){
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
  } delete itN;

  //compute the minimal nested cycles
  GreaterStackEval gSE;
  tmpEval.sort(gSE);
  result.stacks=0;
  result.usedStack=0;
  for (list<StackEval>::iterator it=tmpEval.begin();it!=tmpEval.end();++it) {
    result.usedStack+=it->usedS;
    result.stacks=max(result.stacks,it->freeS+it->usedS);
    result.stacks-=it->usedS;
  }
  result.stacks=result.stacks+result.usedStack;
  //evaluation of tree strahler 
  int tmpDbl;
  int additional=0;
  int available=0;
  strahlerResult.sort();
  while (!strahlerResult.empty()) {
    tmpDbl=strahlerResult.back();
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
    "If true the strahler number will be computed for each node, it means that a spanning tree will be recomputed for each nodes complexity o(n^2)." \
    HTML_HELP_CLOSE(),
  };
}
//==============================================================================
StrahlerMetric::StrahlerMetric(const PropertyContext &context):Metric(context) {
   addParameter<bool>("allNodes", paramHelp[0], "false");
}
//==============================================================================
StrahlerMetric::~StrahlerMetric()
{}
//==============================================================================
bool StrahlerMetric::run() {
  allNodes = false;
  if (dataSet!=0) dataSet->get("allNodes", allNodes);
  stdext::hash_map<node,bool> visited;
  stdext::hash_map<node,bool> finished;
  stdext::hash_map<node,int> prefix;
  stdext::hash_map<node,int> tofree;
  stdext::hash_map<node,Strahler> cachedValues;
  int curPref=0;
  /*
    SelectionProxy *parameter=superGraph->getProperty<SelectionProxy>("viewSelection");
    Iterator<node> *it=superGraph->getNodes();
    for (;it->hasNext();) {
    node curNode=it->next();
    if ((!visited[curNode]) && (parameter->getNodeValue(curNode)) ) {
    tofree[curNode]=0;
    topSortStrahler(curNode,curPref,tofree,prefix,visited,finished,cachedValues);
    }
    } delete it;
  */
  Iterator<node> *itN=superGraph->getNodes();
  unsigned int i = 0;
  while (itN->hasNext()) {
    node itn = itN->next();
    tofree[itn] = 0;
    if (!finished[itn]) { 
      topSortStrahler(itn,curPref,tofree,prefix,visited,finished,cachedValues);
    }
    if (allNodes) {
      if (pluginProgress->progress(i++, superGraph->numberOfNodes())!=TLP_CONTINUE) break;
      metricProxy->setNodeValue(itn,sqrt((double)cachedValues[itn].strahler*(double)cachedValues[itn].strahler
					 +(double)cachedValues[itn].stacks*(double)cachedValues[itn].stacks));
      visited.clear();
      finished.clear();
      prefix.clear();
      tofree.clear();
      cachedValues.clear();
      curPref=0;
    }
  }  delete itN;

  if (!allNodes) {
    itN = superGraph->getNodes();
    while (itN->hasNext()) {
      node itn=itN->next();
      metricProxy->setNodeValue(itn,sqrt((double)cachedValues[itn].strahler*(double)cachedValues[itn].strahler
					 +(double)cachedValues[itn].stacks*(double)cachedValues[itn].stacks));
    } delete itN;
  }
  return pluginProgress->state()!=TLP_CANCEL;
}

bool StrahlerMetric::check(string &erreurMsg) {
  erreurMsg="";
  return true;
}
void StrahlerMetric::reset(){}
