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
#include <ogdf/energybased/DavidsonHarelLayout.h>

#include <tulip/StringCollection.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

/** \addtogroup layout */
// comments below have been extracted from OGDF/src/energybased/DavidsonHarelLayout.cpp

/// An implementation for DavidsonHarel layout
/**
 *
 * This is the frontend for the DavidsonHarel optimization
 * function. It adds the energy functions to the problem and
 * sets their weights. It also contains functions for setting
 * and returning the parameters that influence the quality of
 * the solution and the speed of the optimization process.
 *
 * \author Rene Weiskircher
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

#define ELT_SETTINGS "Settings"
#define ELT_SETTINGSLIST "Standard;Repulse;Planar"
#define STANDARD_ELT 0
#define REPULSE_ELT 1
#define PLANAR_ELT 2

#define ELT_SPEED "Speed"
#define ELT_SPEEDLIST "Fast;Medium;HQ"
#define FAST_ELT 0
#define MEDIUM_ELT 1
#define HQ_ELT 2

static const char *paramHelp[] = {
    // Settings
    "Easy way to set fixed costs.",

    // Speed
    "Easy way to set temperature and number of iterations.",

    // preferredEdgeLength
    "The preferred edge length.",

    // preferredEdgeLengthMultiplier
    "The preferred edge length multiplier for attraction."};

class OGDFDavidsonHarel : public OGDFLayoutPluginBase {

  tlp::StringCollection settings;
  tlp::StringCollection speed;

public:
  PLUGININFORMATION("Davidson Harel (OGDF)", "Rene Weiskircher", "12/11/2007",
                    "Implements the Davidson-Harel layout algorithm which uses simulated annealing "
                    "to find a layout of minimal energy.<br/>Due to this approach, the algorithm "
                    "can only handle graphs of rather limited size.<br/>It is based on the "
                    "following publication:<br/><b>Drawing Graphs Nicely Using Simulated "
                    "Annealing</b>, Ron Davidson, David Harel,  ACM Transactions on Graphics "
                    "15(4), pp. 301-331, 1996.",
                    "1.3", "Force Directed")
  OGDFDavidsonHarel(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::DavidsonHarelLayout()) {
    addInParameter<StringCollection>(ELT_SETTINGS, paramHelp[0], ELT_SETTINGSLIST, true,
                                     "Standard <br> Repulse <br> Planar");
    addInParameter<StringCollection>(ELT_SPEED, paramHelp[1], ELT_SPEEDLIST, true,
                                     "Fast <br> Medium <br> HQ");
    addInParameter<double>("preferredEdgeLength", paramHelp[2], "0.0");
    addInParameter<double>("preferredEdgeLengthMultiplier", paramHelp[3], "2.0");
  }

  void beforeCall() {
    ogdf::DavidsonHarelLayout *davidson = static_cast<ogdf::DavidsonHarelLayout *>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
      settings.setCurrent(0);

      if (dataSet->get(ELT_SETTINGS, settings)) {
        if (settings.getCurrent() == STANDARD_ELT) {
          davidson->fixSettings(ogdf::DavidsonHarelLayout::spStandard);
        } else if (settings.getCurrent() == REPULSE_ELT) {
          davidson->fixSettings(ogdf::DavidsonHarelLayout::spRepulse);
        } else {
          davidson->fixSettings(ogdf::DavidsonHarelLayout::spPlanar);
        }
      }

      speed.setCurrent(0);

      if (dataSet->get(ELT_SPEED, speed)) {
        if (speed.getCurrent() == FAST_ELT) {
          davidson->setSpeed(ogdf::DavidsonHarelLayout::sppFast);
        } else if (speed.getCurrent() == MEDIUM_ELT) {
          davidson->setSpeed(ogdf::DavidsonHarelLayout::sppMedium);
        } else {
          davidson->setSpeed(ogdf::DavidsonHarelLayout::sppHQ);
        }
      }

      double val = 0;

      if (dataSet->get("preferredEdgeLength", val))
        davidson->setPreferredEdgeLength(val);

      if (dataSet->get("preferredEdgeLengthMultiplier", val))
        davidson->setPreferredEdgeLengthMultiplier(val);
    }
  }

  ~OGDFDavidsonHarel() {}
};

PLUGIN(OGDFDavidsonHarel)
