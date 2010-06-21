/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#include <tulip/MouseInteractors.h>
#include <tulip/MouseMagicSelector.h>

#include "NodeLinkDiagramComponentInteractor.h"

using namespace tlp;

/** \brief Tulip interactor to do a magic selection
 *
 */
class InteractorMagicSelection  : public NodeLinkDiagramComponentInteractor {

public:

  /**
   * Default constructor
   */
  InteractorMagicSelection():NodeLinkDiagramComponentInteractor(":/i_magic.png","Selection of reachable elements with equal value"){
    setPriority(3);
    setConfigurationWidgetText(QString("<h3>Magic selection interactor</h3>")+
                   "Select neighbour elements with same metric");
  }

  /**
   * Construct chain of responsibility
   */
  void construct(){
    pushInteractorComponent(new MousePanNZoomNavigator);
    pushInteractorComponent(new MouseMagicSelector);
  }
};

INTERACTORPLUGIN(InteractorMagicSelection, "InteractorMagicSelection", "Tulip Team", "01/04/2009", "Magic Selection Interactor", "1.0");
