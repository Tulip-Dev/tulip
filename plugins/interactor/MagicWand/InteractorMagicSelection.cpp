/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/InteractorComposite.h>

#include "../NodeLinkDiagramComponentInteractor.h"
#include "MagicSelectionInteractorConfigWidget.h"
#include "MouseMagicWandSelector.h"
#include <tulip/View.h>
#include <tulip/Graph.h>

using namespace tlp;

/** \brief Tulip interactor to do a magic selection
 *
 */
class InteractorMagicSelection  : public NodeLinkDiagramComponentInteractor {
public:
  PLUGININFORMATIONS("InteractorMagicSelection", "Tulip Team", "01/04/2009", "Magic Selection Interactor", "1.0", "Modification")
  /**
  * Default constructor
  */
  InteractorMagicSelection(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_magic.png","Fuzzy selection: select neighboring nodes based on their value of a metric."), configWidget(NULL) {
    setPriority(3);
    setConfigurationWidgetText(QString("<h3>Magic selection interactor</h3>")+
                               "Select neighbour elements which have the same value on a metric");
  }

  /**
  * Construct chain of responsibility
  */
  void construct() {
    configWidget = new MagicSelectionInteractorConfigWidget();
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseMagicWandSelector(configWidget));
  }

private:
  MagicSelectionInteractorConfigWidget* configWidget;
};

PLUGIN(InteractorMagicSelection)
