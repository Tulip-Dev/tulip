//-*-c++-*-
#ifndef _RESOLVE_OVERLAP_H
#define _RESOLVE_OVERLAP_H

/** \addtogroup layout */
/*@{*/
/** \file
 *  \brief A layout algorithm to resolve all node overlaps.
 *
 * An implementation of a layout algorithm resolving the problem
 * of node overlaps. It was first published as:
 *
 * Tim Dwyer, Kim Marriot, Peter J. Stuckey,
 * "Fast Node Overlap Removal" in Graph Drawing, Vol. 3843 (2006), pp. 153-164.
 * doi: <a href=\"https://doi.org/10.1007/11618058_15\">10.1007/11618058_15</a>
 *
 * <b>NOTES</b>
 *
 * The algorithm works on general graphs. It finds and applies
 * the minimum scale factor to ensure that no two pairs of nodes
 * overlap in the graph.
 * Let n be the number of nodes, the algorithm complexity is in O(n).
 * If two nodes start out at the same position the algorithm cannot
 * succeed.  It will return failure.
 *
 * <b>HISTORY</b>
 *
 *  - 2006 Version 1.0: first version by Daniel Archambault,
 *  Department of Computer Science, University of British Columbia
 *
 * <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include <tulip/LayoutProperty.h>

class FastOverlapRemoval : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Fast Overlap Removal", "Daniel Archambault", "08/12/2006",
                    "Implements a layout algorithm removing nodes overlap first published as:<br/>"
                    "<b>Fast Node Overlap Removal</b>, Tim Dwyer, Kim Marriot, Peter J. Stuckey, "
                    "Graph Drawing 2005, "
                    "Vol. 3843 (2006), pp. 153-164, doi: <a "
                    "href=\"https://doi.org/10.1007/11618058_15\">10.1007/11618058_15</a>",
                    "1.3", "Misc")

  FastOverlapRemoval(const tlp::PluginContext *context);
  inline ~FastOverlapRemoval() override {}
  bool run() override;
};
/*@}*/
#endif
