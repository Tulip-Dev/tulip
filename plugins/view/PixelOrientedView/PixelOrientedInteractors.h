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

#ifndef PIXELORIENTEDVIEWINTERACTORS_H_
#define PIXELORIENTEDVIEWINTERACTORS_H_

#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include "../../utils/PluginNames.h"

namespace tlp {

class PixelOrientedInteractor : public NodeLinkDiagramComponentInteractor {

public:
  PixelOrientedInteractor(const QString &iconPath, const QString &text,
                          const unsigned int priority = 0);

  bool isCompatible(const std::string &viewName) const override;
};

class PixelOrientedInteractorNavigation : public PixelOrientedInteractor {

public:
  PLUGININFORMATION(InteractorName::PixelOrientedInteractorNavigation, "Tulip Team", "02/04/2009",
                    "Pixel Oriented Navigation Interactor", "1.0", "Navigation")

  PixelOrientedInteractorNavigation(const tlp::PluginContext *);

  void construct() override;
};
} // namespace tlp

#endif /* PIXELORIENTEDVIEWINTERACTORS_H_ */
