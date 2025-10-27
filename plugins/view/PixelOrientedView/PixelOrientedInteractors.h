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

#ifndef PIXELORIENTEDVIEWINTERACTORS_H_
#define PIXELORIENTEDVIEWINTERACTORS_H_

#include <tulip/InteractorViewExplorer.h>
#include "../../utils/PluginNames.h"

namespace tlp {

class PixelOrientedInteractorNavigation : public InteractorViewExplorer {

public:
  PLUGININFORMATION(InteractorName::PixelOrientedInteractorNavigation, "Tulip Team", "02/04/2009",
                    "Explore current view", "1.0", "Navigation")

  PixelOrientedInteractorNavigation(const tlp::PluginContext *);

  bool isCompatible(const std::string &viewName) const override;
};
} // namespace tlp

#endif /* PIXELORIENTEDVIEWINTERACTORS_H_ */
