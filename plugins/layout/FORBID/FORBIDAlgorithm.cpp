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

#include <algorithm>
#include <numeric>

#include <tulip/GraphParallelTools.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include "randomkit.h"

using namespace std;
using namespace tlp;

/** \file
 *  \brief  This plugin is an implementation of the FORBID Overlap Removal
 *  algorithm first published as:
 *  Giovannangeli, L., Lalanne, F., Giot, R., & Bourqui, R. (2022, September).
 *  FORBID: Fast Overlap Removal By stochastic gradIent Descent for Graph Drawing.
 *  In International Symposium on Graph Drawing and Network Visualization (pp. 61-76). Cham:
 * Springer International Publishing. doi: https://dx.doi.org/10.1007/978-3-031-22203-0_6 It is
 * fully inspired by the original source code found in
 *  https://github.com/LoannGio/FORBID/tree/master/src
 */

typedef Vec2f Coord2D;
typedef Vec2f Size2D;

struct Rect2D {
  Coord2D pos;
  Size2D size;
  inline auto x() const {
    return pos.x();
  }
  inline auto y() const {
    return pos.y();
  }
  inline auto width() const {
    return size.width();
  }
  inline auto height() const {
    return size.height();
  }

  Rect2D(float x = 0., float y = 0., float w = 0., float h = 0.) {
    pos.x() = x;
    pos.y() = y;
    size.width() = w;
    size.height() = h;
  }
};

bool overlapCheck(Rect2D const &r1, Rect2D const &r2) {
  auto w1 = r1.width() / 2;
  auto h1 = r1.height() / 2;
  auto w2 = r2.width() / 2;
  auto h2 = r2.height() / 2;
  double minR = std::min(r1.x() + w1, r2.x() + w2);
  double maxL = std::max(r1.x() - w1, r2.x() - w2);
  if (maxL >= minR)
    return false;
  double minT = std::min(r1.y() + h1, r2.y() + h2);
  double maxB = std::max(r1.y() - h1, r2.y() - h2);
  if (maxB >= minT)
    return false;
  return true;
}

void sortNodesByX(vector<Rect2D> &rects, vector<size_t> &sortedIdx) {
  // sort indexes according to x() values
  std::stable_sort(sortedIdx.begin(), sortedIdx.end(),
                   [rects](size_t i1, size_t i2) { return rects[i1].x() < rects[i2].x(); });
}

bool scanLineOverlapCheck(vector<Rect2D> &rects, vector<size_t> &sortedIdx) {
  sortNodesByX(rects, sortedIdx);

  auto numNodes = rects.size();
  for (size_t i = 0; i < numNodes; ++i) {
    Rect2D &ri = rects[sortedIdx[i]];
    for (size_t j = i + 1; j < numNodes; ++j) {
      Rect2D &rj = rects[sortedIdx[j]];
      if (overlapCheck(ri, rj))
        return true;
      else if ((rj.x() - rj.width() / 2) > (ri.x() + ri.width() / 2))
        break;
    }
  }
  return false;
}

vector<tuple<int, int>> getAllOverlaps(vector<Rect2D> &rects, vector<size_t> &sortedIdx) {

  sortNodesByX(rects, sortedIdx);

  vector<tuple<int, int>> overlaps;
  auto numNodes = rects.size();
  for (size_t i = 0; i < numNodes; ++i) {
    Rect2D &ri = rects[sortedIdx[i]];
    for (size_t j = i + 1; j < numNodes; ++j) {
      Rect2D &rj = rects[sortedIdx[j]];
      if (overlapCheck(ri, rj))
        overlaps.emplace_back(make_tuple(sortedIdx[i], sortedIdx[j]));
      else if ((rj.x() - rj.width() / 2) > (ri.x() + ri.width() / 2))
        break;
    }
  }
  return overlaps;
}

inline double vecNorm2D(double vec_x, double vec_y) {
  return sqrt(vec_x * vec_x + vec_y * vec_y);
}

inline double dist2D(Coord2D const &p1, Coord2D const &p2) {
  return vecNorm2D(p1.x() - p2.x(), p1.y() - p2.y());
}

