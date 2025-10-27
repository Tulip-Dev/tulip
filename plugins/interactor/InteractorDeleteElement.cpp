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

#include <tulip/MouseInteractors.h>
#include <tulip/NodeLinkDiagramComponentInteractor.h>
#include <tulip/NodeLinkDiagramComponent.h>
#include <tulip/StandardInteractorPriority.h>

using namespace tlp;

/** \brief Tulip interactor to delete an element
 *
 */
class InteractorDeleteElement : public NodeLinkDiagramComponentInteractor {

public:
  PLUGININFORMATION("InteractorDeleteElement", "Tulip Team", "01/04/2009",
                    "Delete Element Interactor", "1.0", "Modification")
  /**
   * Default constructor
   */
  InteractorDeleteElement(const tlp::PluginContext *)
      : NodeLinkDiagramComponentInteractor(":/tulip/gui/icons/i_del.png", "Delete nodes or edges",
                                           StandardInteractorPriority::DeleteElement) {}

  /**
   * Construct chain of responsibility
   */
  void construct() override {
    setConfigurationWidgetText(
        QString("<h3>Delete nodes or edges</h3>") +
        "<b>Mouse left</b> click on an element to delete it.<br/>No "
        "deletion confirmation will be asked.<br/><br/>" +
        "<u>Navigation in the graph</u><br/><br/>" +
        "Translation: <ul><li><b>Arrow</b> keys</li></ul>" +
#if !defined(__APPLE__)
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> up/down</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#else
        "Zoom/Unzoom: <ul><li><b>Mouse wheel</b> down/up</li><li> or <b>Pg up/Pg "
        "down</b> keys</li></ul>"
#endif
    );
    push_back(new MouseNKeysNavigator(false));
    push_back(new MouseElementDeleter);
  }

  bool isCompatible(const std::string &viewName) const override {
    return (viewName == NodeLinkDiagramComponent::viewName);
  }
};

PLUGIN(InteractorDeleteElement)
