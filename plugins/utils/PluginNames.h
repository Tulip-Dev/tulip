/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef VIEWNAMES_H
#define VIEWNAMES_H
namespace tlp {
namespace ViewName {

const static char GeographicViewName[] = "Geographic view";
const static char HistogramViewName[] = "Histogram view";
const static char MatrixViewName[] = "Adjacency Matrix view";
const static char ParallelCoordinatesViewName[] = "Parallel Coordinates view";
const static char PixelOrientedViewName[] = "Pixel Oriented view";
const static char ScatterPlot2DViewName[] = "Scatter Plot 2D view";
const static char SOMViewName[] = "Self Organizing Map view";
}

namespace SelectionAlgorithm {

const static char LoopSelection[] = "Loop Selection";
const static char MultipleEdgeSelection[] = "Multiple Edges Selection";
const static char InducedSubGraphSelection[] = "Induced Sub-Graph";
const static char ReachableSubGraphSelection[] = "Reachable Sub-Graph";
const static char SpanningDagSelection[] = "Spanning Dag";
const static char MakeSelectionGraph[] = "Make Selection a Graph";
}
}

#endif // VIEWNAMES_H
