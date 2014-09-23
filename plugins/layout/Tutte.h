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
#ifndef _TutteLAYOUT_H
#define _TutteLAYOUT_H
#include <cmath>
#include <tulip/TulipPluginHeaders.h>

/** \addtogroup layout */

/** This plugin is an implementation of the Tutte layout for 3-Connected graph
 *  algorithm first published as:
 *
 *  W.T. Tutte,
 *  "How to Draw a Graph",
 *  "Proc. London Math. Soc.",
 *  "1963",
 *  pages 743-768.
 *
*/
class Tutte:public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("3-Connected (Tutte)","David Auber","06/11/2002","Beta","1.0","Planar")
  Tutte(const tlp::PluginContext* context);
  ~Tutte();

  bool run();
  bool check(std::string &);
};

#endif