double maxScaleRatio(vector<Rect2D> &rects, vector<size_t> &sortedIdx) {
  double padding = 1e-4;
  double maxRatio = 1.;
  auto overlaps = getAllOverlaps(rects, sortedIdx);
  for (unsigned int i = 0; i < overlaps.size(); ++i) {
    auto [u, v] = overlaps[i];
    auto actualDist = dist2D(rects[u].pos, rects[v].pos);

    double actualX = rects[u].x() - rects[v].x();
    double actualY = rects[u].y() - rects[v].y();
    double desiredWidth = (rects[u].width() + rects[v].width()) / 2 + padding;
    double desiredHeight = (rects[u].height() + rects[v].height()) / 2 + padding;

    double widthRatio = desiredWidth / actualX;
    double heightRatio = desiredHeight / actualY;

    double unoverlapRatio = min(abs(widthRatio), abs(heightRatio));
    actualX *= unoverlapRatio;
    actualY *= unoverlapRatio;

    double optimalDist = vecNorm2D(actualX, actualY);
    double ratio = optimalDist / actualDist;

    maxRatio = max(maxRatio, ratio);
  }
  return maxRatio;
}

void scaleLayout(vector<Rect2D> &rects, double scaleFactor) {
  auto numNodes = rects.size();
  for (size_t i = 0; i < numNodes; ++i) {
    rects[i].pos *= scaleFactor;
  }
}

bool isCurrentScaleSolvable(vector<Rect2D> &rects) {
  double areas_sum = 0;
  double min_x = DBL_MAX;
  double min_y = DBL_MAX;
  double max_x = -DBL_MAX;
  double max_y = -DBL_MAX;

  auto numNodes = rects.size();
  for (size_t i = 0; i < numNodes; ++i) {
    Rect2D &r = rects[i];
    double x = r.x();
    double y = r.y();
    double w = r.width();
    double h = r.height();
    areas_sum += w * h;
    double left = x - w / 2;
    double right = x + w / 2;
    double top = y + h / 2;
    double bot = y - h / 2;
    if (left < min_x)
      min_x = left;
    if (right > max_x)
      max_x = right;
    if (bot < min_y)
      min_y = bot;
    if (top > max_y)
      max_y = top;
  }
  double bb_area = (max_x - min_x) * (max_y - min_y);
  return bb_area >= areas_sum;
}

struct term {
  size_t i, j;
  double d, w;
  bool o;
  term(size_t i, size_t j, double d, double w, bool o) : i(i), j(j), d(d), w(w), o(o) {}
  term(size_t i, size_t j, double d, double w) : i(i), j(j), d(d), w(w) {}
};

// S_GD2 function, taken from https://github.com/jxz12/s_gd2/blob/master/cpp/s_gd2/layout.cpp
vector<double> schedule(const vector<term> &terms, int t_max, double eps) {
  double w_min = terms[0].w, w_max = terms[0].w;
  for (size_t i = 1; i < terms.size(); i++) {
    const double &w = terms[i].w;
    if (w < w_min)
      w_min = w;
    if (w > w_max)
      w_max = w;
  }
  double eta_max = 1.0 / w_min;
  double eta_min = eps / w_max;
  double lambda = log(eta_max / eta_min) / (t_max - 1);

  // initialize step sizes
  vector<double> etas;
  etas.reserve(t_max);
  for (int t = 0; t < t_max; t++)
    etas.push_back(eta_max * exp(-lambda * t));

  return etas;
}

void fisheryates_shuffle(vector<term> &terms, rk_state &rstate) {
  size_t n = terms.size();
  for (size_t i = n - 1; i >= 1; i--) {
    unsigned j = rk_interval(i, &rstate);
    term temp = terms[i];
    terms[i] = terms[j];
    terms[j] = temp;
  }
}

