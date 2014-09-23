/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <tulip/MouseInteractors.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include "../utils/StandardInteractorPriority.h"

using namespace tlp;

/** \brief Tulip interactor to delete an element
 *
 */
class InteractorDeleteElement  : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorDeleteElement", "Tulip Team", "01/04/2009", "Delete Element Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorDeleteElement(const tlp::PluginContext*):NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_del.png","Delete nodes or edges") {
    setPriority(StandardInteractorPriority::DeleteElement);
    setConfigurationWidgetText(QString("<h3>Delete nodes or edges</h3>")+
                               "<b>Mouse left</b> click on an element to delete it.<br/>No deletion confirmation will be asked.");
  }

  /**
   * Construct chain of responsibility
   */
  void construct() {
    push_back(new MousePanNZoomNavigator);
    push_back(new MouseElementDeleter);
  }

  bool isCompatible(const std::string &viewName) const {
    return (viewName==NodeLinkDiagramComponent::viewName);
  }


};

PLUGIN(InteractorDeleteElement)
