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
#include <tulip/MouseShowElementInfos.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include "../utils/StandardInteractorPriority.h"

using namespace tlp;

/** \brief Tulip interactor to get information about an element of the graph
 *
 */
class InteractorGetInformation  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATIONS("InteractorGetInformation", "Tulip Team", "01/04/2009", "Get Information Interactor", "1.0", "Information")
  /**
   * Default constructor
   */
  InteractorGetInformation(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_select.png","Get/Edit informations on node or edge") {
    setPriority(StandardInteractorPriority::GetInformation);
    setConfigurationWidgetText(QString("<h3>Get/Edit node or edge informations</h3>")+
                               "<b>Mouse left click</b> on an element to display its properties.<br/>then <b>Mouse left click</b> on a row to edit the corresponding value.");
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseShowElementInfos);
  }

};

PLUGIN(InteractorGetInformation)
