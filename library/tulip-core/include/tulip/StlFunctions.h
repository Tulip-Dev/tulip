/*
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
///@cond DOXYGEN_HIDDEN

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef _TLPSTLFUNCTIONS_H
#define _TLPSTLFUNCTIONS_H

namespace tlp {

class DoubleProperty;
struct node;
struct edge;
/**
   This class enables to compare nodes and edges according to a metric,
   instances of this class are useful for using stl sort function.
 */
class LessByMetric {
public:
  LessByMetric(DoubleProperty *metric):metric(metric) {}
  bool operator() (node n1,node n2);
  bool operator() (edge e1,edge e2);
private:
  DoubleProperty *metric;
};

}
#endif
#endif // DOXYGEN_NOTFOR_DEVEL
///@endcond
