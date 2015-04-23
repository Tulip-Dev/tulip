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

#ifndef BENDSTOOLS_H
#define BENDSTOOLS_H

#include <tulip/LayoutProperty.h>
#include <tulip/Node.h>
#include <vector>

class BendsTools {
public:
  static  std::vector<tlp::node> bendsSimplification( std::vector<tlp::node> &bends, tlp::LayoutProperty * layout);

private:
  BendsTools();

  static bool straightLine(tlp::LayoutProperty *  layout, const tlp::node a, const tlp::node b, const tlp::node c);
  static double cosAlpha(tlp::LayoutProperty *  layout, const tlp::node a, const tlp::node b, const tlp::node c);
};

#endif // BENDSTOOLS_H