vector<term> layoutToTerms(vector<Rect2D> &rects, float alpha, float k) {
  auto numNodes = rects.size();
  vector<term> terms;
  for (size_t i = 0; i < numNodes; i++) {
    Rect2D &ri = rects[i];

    for (size_t j = i + 1; j < numNodes; j++) {
      double d_ij, w_ij;
      bool overlap;
      Rect2D &rj = rects[j];

      if (overlapCheck(ri, rj)) {
        // there is overlap
        d_ij = vecNorm2D((ri.width() + rj.width()) / 2, (ri.height() + rj.height()) / 2);
        w_ij = pow(d_ij, k * alpha);
        overlap = true;
      } else {
        d_ij = dist2D(ri.pos, rj.pos);
        w_ij = pow(d_ij, alpha);
        overlap = false;
      }
      terms.push_back(term(i, j, d_ij, w_ij, overlap));
    }
  }
  return terms;
}

// S_GD2 optim algorithm, adapted from
// https://github.com/jxz12/s_gd2/blob/master/cpp/s_gd2/layout.cpp
void OPTIMIZATION_PASS(vector<Rect2D> &rects, vector<term> &terms, const vector<double> &etas,
                       float alpha, float k, float minMove) {
  rk_state rstate;
  rk_seed(0, &rstate);
  double mvt_sum;
  unsigned int i_eta;
  for (i_eta = 0; i_eta < etas.size(); i_eta++) {
    const double eta = etas[i_eta];

    unsigned n_terms = terms.size();
    if (n_terms == 0)
      return;
    fisheryates_shuffle(terms, rstate);

    mvt_sum = 0;
    for (unsigned i_term = 0; i_term < n_terms; i_term++) {
      const term &t = terms[i_term];
      /*if (true || t.o)*/ {
        double mu = eta * t.w;
        if (mu > 1)
          mu = 1;
        Coord2D &pi = rects[t.i].pos;
        Coord2D &pj = rects[t.j].pos;
        double dx = pi.x() - pj.x();
        double dy = pi.y() - pj.y();
        double mag = sqrt(dx * dx + dy * dy);

        if (mag != 0) {
          double r = (mu * (mag - t.d)) / (2 * mag);
          double r_x = r * dx;
          double r_y = r * dy;
          mvt_sum += abs(r_x) + abs(r_y);
          pi.x() -= r_x;
          pi.y() -= r_y;
          pj.x() += r_x;
          pj.y() += r_y;
        }
      }
    }
    if (mvt_sum < minMove)
      break;
    terms = layoutToTerms(rects, alpha, k);
  }
}

class FORBIDAlgorithm : public tlp::LayoutAlgorithm {
  const double eps = 0.01;
  float alpha, k, minMove, scaleStep;
  unsigned int maxIter, maxPasses;
  bool prime;

public:
  PLUGININFORMATION(
      "FORBID", "P. Mary", "01/08/2024",
      "Implements the FORBID algorithm, an overlap removal "
      "algorithm first published as:<br/>"
      "<b>FORBID: Fast Overlap Removal By stochastic gradIent "
      "Descent for Graph Drawing</b>,<br/>"
      "Giovannangeli, L., Lalanne, F., Giot, R., & Bourqui, R. (2022, September)."
      " In International Symposium on Graph Drawing and Network Visualization (pp. 61-76). Cham: Springer International Publishing.<br/>"
      "doi: <a href=\"https://dx.doi.org/10.1007/978-3-031-22203-0_6\">10.1007/978-3-031-22203-0_6</a>",
      "1.0", "Misc")

  FORBIDAlgorithm(const tlp::PluginContext *context)
      : tlp::LayoutAlgorithm(context), alpha(2), k(4), minMove(0.000001), scaleStep(0.1),
        maxIter(30), maxPasses(100), prime(true) {
    addInParameter<LayoutProperty>("initial layout", "The property used as input of nodes layout.",
                                   "viewLayout");
    addInParameter<SizeProperty>("bounding box", "The property used for nodes sizes.", "viewSize");
    addInParameter<float>(
        "alpha",
        "The weight factor for ideal distance for both overlapped and non-overlapped pairs of nodes",
        "2");
    addInParameter<float>("k", "The additional weight factor for overlapped pairs of nodes", "4");
    addInParameter<float>(
        "minimal movement",
        "The threshold value for the optimization pass. The pass ends, if the sum of nodes movement is below that threshold.",
        "0.000001");
    addInParameter<unsigned int>(
        "max iterations",
        "The maximum number of iterations in each pass in the optimization algorithm", "30");
    addInParameter<unsigned int>(
        "max passes", "The maximum number of passes before exiting (should not be reached)", "100");
    addInParameter<float>(
        "scale step", "The minimal step size that stops the binary search for the optimal scale",
        "0.1");
    addInParameter<bool>("prime", "Indicates whether to use the FORBID or FORBID' variant", "true");
  }

