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
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>
#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/upward/UpwardPlanarizationLayout.h
/** \addtogroup layout */

/**
 * upward planarization layout algorithm.
 *
 * \author Hoi-Ming Wong
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 * Copyright (C) 2005-2008
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
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

using namespace tlp;

static const char *paramHelp[] = {
    // transpose
    "If true, transpose the layout vertically."};

class OGDFUpwardPlanarization : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Upward Planarization (OGDF)", "Hoi-Ming Wong", "12/11/2007",
                    "Implements an alternative to the classical Sugiyama approach. It adapts the "
                    "planarization approach for hierarchical graphs and produces significantly "
                    "less crossings than Sugiyama layout.",
                    "1.1", "Hierarchical")
  OGDFUpwardPlanarization(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()) {
    addInParameter<bool>("transpose", paramHelp[0], "false");
    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    csl->setLayoutModule(new ogdf::UpwardPlanarizationLayout());
  }

  ~OGDFUpwardPlanarization() override {}

  void afterCall() override {
    if (dataSet != nullptr) {
      bool bval = false;

      if (dataSet->get("transpose", bval)) {
        if (bval) {
          transposeLayoutVertically();
        }
      }
    }
  }
};

PLUGIN(OGDFUpwardPlanarization)
