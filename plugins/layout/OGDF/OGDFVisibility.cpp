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
#include <ogdf/upward/VisibilityLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/upward/VisibilityLayout.cpp
/** \addtogroup layout */

/**
 * Implementation of visibility layout algorithm.
 *
 * \author Hoi-Ming Wong and Carsten Gutwenger
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 *
 * Copyright (C). All rights reserved.
 * See README.txt in the root directory of the OGDF installation for details.
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

static const char *paramHelp[] = {
    // minimum grid distance
    "The minimum grid distance.",

    // transpose
    "If true, transpose the layout vertically."};

class OGDFVisibility : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Visibility (OGDF)", "Hoi-Ming Wong", "12/11/2007",
                    "Implements a simple upward drawing algorithm based on visibility "
                    "representations (horizontal segments for nodes, vectical segments for edges).",
                    "1.1", "Hierarchical")
  OGDFVisibility(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()),
        visibility(new ogdf::VisibilityLayout()) {
    addInParameter<int>("minimum grid distance", paramHelp[0], "1");
    addInParameter<bool>("transpose", paramHelp[1], "false");
    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the VisibilityLayout instance
    csl->setLayoutModule(visibility);
  }

  ~OGDFVisibility() {}

  void beforeCall() {

    if (dataSet != nullptr) {
      int ival = 0;

      if (dataSet->get("minimum grid distance", ival))
        visibility->setMinGridDistance(ival);
    }
  }

  void afterCall() {
    if (dataSet != nullptr) {
      bool bval = false;

      if (dataSet->get("transpose", bval)) {
        if (bval) {
          transposeLayoutVertically();
        }
      }
    }
  }

private:
  ogdf::VisibilityLayout *visibility;
};

PLUGIN(OGDFVisibility)
