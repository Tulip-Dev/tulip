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
#include "StrengthMetric.h"

PLUGIN(StrengthMetric)

using namespace std;
using namespace tlp;

StrengthMetric::StrengthMetric(const tlp::PluginContext* context):DoubleAlgorithm(context) {}

StrengthMetric::~StrengthMetric() {}
//=============================================================
double StrengthMetric::e(TLP_HASH_SET<tlp::node> &U,TLP_HASH_SET<tlp::node> &V) {
  TLP_HASH_SET<node>::const_iterator itU;
  double result=0;
  TLP_HASH_SET<node> *A, *B;

  if (U.size()<V.size()) {
    A = &U;
    B=&V;
  }
  else {
    A = &V;
    B=&U;
  }

  for (itU=A->begin(); itU!=A->end(); ++itU) {
    Iterator<node> *itN=graph->getInOutNodes(*itU);

    while (itN->hasNext()) {
      node itn=itN->next();

      if (B->find(itn)!=B->end()) result+=1.0;
    }

    delete itN;
  }

  return result;
}
//=============================================================
double StrengthMetric::e(const TLP_HASH_SET<tlp::node> &U) {
  TLP_HASH_SET<node>::const_iterator itU;
  double result=0.0;

  for (itU=U.begin(); itU!=U.end(); ++itU) {
    Iterator<node> *itN=graph->getInOutNodes(*itU);

    while (itN->hasNext()) {
      node itn=itN->next();

      if (U.find(itn)!=U.end()) result+=1.0;
    }

    delete itN;
  }

  return result/2.0;
}
//=============================================================
double StrengthMetric::s(TLP_HASH_SET<tlp::node> &U, TLP_HASH_SET<tlp::node> &V) {
  if ((U.empty()) || (V.empty())) return 0;

  return (e(U,V) / double(U.size()*V.size()));
}
//=============================================================
double StrengthMetric::s(const TLP_HASH_SET<tlp::node> &U) {
  if (U.size()<2) return 0.0;

  return  (e(U)) * 2.0 / double(U.size()*(U.size()-1));
}
//=============================================================
double StrengthMetric::getEdgeValue(const tlp::edge ee ) {
  const std::pair<node, node>& eEnds = graph->ends(ee);
  node u = eEnds.first;
  node v = eEnds.second;
  TLP_HASH_SET<node> Nu,Nv,Wuv;

  //Compute Nu
  Iterator<node> *itN = graph->getInOutNodes(u);

  while (itN->hasNext()) {
    node n=itN->next();

    if (n!=v) Nu.insert(n);
  }

  delete itN;

  if (Nu.empty()) return 0;

  //Compute Nv
  itN=graph->getInOutNodes(v);

  while (itN->hasNext()) {
    node n=itN->next();

    if (n!=u) Nv.insert(n);
  }

  delete itN;

  if (Nv.empty()) return 0;

  //Compute Wuv, choose the minimum set to minimize operation
  TLP_HASH_SET<node> *A, *B;

  if (Nu.size()<Nv.size()) {
    A = &Nu;
    B=&Nv;
  }
  else {
    A = &Nv;
    B=&Nu;
  }

  TLP_HASH_SET<node>::const_iterator itNu;

  for (itNu=A->begin(); itNu!=A->end(); ++itNu) {
    if (B->find(*itNu)!=B->end()) Wuv.insert(*itNu);
  }

  TLP_HASH_SET<node> &Mu = Nu;
  TLP_HASH_SET<node> &Mv = Nv;

  /* Compute Mu and Mv, we do not need Nu and Nv anymore,
     thus we modify them to speed up computation
  */
  for (itNu=Wuv.begin(); itNu!=Wuv.end(); ++itNu) {
    Mu.erase(*itNu);
    Mv.erase(*itNu);
  }

  //compute strength metric
  double gamma3 = double(Wuv.size());
  double norm3  = double((Wuv.size()+Mv.size()+Mu.size()));

  double gamma4 = (e(Mu,Wuv) +  e(Mv,Wuv) +  e(Mu,Mv) +  e(Wuv));
  double norm4  = (double(Mu.size() * Wuv.size() +
                          Mv.size() * Wuv.size() +
                          Mu.size() * Mv.size() ) +
                   double(Wuv.size()*(Wuv.size()-1)) / 2.0);

  double norm = norm3 + norm4;
  double gamma = gamma3 + gamma4;

  if (norm > 1E-5)
    gamma /= norm;
  else
    gamma = 0;

  return gamma;
}
//=============================================================
double StrengthMetric::getNodeValue(const tlp::node n ) {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  if (graph->deg(n)==0) return 0;

  double res=0;
  Iterator<edge> *itE = graph->getInOutEdges(n);

  while (itE->hasNext()) {
    edge ite = itE->next();
    res += result->getEdgeValue(ite);
  }

  delete itE;
  return res/double(graph->deg(n));
}
//=============================================================
bool StrengthMetric::run() {
  edge e;
  unsigned int steps = 0, maxSteps = graph->numberOfEdges();

  if (maxSteps < 10)
    maxSteps = 10;

  if (pluginProgress) {
    pluginProgress->showPreview(false);
    pluginProgress->setComment("Computing Strength metric on edges...");
  }

  forEach(e, graph->getEdges()) {
    result->setEdgeValue(e, getEdgeValue(e));

    if (pluginProgress && ((++steps % (maxSteps / 10)) == 0)) {
      pluginProgress->progress(++steps, maxSteps);

      if (pluginProgress->state() !=TLP_CONTINUE)
        return pluginProgress->state()!= TLP_CANCEL;
    }
  }
  node n;
  steps = 0;
  maxSteps = graph->numberOfNodes();

  if (maxSteps < 10)
    maxSteps = 10;

  pluginProgress->setComment("Computing Strength metric on nodes...");
  forEach(n, graph->getNodes()) {
    result->setNodeValue(n, getNodeValue(n));

    if (pluginProgress && ((++steps % (maxSteps / 10)) == 0)) {
      pluginProgress->progress(++steps, maxSteps);

      if (pluginProgress->state() !=TLP_CONTINUE)
        return pluginProgress->state()!= TLP_CANCEL;
    }
  }

  return true;
}
//=============================================================
