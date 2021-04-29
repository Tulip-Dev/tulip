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

#ifndef SOMVIEWINTERACTOR_H_
#define SOMVIEWINTERACTOR_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include "../../utils/PluginNames.h"

namespace tlp {

class SOMViewInteractor : public tlp::NodeLinkDiagramComponentInteractor {
public:
  SOMViewInteractor(const QString &iconPath, const QString &text, const unsigned int priority = 0);

  bool isCompatible(const std::string &viewName) const override;
};

class SOMViewNavigation : public SOMViewInteractor {

  PLUGININFORMATION(InteractorName::SOMViewNavigation, "Dubois Jonathan", "02/04/2009",
                    "Standard navigation with scale", "1.0", "Navigation")

public:
  SOMViewNavigation(tlp::PluginContext *);

  void construct() override;
};

class SOMViewSelection : public SOMViewInteractor {

  PLUGININFORMATION("SOMViewSelection", "Dubois Jonathan", "02/04/2009",
                    "Standard selection with scale", "1.0", "Selection")

public:
  SOMViewSelection(tlp::PluginContext *);

  void construct() override;
};

class SOMViewProperties : public SOMViewInteractor {

  PLUGININFORMATION("SOMViewProperties", "Dubois Jonathan", "02/04/2009",
                    "Standard show element info with scale", "1.0", "Information")

public:
  SOMViewProperties(tlp::PluginContext *);

  void construct() override;
};

class SOMViewThreshold : public SOMViewInteractor {

  PLUGININFORMATION("SOMViewThreshold", "Dubois Jonathan", "02/04/2009",
                    "Threshold selection on SOM", "1.0", "Information")

public:
  SOMViewThreshold(tlp::PluginContext *);

  void construct() override;
};
} // namespace tlp
#endif /* SOMVIEWINTERACTOR_H_ */
