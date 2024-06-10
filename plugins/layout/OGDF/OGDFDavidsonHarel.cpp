/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#define SETTINGSLIST "standard;repulse;planar"
#define STANDARD_ELT 0
#define REPULSE_ELT 1
#define PLANAR_ELT 2

#define SPEEDLIST "fast;medium;hq"
#define FAST_ELT 0
#define MEDIUM_ELT 1
#define HQ_ELT 2

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // Settings
    "Fixes the cost values to special configurations.",

    // Speed
    "More convenient way of setting the speed of the algorithm. Influences number of iterations "
    "per temperature step, starting temperature, and cooling factor.",

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
                    "Annealing</b>,<br/>Ron Davidson, David Harel,  ACM Transactions on Graphics "
                    "15(4), pp. 301-331, 1996.",
                    "1.4", "Force Directed")
  OGDFDavidsonHarel(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::DavidsonHarelLayout() : nullptr) {
    addInParameter<StringCollection>("settings", paramHelp[0], SETTINGSLIST, true,
                                     "standard<br/>repulse<br/>planar");
    addInParameter<StringCollection>("speed", paramHelp[1], SPEEDLIST, true,
                                     "fast<br/>medium<br/>hq");
    addInParameter<double>("edge length", paramHelp[2], "0.0");
    addInParameter<double>("edge length multiplier", paramHelp[3], "2.0");
  }

  void beforeCall() override {
    ogdf::DavidsonHarelLayout *davidson = static_cast<ogdf::DavidsonHarelLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      settings.setCurrent(0);

      if (dataSet->get("settings", settings)) {
        switch (settings.getCurrent()) {
        case STANDARD_ELT:
          davidson->fixSettings(DavidsonHarelLayout::SettingsParameter::Standard);
          break;
        case REPULSE_ELT:
          davidson->fixSettings(DavidsonHarelLayout::SettingsParameter::Repulse);
          break;
        default:
          davidson->fixSettings(DavidsonHarelLayout::SettingsParameter::Planar);
        }
      }

      speed.setCurrent(0);

      if (dataSet->get("speed", speed)) {
        switch (speed.getCurrent()) {
        case FAST_ELT:
          davidson->setSpeed(DavidsonHarelLayout::SpeedParameter::Fast);
          break;
        case MEDIUM_ELT:
          davidson->setSpeed(DavidsonHarelLayout::SpeedParameter::Medium);
          break;
        default:
          davidson->setSpeed(DavidsonHarelLayout::SpeedParameter::HQ);
        }
      }

      double val = 0;

      if (dataSet->get("edge length", val))
        davidson->setPreferredEdgeLength(val);

      if (dataSet->get("edge length multiplier", val))
        davidson->setPreferredEdgeLengthMultiplier(val);
    }
  }

  ~OGDFDavidsonHarel() override {}
};

PLUGIN(OGDFDavidsonHarel)
