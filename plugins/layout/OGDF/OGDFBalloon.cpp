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
#include <ogdf/misclayout/BalloonLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/misclayout/BallonLayout.cpp
/** \addtogroup layout */
/// Layout for trees that can also be applied to general graphs.
/**
 *
 * It is partially based on the papers by Lin/Yen and Carriere/Kazman
 *
 * \author Karsten Klein
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
    // Even angles
    "Subtrees may be assigned even angles or angles depending on their size."};

class OGDFBalloon : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Balloon (OGDF)", "Karsten Klein", "13/11/2007",
                    "Computes a radial (balloon) layout based on a spanning tree.<br/>The "
                    "algorithm is partially based on the paper <b>On Balloon Drawings of Rooted "
                    "Trees</b> by Lin and Yen and on <b>Interacting with Huge Hierarchies: Beyond "
                    "Cone Trees</b> by Carriere and Kazman. ",
                    "1.4", "Hierarchical")
  OGDFBalloon(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()),
        balloon(new ogdf::BalloonLayout()) {
    addInParameter<bool>("Even angles", paramHelp[0], "false", false);
    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the BalloonLayout instance
    csl->setLayoutModule(balloon);
  }
  ~OGDFBalloon() {}

  void beforeCall() {

    if (dataSet != NULL) {
      bool val = false;

      if (dataSet->get("Even angles", val))
        balloon->setEvenAngles(val);
    }
  }

private:
  ogdf::BalloonLayout *balloon;
};

PLUGIN(OGDFBalloon)
