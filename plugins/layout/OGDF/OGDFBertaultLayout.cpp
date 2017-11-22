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

#include <ogdf/misclayout/BertaultLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/misclayout/BertaultLayout.cpp

/**
 * Computes a force directed layout (Bertault Layout) for preserving the planar embedding in the
 *graph.
 * The algorithm is based on the paper
 * "A force-directed algorithm that preserves
 * edge-crossing properties" by Francois Bertault
 *
 * \author Smit Sanghavi;
 *
 * \par License:
 * This file is part of the Open Graph Drawing Framework (OGDF).
 *
 * \par
 * Copyright (C)<br>
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation;
 * see the file LICENSE.txt included in the packaging of this file
 * for details.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

static const char *paramHelp[] = {
    // impred
    "Sets impred option.",

    // iterno
    "The number of iterations. If 0, the number of iterations will be set as 10 times the number "
    "of nodes.",

    // reqlength
    "The required edge length."};

class OGDFBertaultLayout : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Bertault (OGDF)", "Smit Sanghavi", "29/05/2015",
                    "Computes a force directed layout (Bertault Layout) for preserving the planar "
                    "embedding in the graph.",
                    "1.0", "Force Directed")
  OGDFBertaultLayout(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::BertaultLayout()) {
    addInParameter<bool>("impred", paramHelp[0], "false", false);
    addInParameter<int>("iterno", paramHelp[1], "20", false);
    addInParameter<double>("reqlength", paramHelp[2], "0.0", false);
  }
  ~OGDFBertaultLayout() {}

  void beforeCall() {
    ogdf::BertaultLayout *bertault = static_cast<ogdf::BertaultLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      bool bval = false;
      int ival = 20;
      double val = 0;

      if (dataSet->get("impred", bval))
        bertault->setImpred(bval);

      if (dataSet->get("iterno", ival))
        bertault->iterno(ival);

      if (dataSet->get("reqlength", val))
        bertault->reqlength(val);
    }
  }
};

PLUGIN(OGDFBertaultLayout)
