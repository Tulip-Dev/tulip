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
#include "StrengthMetric.h"

PLUGIN(StrengthMetric)

using namespace std;
using namespace tlp;

StrengthMetric::StrengthMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {}

StrengthMetric::~StrengthMetric() {}
//=============================================================
double StrengthMetric::e(std::unordered_set<tlp::node> &U, std::unordered_set<tlp::node> &V) {
  std::unordered_set<node>::const_iterator itU;
  double result = 0;
  std::unordered_set<node> *A, *B;

  if (U.size() < V.size()) {
    A = &U;
    B = &V;
  } else {
    A = &V;
    B = &U;
  }

  for (itU = A->begin(); itU != A->end(); ++itU) {
    for (auto n : graph->getInOutNodes(*itU)) {

      if (B->find(n) != B->end())
        result += 1.0;
    }
  }

  return result;
}
//=============================================================
double StrengthMetric::e(const std::unordered_set<tlp::node> &U) {
  double result = 0.0;

  for (auto u : U) {
    for (auto n : graph->getInOutNodes(u)) {
      if (U.find(n) != U.end())
        result += 1.0;
    }
  }

  return result / 2.0;
}
//=============================================================
double StrengthMetric::s(std::unordered_set<tlp::node> &U, std::unordered_set<tlp::node> &V) {
  if ((U.empty()) || (V.empty()))
    return 0;

  return (e(U, V) / double(U.size() * V.size()));
}
//=============================================================
double StrengthMetric::s(const std::unordered_set<tlp::node> &U) {
  if (U.size() < 2)
    return 0.0;

  return (e(U)) * 2.0 / double(U.size() * (U.size() - 1));
}
//=============================================================
double StrengthMetric::getEdgeValue(const tlp::edge ee) {
  auto eEnds = graph->ends(ee);
  node u = eEnds.first;
  node v = eEnds.second;
  std::unordered_set<node> Nu, Nv, Wuv;

  // Compute Nu
  for (auto n : graph->getInOutNodes(u)) {
    if (n != v)
      Nu.insert(n);
  }

  if (Nu.empty())
    return 0;

  // Compute Nv
  for (auto n : graph->getInOutNodes(v)) {
    if (n != u)
      Nv.insert(n);
  }

  if (Nv.empty())
    return 0;

  // Compute Wuv, choose the minimum set to minimize operation
  std::unordered_set<node> *A, *B;

  if (Nu.size() < Nv.size()) {
    A = &Nu;
    B = &Nv;
  } else {
    A = &Nv;
    B = &Nu;
  }

  for (auto na : *A) {
    if (B->find(na) != B->end())
      Wuv.insert(na);
  }

  std::unordered_set<node> &Mu = Nu;
  std::unordered_set<node> &Mv = Nv;

  /* Compute Mu and Mv, we do not need Nu and Nv anymore,
     thus we modify them to speed up computation
  */
  for (auto uv : Wuv) {
    Mu.erase(uv);
    Mv.erase(uv);
  }

  // compute strength metric
  double gamma3 = double(Wuv.size());
  double norm3 = double((Wuv.size() + Mv.size() + Mu.size()));

  double gamma4 = (e(Mu, Wuv) + e(Mv, Wuv) + e(Mu, Mv) + e(Wuv));
  double norm4 = (double(Mu.size() * Wuv.size() + Mv.size() * Wuv.size() + Mu.size() * Mv.size()) +
                  double(Wuv.size() * (Wuv.size() - 1)) / 2.0);

  double norm = norm3 + norm4;
  double gamma = gamma3 + gamma4;

  if (norm > 1E-5)
    gamma /= norm;
  else
    gamma = 0;

  return gamma;
}
//=============================================================
double StrengthMetric::getNodeValue(const tlp::node n) {
  //  tlp::warning() << __PRETTY_FUNCTION__ << endl;
  if (graph->deg(n) == 0)
    return 0;

  double res = 0;

  for (auto ite : graph->getInOutEdges(n)) {
    res += result->getEdgeValue(ite);
  }

  return res / double(graph->deg(n));
}
//=============================================================
bool StrengthMetric::run() {
  unsigned int steps = 0, maxSteps = graph->numberOfEdges();

  if (maxSteps < 10)
    maxSteps = 10;

  pluginProgress->showPreview(false);
  pluginProgress->setComment("Computing Strength metric on edges...");

  for (auto e : graph->edges()) {
    result->setEdgeValue(e, getEdgeValue(e));

    if ((++steps % (maxSteps / 10)) == 0) {
      if (pluginProgress->progress(steps, maxSteps) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;
    }
  }

  steps = 0;
  maxSteps = graph->numberOfNodes();

  if (maxSteps < 10)
    maxSteps = 10;

  pluginProgress->setComment("Computing Strength metric on nodes...");
  for (auto n : graph->nodes()) {
    result->setNodeValue(n, getNodeValue(n));

    if ((++steps % (maxSteps / 10)) == 0) {
      if (pluginProgress->progress(steps, maxSteps) != TLP_CONTINUE)
        return pluginProgress->state() != TLP_CANCEL;
    }
  }

  return true;
}
//=============================================================