  ~FORBIDAlgorithm() override {}

  void passInOptim(vector<Rect2D> &rects) {
    vector<term> orig_terms = layoutToTerms(rects, alpha, k);
    vector<double> etas = schedule(orig_terms, maxIter, eps);
    OPTIMIZATION_PASS(rects, orig_terms, etas, alpha, k, minMove);
  }

  bool run() override {
    if (pluginProgress) {
      // user cannot interact while computing
      pluginProgress->showPreview(false);
      pluginProgress->showStops(false);
    }

    LayoutProperty *viewLayout = nullptr;
    SizeProperty *viewSize = nullptr;

    if (dataSet != nullptr) {
      dataSet->get("initial layout", viewLayout);
      dataSet->get("bounding box", viewSize);
      dataSet->get("alpha", alpha);
      dataSet->get("k", k);
      dataSet->get("minimal movement", minMove);
      dataSet->get("scale step", scaleStep);
      dataSet->get("max iterations", maxIter);
      dataSet->get("max passes", maxPasses);
      dataSet->get("prime", prime);
    }
    if (viewLayout == nullptr)
      viewLayout = graph->getProperty<LayoutProperty>("viewLayout");

    if (viewSize == nullptr)
      viewSize = graph->getProperty<SizeProperty>("viewSize");

    // initialize result for edges
    result->setAllEdgeValue(viewLayout->getEdgeDefaultValue());
    for (auto e : viewLayout->getNonDefaultValuatedEdges())
      result->setEdgeValue(e, viewLayout->getEdgeValue(e));

    // set initial nodes pos, sizes, and sorted indexes
    auto numNodes = graph->numberOfNodes();
    vector<Rect2D> rects(numNodes);
    vector<size_t> sortedIdx(numNodes);
    TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
      auto pos = viewLayout->getNodeValue(n);
      rects[i].pos.set(pos.x(), pos.y());
      auto size = viewSize->getNodeValue(n);
      rects[i].size.set(size.width(), size.height());
      sortedIdx[i] = i;
    });

    // nothing to do if there is no overlap
    if (scanLineOverlapCheck(rects, sortedIdx) ||
        // or it can be solved at the current scale
        (passInOptim(rects), scanLineOverlapCheck(rects, sortedIdx))) {
      // scale layout loop
      double upperScale = maxScaleRatio(rects, sortedIdx);
      double lowerScale = 1.;
      double prevScale = 1.;
      double curScale = 1.;
      unsigned int numPasses = 0;
      vector<Rect2D> *scaledRects = nullptr;
      while (numPasses++ < maxPasses) {
        curScale = (upperScale + lowerScale) / 2;
        double scaleFactor = curScale / prevScale;
        prevScale = curScale;
        if (prime) {
          if (!scaledRects)
            scaledRects = new vector<Rect2D>(rects);
          scaleLayout(*scaledRects, scaleFactor);
          rects = *scaledRects;
        } else
          scaleLayout(rects, scaleFactor);

        passInOptim(rects);

        if (scanLineOverlapCheck(rects, sortedIdx))
          lowerScale = curScale;
        else { // no overlap
          if (upperScale - lowerScale < scaleStep)
            break;
          else
            upperScale = curScale;
        }
      }
      delete scaledRects;
    }
    // finally set values
    TLP_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
      result->setNodeValue(n, Coord(rects[i].pos, 0.));
    });

    return true;
  }
};

PLUGIN(FORBIDAlgorithm)
