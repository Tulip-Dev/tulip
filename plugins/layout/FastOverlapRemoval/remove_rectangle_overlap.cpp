/**
 * \brief remove overlaps between a set of rectangles.
 *
 * Authors:
 *   Tim Dwyer <tgdwyer@gmail.com>
 *
 * Copyright (C) 2005 Authors
 *
 * Released under GNU LGPL.  Read the file 'COPYING' for more information.
 */

#include <iostream>
#include <cassert>
#include <vector>

#include <tulip/tulipconf.h>
#include <tulip/ParallelTools.h>

#include "generate-constraints.h"
#include "solve_VPSC.h"
#include "variable.h"
#include "constraint.h"

#ifdef RECTANGLE_OVERLAP_LOGGING
#include <fstream>
#include "blocks.h"
using std::endl;
using std::ios;
using std::ofstream;
#endif

#define EXTRA_GAP 0.0001
using namespace vpsc;
using namespace tlp;
/**
 * Takes an array of n rectangles and moves them as little as possible
 * such that rectangles are separated by at least xBorder horizontally
 * and yBorder vertically
 *
 * Works in three passes:
 * 1) removes some overlap horizontally
 * 2) removes remaining overlap vertically
 * 3) a last horizontal pass removes all overlap starting from original
 *    x-positions - this corrects the case where rectangles were moved
 *    too much in the first pass.
 */
void removeRectangleOverlap(unsigned n, Rectangle rs[], double &xBorder, double &yBorder) {
  try {
    // The extra gap avoids numerical imprecision problems
    xBorder += EXTRA_GAP;
    yBorder += EXTRA_GAP;
    std::vector<Variable> vs(n);

    Constraint **cs;
    double *oldX = new double[n];
    unsigned m = ConstraintsGenerator(n).generateXConstraints(rs, vs.data(), cs, true);

    TLP_PARALLEL_MAP_INDICES(n, [&](unsigned int i) { oldX[i] = vs[i].desiredPosition; });

    Solver vpsc_x(n, vs.data(), m, cs);
#ifdef RECTANGLE_OVERLAP_LOGGING
    ofstream f(LOGFILE, ios::app);
    f << "Calling VPSC: Horizontal pass 1" << endl;
    f.close();
#endif
    vpsc_x.solve();

    TLP_PARALLEL_MAP_INDICES(n, [&](unsigned int i) { rs[i].moveCentreX(vs[i].position()); });

    for (unsigned i = 0; i < m; ++i) {
      delete cs[i];
    }

    delete[] cs;
    // Removing the extra gap here ensures things that were moved to be adjacent to
    // one another above are not considered overlapping
    xBorder -= EXTRA_GAP;
    m = ConstraintsGenerator(n).generateYConstraints(rs, vs.data(), cs);
    Solver vpsc_y(n, vs.data(), m, cs);
#ifdef RECTANGLE_OVERLAP_LOGGING
    f.open(LOGFILE, ios::app);
    f << "Calling VPSC: Vertical pass" << endl;
    f.close();
#endif
    vpsc_y.solve();

    TLP_PARALLEL_MAP_INDICES(n, [&](unsigned int i) {
      rs[i].moveCentreY(vs[i].position());
      rs[i].moveCentreX(oldX[i]);
    });

    delete[] oldX;

    for (unsigned i = 0; i < m; ++i) {
      delete cs[i];
    }

    delete[] cs;
    yBorder -= EXTRA_GAP;
    m = ConstraintsGenerator(n).generateXConstraints(rs, vs.data(), cs, false);
    Solver vpsc_x2(n, vs.data(), m, cs);
#ifdef RECTANGLE_OVERLAP_LOGGING
    f.open(LOGFILE, ios::app);
    f << "Calling VPSC: Horizontal pass 2" << endl;
    f.close();
#endif
    vpsc_x2.solve();

    for (unsigned i = 0; i < m; ++i) {
      delete cs[i];
    }

    delete[] cs;

    TLP_PARALLEL_MAP_INDICES(n, [&](unsigned int i) { rs[i].moveCentreX(vs[i].position()); });

  } catch (char const *str) {
    std::cerr << str << std::endl;

    for (unsigned i = 0; i < n; i++) {
      std::cerr << rs[i] << std::endl;
    }
  }
}

void removeRectangleOverlapX(unsigned n, Rectangle rs[], double &xBorder, double &yBorder) {
  try {
    // The extra gap avoids numerical imprecision problems
    yBorder = (xBorder += EXTRA_GAP);
    std::vector<Variable> vs(n);

    Constraint **cs;
    unsigned m = ConstraintsGenerator(n).generateXConstraints(rs, vs.data(), cs, false);
    Solver vpsc_x(n, vs.data(), m, cs);
#ifdef RECTANGLE_OVERLAP_LOGGING
    ofstream f(LOGFILE, ios::app);
    f << "Calling VPSC: Horizontal pass 1" << endl;
    f.close();
#endif
    vpsc_x.solve();

    TLP_PARALLEL_MAP_INDICES(n, [&](unsigned int i) { rs[i].moveCentreX(vs[i].position()); });

    for (unsigned i = 0; i < m; ++i) {
      delete cs[i];
    }

    delete[] cs;

  } catch (char const *str) {
    std::cerr << str << std::endl;

    for (unsigned i = 0; i < n; i++) {
      std::cerr << rs[i] << std::endl;
    }
  }
}

void removeRectangleOverlapY(unsigned n, Rectangle rs[], double &yBorder) {
  try {
    // The extra gap avoids numerical imprecision problems
    yBorder += EXTRA_GAP;
    std::vector<Variable> vs(n);

    Constraint **cs;
    unsigned int m = ConstraintsGenerator(n).generateYConstraints(rs, vs.data(), cs);
    Solver vpsc_y(n, vs.data(), m, cs);
#ifdef RECTANGLE_OVERLAP_LOGGING
    f.open(LOGFILE, ios::app);
    f << "Calling VPSC: Vertical pass" << endl;
    f.close();
#endif
    vpsc_y.solve();

    TLP_PARALLEL_MAP_INDICES(n, [&](unsigned int i) { rs[i].moveCentreY(vs[i].position()); });

    for (unsigned i = 0; i < m; ++i) {
      delete cs[i];
    }

    delete[] cs;
  } catch (char const *str) {
    std::cerr << str << std::endl;

    for (unsigned i = 0; i < n; i++) {
      std::cerr << rs[i] << std::endl;
    }
  }
}
