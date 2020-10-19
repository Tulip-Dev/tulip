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
#ifndef LinLogAlgorithm_H_
#define LinLogAlgorithm_H_

// An implementation of the LinLog layout algorithm
/** This plugin is an implementation of the LinLog
 *  algorithm first published as:
 *
 * Andreas Noack. \n
 * "Energy Models for Graph Clustering", \n
 * in Journal of Graph \n
 * Algorithms and Applications 11(2):453-480, 2007
 * doi: <a href=\"https://dx.doi.org/10.7155/jgaa.00154\">10.7155/jgaa.00154</a>,
 *
 * The code of the plugin has been translated and adapted
 * from A. Noack's java implementation of the LinLog algorithm
 */
#include <tulip/TulipPluginHeaders.h>

class LinLogAlgorithm : public tlp::LayoutAlgorithm {

public:
  PLUGININFORMATION("LinLog", "B. Renoust", "11/02/13",
                    "Implements the LinLog layout algorithm, an energy model layout algorithm, "
                    "first published as:<br/>"
                    "<b>Energy Models for Graph Clustering</b>, Andreas Noack., "
                    "Journal of Graph Algorithms and Applications 11(2):453-480, 2007, "
                    "doi: <a href=\"https://dx.doi.org/10.7155/jgaa.00154\">10.7155/jgaa.00154</a>",
                    "1.0", "Force Directed")

  LinLogAlgorithm(const tlp::PluginContext *context);

  ~LinLogAlgorithm() override;

  bool run() override;
};

#endif
